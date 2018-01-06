#ifndef PERSISTENT_SET_HEADER
#define PERSISTENT_SET_HEADER

#define MY_DEBUG
#ifdef MY_DEBUG
#include <iostream>
//#define TRACK_DESTRUCTOR
//#define HONEST_SIZE
#endif

#include <memory>
#include <cassert>
#include <utility>
#include "smart_pointers/linked_ptr.h"
#include "smart_pointers/shared_ptr.h"

///Requirements for type T:
//Destructible
//LessThanComparable

//CopyConstructible

template <typename, template <typename> class>
class persistent_set;

template <typename T, template<typename> class Memory_manager_ptr = std::shared_ptr>
class persistent_set {
private: // nested structs
    struct Node;
    typedef Memory_manager_ptr<Node> Pointer;
    struct Node {
    public: // members
        Pointer left, right;
    public: // method
        Node(Pointer const &left, Pointer const &right) noexcept
            : left(left), right(right) { }

        Node() noexcept
            : Node(nullptr, nullptr) { }

        Node(Node const& other) noexcept
            : Node(other.left, other.right) { }

        virtual Node* copy() const {
            return new Node(*this);
        }

        virtual bool is_end() const noexcept {
            return true;
        }

        virtual ~Node() {
#ifdef TRACK_DESTRUCTOR
            std::cout << "Node del: end" << std::endl;
#endif
        }

        virtual T const& get_data() const {
            assert(false && "you tried get data from node_base");
        }
    };

    struct Node_with_data: public Node {
    public: // members
        T data;
    public: //method
        Node_with_data(T const& data, Pointer const &left, Pointer const &right)
            noexcept(noexcept(T(data)))
            : Node(left, right), data(data) { }

        Node_with_data(T const& data) noexcept(noexcept(T(data)))
            : Node(), data(data) {}

        Node_with_data(Node_with_data const &other) noexcept(noexcept(T(data)))
            : Node(other), data(other.data) { }

        virtual Node* copy() const override {
            return new Node_with_data(*this);
        }

        virtual bool is_end() const noexcept override {
            return false;
        }

        ~Node_with_data() override {
#ifdef TRACK_DESTRUCTOR
            std::cout << "Node del: " << data << " and base: ";
#endif
        }

        T const& get_data() const override {
            return data;
        }
    };

   static void swap_nodes(Node* a, Node *b) {
       assert(!(a->is_end() || b->is_end()));
       std::swap(static_cast<Node_with_data*>(a)->data, static_cast<Node_with_data*>(b)->data);
   }

public: // nested structs

    struct my_iterator {
        friend class persistent_set;

    private: // members of iterator
        Node *ptr_node, *root;
    public: // method of iterator
        my_iterator() noexcept : ptr_node(nullptr), root(nullptr) { }

        my_iterator(my_iterator const &other) noexcept :
            ptr_node(other.ptr_node), root(other.root) { }

        my_iterator& operator=(my_iterator const &other) noexcept {
            ptr_node = other.ptr_node;
            root = other.root;
            return *this;
        }

        ~my_iterator() = default;

        T operator*() const noexcept {
            assert(!ptr_node->is_end());
            return ptr_node->get_data();
        }

        my_iterator& operator++() noexcept {
            ptr_node = next_node(ptr_node, root);
            return *this;
        }
        my_iterator operator++(int) noexcept {
            my_iterator temp = (*this);
            ++(*this);
            return temp;
        }

        my_iterator& operator--() noexcept {
            ptr_node = prev_node(ptr_node, root);
            return *this;
        }
        my_iterator operator--(int) noexcept {
            my_iterator temp = (*this);
            --(*this);
            return temp;
        }

        friend bool operator==(my_iterator const &a, my_iterator const &b) noexcept {
            return  a.ptr_node == b.ptr_node;
        }

        friend bool operator!=(my_iterator const &a, my_iterator const &b) noexcept {
            return a.ptr_node != b.ptr_node;
        }

        friend void swap(my_iterator &a, my_iterator &b) noexcept {
            std::swap(a.ptr_node, b.ptr_node);
            std::swap(a.root, b.root);
        }

    private: // method of iterator
        my_iterator(Node *v, Node *root_v) noexcept
            : ptr_node(v), root(root_v) { }
    };
    typedef my_iterator iterator;
private: // members of perset
    Pointer root;
    Node* end_node;
    size_t _size;
public: // method of perset
    persistent_set() noexcept : _size(0) {
        root = Pointer(end_node = new Node());
    }

    persistent_set(persistent_set const &other) noexcept
        : root(other.root), end_node(other.end_node), _size(other._size) { }

