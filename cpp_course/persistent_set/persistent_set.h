#ifndef PERSISTENT_SET_HEADER
#define PERSISTENT_SET_HEADER
#include <memory>
#include <cassert>
#include "smart_pointers/linked_ptr.h"
#include "smart_pointers/shared_ptr.h"

//Requirements for T:
//DefaultConstructible => not necessary
//Destructible
//LessThanComparable

//CopyConstructible

template <typename, template <typename> class>
class persistent_set;

template <typename T, template<typename> class Memory_manager_ptr = std::shared_ptr>
class persistent_set {
private: // struct
    struct Node_base;
    typedef Memory_manager_ptr<Node_base> Pointer;
    struct Node_base {
    public: // members
        Pointer left, right;
        Node_base *parent;
    public: // method
        Node_base(Pointer const &left, Pointer const &right, Node_base* parent):
            left(left), right(right), parent(parent) { }

        Node_base():
            Node_base(nullptr, nullptr, nullptr) { }

        Node_base(Node_base const& other):
            Node_base(other.left, other.right, other.parent) { }

        void set_field(Pointer left_sptr, Pointer right_sptr, Node_base *parent_ptr) {
            left = left_sptr;
            right = right_sptr;
            parent = parent_ptr;
        }

        virtual Node_base* copy() {
            return new Node_base(*this);
        }
        virtual ~Node_base() = default;

        virtual T const& get_data() {
            assert(false && "you tried get data from node_base");
        }
    };

    struct Node: public Node_base {
    public: // members
        T data;
    public: //method
        Node(T const& data, Pointer const &left, Pointer const &right, Node_base *parent):
             Node_base(left, right, parent), data(data) { }

        Node(T const& data):
            Node_base(), data(data) {}

        Node(Node const &other):
            Node_base(other), data(other.data) { }

        virtual Node_base* copy() override {
            return new Node(*this);
        }

        ~Node() override = default;

        T const& get_data() override {
            return data;
        }
    };

public: // struct

    struct my_iterator {
        friend class persistent_set;

    private: // members of iterator
        Node_base *ptr_node;
    public: // method of iterator
        my_iterator():
            ptr_node(nullptr) { }

        my_iterator(my_iterator const &other):
            ptr_node(other.ptr_node) { }

        my_iterator(Node_base *v):
            ptr_node(v) { }

        my_iterator& operator=(my_iterator const &other) {
            ptr_node = other.ptr_node;
            return *this;
        }

        ~my_iterator() = default;

        T operator*() const {
            assert(!is_end(ptr_node));
            return ptr_node->get_data();
        }

        my_iterator& operator++() {
            ptr_node = next_node(ptr_node);
            return *this;
        }
        my_iterator operator++(int) {
            my_iterator temp = (*this);
            ++(*this);
            return temp;
        }

        my_iterator& operator--() {
            ptr_node = prev_node(ptr_node);
            return *this;
        }
        my_iterator operator--(int) {
            my_iterator temp = (*this);
            ++(*this);
            return temp;
        }

        friend bool operator==(my_iterator const &a, my_iterator const &b) {
            return  a.ptr_node == a.ptr_node;
        }

        friend bool operator!=(my_iterator const &a, my_iterator const &b) {
            return a.ptr_node != a.ptr_node;
        }

        // typedef std::ptrdiff_t difference_type;
        // typedef T value_type;
        // typedef T* pointer;
        // typedef T& reference;
        // typedef std::bidirectional_iterator_tag iterator_category;
    };
    typedef my_iterator iterator;
    //typedef my_iterator const_iterator;
    // typedef std::reverse_iterator<iterator> reverse_iterator;
    // typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
private: // members of perset
    Pointer root, end_node;
    size_t _size;
public: // method of perset
    persistent_set(): _size(0) {
        end_node = Pointer(new Node_base());
        root = end_node;
    }

    persistent_set(persistent_set const &other):
        root(other.root), end_node(other.end_node), _size(other._size) { }

    persistent_set& operator=(persistent_set const& other) {
        root = other.root;
        end_node = other.end_node;
        _size = other._size;
        return *this;
    }

    ~persistent_set() = default;

    iterator find(T const &element) {
        Node_base *v = find_Impl(root.get(), element);
        if (!is_end(v) && v->get_data() == element)
            return iterator(v);
        else return iterator(end_node.get());
    }

/*--------------------------------
* if insert return true, than version this will change.
* Old version will delete, if you didn't copy before insert
* -------------------------------*/
    std::pair<iterator, bool> insert(T const& element) {
        Node_base *v = find_Impl(root.get(), element);
        if ( !is_end(v) && v->get_data() == element) {
            return std::make_pair(iterator(v), false);
        } else {
            // insert and change root and end
            v = insert_Impl(v, element);
            ++_size;
            return std::make_pair(iterator(v), true);
        }
    }

    void erase(iterator const& iter) {
        assert(iter.ptr_node);
        assert(is_end(iter.ptr_node));
        erase_Impl(iter.ptr_node);
        --_size;
    }

    bool empty() {
        assert(root == end_node);
        return _size == 0;
    }

    size_t size() {
        return _size;
    }