    persistent_set& operator=(persistent_set const& other) noexcept {
        root = other.root;
        end_node = other.end_node;
        _size = other._size;
        return *this;
    }

    persistent_set(persistent_set &&other)
        : root(std::move(other.root)), end_node(std::move(other.end_node)), _size(std::move(other._size)) { }

    persistent_set& operator=(persistent_set &&other) {
        root = std::move(other.root);
        end_node = std::move(other.end_node);
        _size = std::move(_size);
        return *this;
    }

    ~persistent_set() = default;

    iterator find(T const &element) const noexcept {
        Node* prediction = find_impl(root.get(), element);
        if (comp(prediction, element) == 0)
            return iterator(prediction, root.get());
        else return iterator(end_node, root.get());
    }

/*--------------------------------
* if insert return true, than version this will change.
* Old version will delete, if you didn't copy before insert
* -------------------------------*/
    std::pair<iterator, bool> insert(T const& element) {
        Node* prediction = find_impl(root.get(), element);
        if (comp(prediction, element) == 0)
            return std::make_pair(iterator(prediction, root.get()), false);
        else {
            Node* last;
            Pointer new_root(copy_path(root.get(), prediction, last));
            assert(comp(last, prediction) == 0);
            Node *new_node = new Node_with_data(element);
            if (comp(last, element) == -1)
                last->right = Pointer(new_node);
            else
                last->left = Pointer(new_node);
            root = new_root;
            end_node = max_node(root.get());
            ++_size;
            return std::make_pair(iterator(new_node, root.get()), true);
        }
    }

    void erase(iterator const& iter) {
        assert(iter.ptr_node != end_node);
        Node* target_node = iter.ptr_node; // this node must die
        Node* copy_target_node = nullptr; // it's copy in new branch
        Node* new_root = nullptr; // it's root new branch
        Node* instead = nullptr; // node which swap with target
        Node* copy_instead = nullptr; // instead in new branch
        Node* copy_target_node_left; // it's left target's son in new branch    | start of second part path
        Node* copy_target_node_right; // it's right target's son in new branch  |
        Node* last_right_parent = nullptr; //|
        Node* last_left_parent = nullptr; // | -> for find_rot
        Node* last_parent = nullptr; //      |
        Node* last_node = nullptr; // for copy_path
        if(target_node->left) {
            new_root = copy_path(root.get(), target_node, copy_target_node);
            instead = max_node(target_node->left.get());
            copy_target_node_left = copy_path(target_node->left.get(), instead, copy_instead);
            copy_target_node->left = Pointer(copy_target_node_left);
            find_rot(copy_target_node, copy_instead, last_right_parent, last_left_parent, last_parent);
            swap_nodes(copy_target_node, copy_instead);
            if (last_left_parent != nullptr) {
                assert(last_left_parent->right.get() == copy_instead);
                last_left_parent->right = last_left_parent->right->left;
            } else {
                assert(copy_target_node->left.get() == copy_instead);
                copy_target_node->left = copy_target_node->left->left;
            }
        } else if(target_node->right) {
            new_root = copy_path(root.get(), target_node, copy_target_node);
            instead = min_node(target_node->right.get());
            copy_target_node_right = copy_path(target_node->right.get(), instead, copy_instead);
            copy_target_node->right = Pointer(copy_target_node_right);
            find_rot(copy_target_node, copy_instead, last_right_parent, last_left_parent, last_parent);
            swap_nodes(copy_instead, copy_target_node);
            if (last_right_parent != nullptr) {
                assert(last_right_parent->left.get() == copy_instead);
                last_right_parent->left = last_right_parent->left->right;
            } else {
                assert(copy_target_node->right.get() == copy_instead);
                copy_target_node->right = copy_target_node->right->right;
            }
        } else {
            find_rot(root.get(), target_node, last_right_parent, last_left_parent, last_parent);
            assert(last_parent != nullptr);
            new_root = copy_path(root.get(), last_parent, last_node);
            if (last_parent == last_left_parent) {
                last_node->right = nullptr;
            } else {
                last_node->left = nullptr;
            }
        }
        root = Pointer(new_root);
        end_node = max_node(new_root);
        --_size;
    }

    bool empty() const noexcept {
        assert(root.get() == end_node);
        return _size == 0;
    }

    size_t size() const noexcept {
#ifdef HONEST_SIZE
        return honest_size(root.get()) - 1;
#else
        return _size;
#endif
    }

    iterator begin() const noexcept {
        return iterator(min_node(root.get()), root.get());
    }

    iterator end() const noexcept {
        return iterator(end_node, root.get());
    }

    friend bool operator==(persistent_set const& a, persistent_set const& b) noexcept {
        return a.root == b.root;
    }

    friend bool operator!=(persistent_set const& a, persistent_set const& b) noexcept {
        return a.root != b.root;
    }


    friend void swap(persistent_set &a, persistent_set &b) noexcept {
       Pointer::swap(a.root, b.root);
       Pointer::swap(a.end_node, b.end_node);
    }

#ifdef MY_DEBUG
     std::string to_string() const {
         std::string message = " ";
         message += "Current tree: ";
         to_string_rec(root.get(), message);
         message += "\n";
         return message;
     }
#endif

private: // method of perset

/* comparator
* -1 v.data < key (key is right)
* 0 v.data = key
* +1 v.data > key (key is left)
*/
    static int32_t comp(Node const *v, T const& key) noexcept {
        if (v->is_end())
            return 1;
        else if (v->get_data() == key)
            return 0;
        else return (v->get_data() < key ? -1 : 1);
    }
    static int32_t comp(Node const *a, Node const *b) noexcept { // a < b = true
        if (b->is_end())
            return (a->is_end()? 0: -1);
        else
            return comp(a, b->get_data());
    }

#ifdef MY_DEBUG
     void to_string_rec(Node *v, std::string &message) const {
         if (v != nullptr) {
             if (v->is_end())
                 message += "end ";
             else message += std::to_string(v->get_data()) + " ";
             to_string_rec(v->left.get(), message);
             to_string_rec(v->right.get(), message);
         }
         return;
     }

     size_t honest_size(Node *v) const noexcept {
         if (v) {
             return honest_size(v->left.get()) +
                    honest_size(v->right.get()) + 1;
         } else
             return 0;
     }
#endif

    Node* find_impl(Node *v, T const& key) const noexcept {
        switch(comp(v, key)) {
        case(1): return (v->left? find_impl(v->left.get(), key) : v);
        case(0): return v;
        case(-1): return (v->right? find_impl(v->right.get(), key) : v);
        }
        assert(true && "fail in find_impl");
    }

/* return pointer on new_root
* and pointer on last element of path -> last
* pre: path must exist in tree */
    Node* copy_path(Node *v, Node const *target, Node *(&last)) {
        Node* v_copy = v->copy();
        switch(comp(v, target)) {
        case(0):
            last = v_copy;
            break;
        case(1):
            assert(v->left);
            v_copy->left = Pointer(copy_path(v->left.get(), target, last));
            break;
        case(-1):
            assert(v->right);
            v_copy->right = Pointer(copy_path(v->right.get(), target, last));
            break;
        }
        return v_copy;
    }

    static Node* min_node(Node *v) noexcept {
        if (v->left)
            return min_node(v->left.get());
        else return v;
    }

    static Node* max_node(Node *v) noexcept {
        if (v->right)
            return max_node(v->right.get());
        else return v;
    }

//pre: path must exist
    static void find_rot(Node *v, Node *target,
                         Node *(&last_right_parent), Node *(&last_left_parent), Node *(&last_parent)) noexcept {
        switch(comp(v, target)) {
        case(0):
            return;
        case(1):
            assert(v->left);
            last_right_parent = v;
            last_parent = v;
            find_rot(v->left.get(), target, last_right_parent, last_left_parent, last_parent);
            break;
        case(-1):
            assert(v->right);
            last_left_parent = v;
            last_parent = v;
            find_rot(v->right.get(), target, last_right_parent, last_left_parent, last_parent);
            break;
        }
        return;
    }

    static Node* next_node(Node *v, Node* root) noexcept {
        assert(v != nullptr);
        assert(root != nullptr);
        if (v->right) {
            return min_node(v->right.get());
        } else {
            Node *last_left_parent = nullptr, *last_right_parent = nullptr, *last_parent = nullptr;
            find_rot(root, v, last_right_parent, last_left_parent, last_parent);
            assert(last_right_parent != nullptr);
            return last_right_parent;
        }
    }

    static Node* prev_node(Node *v, Node* root) noexcept {
        assert(v != nullptr);
        assert(root != nullptr);
        if (v->left) {
            return max_node(v->left.get());
        } else {
            Node *last_left_parent, *last_right_parent, *last_parent = nullptr;
            find_rot(root, v, last_right_parent, last_left_parent, last_parent);
            assert(last_left_parent != nullptr);
            return last_left_parent;
        }
    }
};

#endif //PERSISTENT_SET_HEADER
/*
* 1) test of time-life other element after operation. Because when you copy path your might lost root's control, and all old node'll delete.
* 2) test of copy
*/