    iterator begin() const {
        Node_base *v = root.get();
        while (v->left) {
            v = v->left.get();
        }
        return iterator(v);
    }

    iterator end() const {
        return iterator(end_node.get());
    }

    friend bool operator==(persistent_set const& a, persistent_set const& b) {
        return a.root == b.root;
    }

    friend bool operator!=(persistent_set const& a, persistent_set const& b) {
        return a.root != b.root;
    }

private: // method of perset
    // copy path
    // v => node old branch, which first copy
    // ptr => node from new branch
    static bool less_keys(Node_base *a, Node_base *b) { // a < b = true
        assert(!is_end(a) || !is_end(b));
        if (is_end(a)) // end is the biggest key
            return false;
        else if (is_end(b))
            return true;
        else return a->get_data() < b->get_data();
    }

    static bool less_keys(Node_base *v, T const& key) { // a < b = true
        if(is_end(v))
            return false;
        else return v->get_data() < key;
    }

    void copy_path_to_root(Node_base *v, Pointer ptr, bool ptr_left) {
        Node_base *new_v;
        while(v != nullptr) {
            new_v = v->copy();
            if (ptr_left)
                new_v->left = ptr;
            else new_v->right = ptr;
            ptr->parent = new_v;
            ptr_left = (v->parent && v->parent->left && v->parent->left.get() == v);
            ptr = Pointer(new_v);
            if (is_end(v))
                end_node = ptr;
            v = v->parent;
        }
        root = ptr;
        return;
    }

    Node_base* find_Impl(Node_base *v, T const& key) {
        if (less_keys(v, key)) {
            if (v->right)
                return find_Impl(v->right.get(), key);
            else return v;
        } else if (!less_keys(v, key)) {
            if (v->left)
                return find_Impl(v->left.get(), key);
            else return v;
        } else return v;
    }

    void erase_Impl(Node_base *deletable) {
        Pointer deletable_Pointer(deletable); // for capture
        // capture instead node, if we miss this pointer, then node'll delete.
        Pointer instead_Pointer;// Pointer on new node, which replace deletable
        bool instead_from_left;
        if (deletable->left) {
            instead_Pointer = Pointer(deletable->left->copy());
            instead_from_left = true;
        } else if (deletable->right) {
            instead_Pointer = Pointer(deletable->right->copy());
            instead_from_left = false;
        } else {
            instead_Pointer = nullptr;
        }
        copy_path_to_root( deletable->parent,
                           instead_Pointer,
        /*for deleteble*/ (deletable->parent == nullptr || deletable->parent->left == deletable_Pointer ? true : false ));

        /// path to root is good; fix path to listerase_Impl

        Pointer cur_deletable = deletable_Pointer, new_instead_Pointer;
        bool new_instead_from_left;
        while (instead_Pointer) {
            // begin init instead node
            if (instead_from_left) {
                instead_Pointer->set_field(Pointer(nullptr), cur_deletable->right, cur_deletable->parent);
                cur_deletable = cur_deletable->left;
            } else {
                instead_Pointer->set_field(cur_deletable->left, Pointer(nullptr), cur_deletable->parent);
                cur_deletable = cur_deletable->right;
            }
            assert(!cur_deletable);
            // choose new instead
            if (cur_deletable->left) {
                new_instead_Pointer = Pointer(cur_deletable->left->copy());
                new_instead_from_left = true;
            } else if (cur_deletable->right) {
                new_instead_Pointer = Pointer(cur_deletable->right->copy());
                new_instead_from_left = false;
            } else {
                new_instead_Pointer = Pointer(nullptr);
            }
            // end init instead_Pointer (connect instead -> new_instead)
            (instead_from_left ? instead_Pointer->left : instead_Pointer->right) = new_instead_Pointer;
            // transition to new state
            instead_Pointer = new_instead_Pointer;
            instead_from_left = new_instead_from_left;
        }
    }

    Node_base* insert_Impl(Node_base* v, T const& data) {
        // change version subtree, so change root end_node
        Node_base *ret = new Node(data);
        Pointer ptr = Pointer(ret);
        copy_path_to_root(v, ptr, less_keys(ptr.get(), v));
        return ret;
    }

    static Node_base* next_node(Node_base *v) {
        assert(v != nullptr);
        Node_base *it = v;
        if (it->right) {
            it = it->right.get();
            while (it->left) {
                it = it->left.get();
            }
            return it;
        } else {
            while (it->parent && (it->parent->right.get() == it) ) {
                it = it->parent;
            }
            return it->parent;
        }
    }

    static Node_base* prev_node(Node_base *v) {
        assert(v != nullptr);
        Node_base *it = v;
        if (it->left) {
            while (it->right) {
                it = it->right.get();
            }
            return it;
        } else {
            while ( it->parent && (it->parent->left.get() == it) ) {
                it = it->parent;
            }
            return it->parent;
        }
    }

    static bool is_end(Node_base *node) {
        return dynamic_cast<Node*>(node) == nullptr;
    }
};

#endif //PERSISTENT_SET_HEADER
/*
* 1) test of time-life other element after operation. Because when you copy path your might lost root's control, and all old node'll delete.
* 2) test of copy
* NB) May be, you should make delay deleted old root
*/


///TODO add swap()
///TODO add rvalue
