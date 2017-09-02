#ifndef LIST_DEBUG_H
#define LIST_DEBUG_H
#include <vector>
#include <cassert>

template <typename T>
class list {

private:
    template <typename CT> class my_iterator;
    typedef my_iterator<T> iterator;
    typedef my_iterator<const T> const_iterator;
    class node_base {
    public:
        node_base *next, *prev;
        std::vector<const_iterator *> vec_iters;

        node_base(): next(nullptr), prev(nullptr), vec_iters() {}

        static void connect(node_base *a, node_base *b) { // a <=> b b.next = b;
            a->prev = b;
            b->next = a;
        }

        void add_iter(const_iterator* iter) {
            vec_iters.push_back(iter);
        }
        void invalid_all() {
            for (size_t i = 0; i < vec_iters.size(); i++) {
                vec_iters[i]->invalid();
            }
        }
        void sub_iter(const_iterator* iter) {
            for(size_t i = 0; i < vec_iters.size(); i++) {
                if (vec_iters[i] == iter) {
                    vec_iters.erase(vec_iters.begin() + i);
                    return;
                }
            }
            assert(true);
        }

        virtual ~node_base() {
            node_base::invalid_all();
        }
    };

    class node: public node_base {
    public:
        T data;

        node() = delete;
        node(T data): node_base(), data(data) { }

        ~node() {
            node_base::invalid_all();
        }
    };
    node_base *begin_node, *end_node;//fake node

    T const& get_data(node_base *cur) const {
        return dynamic_cast<node *>(cur)->data;
    }

    T &get_data(node_base *cur){
        return dynamic_cast<node *>(cur)->data;
    }

public:
    list():begin_node(), end_node() {
        begin_node = new node_base();
        end_node = new node_base();
        node_base::connect(begin_node, end_node);
    }
    list(list const&) {
 // need push_back
    }
    list& operator=(list const&);
    bool empty() const{
        return begin_node->next == end_node;
    }
    void push_back(T const &new_data){
        node* _node = new node(new_data);
        node_base::connect(end_node->next, _node);
        node_base::connect(_node, end_node);
    }
    void push_front(T const &new_data) {
        node* _node = new node(new_data);
        node_base::connect(_node, begin_node->prev);
        node_base::connect(begin_node, _node);
    }

    T& front() {
        assert(!empty());
        return get_data(begin_node->prev);
    }
    T& back() {
        assert(!empty());
        return get_data(end_node->next);
    }

    T const& front() const{
        assert(!empty());
        return get_data(begin_node->prev);
    }
    T const& back() const {
        assert(!empty());
        return get_data(end_node->next);
    }

    void pop_back(){
        assert(!empty());
        node_base *_node = end_node->next;
        node_base::connect(_node->next, end_node);
        delete _node;
    }

    void pop_front() { // check empty
        assert(!empty());
        node_base *_node = begin_node->prev;
        node_base::connect(begin_node, _node->prev);
        delete _node;
    }
    void insert(const_iterator iter, T const& data) { // iterator for prev
        node *_node = new node(data);
        node_base::connect(iter._node->next, _node);
        node_base::connect(_node, iter._node);
    }

    iterator erase(const iterator iter) {
//        node_base::connect(iter);
        /// need ++oper for iter
    }

    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);

    ~list() {
        delete begin_node;
        delete end_node;
    }
};
template <typename T>
template <typename CT>
class list<T>::my_iterator {
private:
    friend list<T>::node_base;
    friend list<T>;
    node *_node;
    bool is_invalid;
    void invalid() {
        assert(is_invalid == false);
        is_invalid = true;
    }
public:
    my_iterator() = delete;
    my_iterator(my_iterator const& other): _node(other._node), is_invalid(other.is_invalid) {
        _node->add_iter(this);
    }

    template <typename A>
    my_iterator(const my_iterator<A> &other, typename std::enable_if<std::is_convertible<CT, A>::value>::type *def = nullptr)


    ~my_iterator() {
        _node->sub_iter(this);
    }
private:

};


//template <typename T>
//struct list::iterator {
//    operator*(); // check end
//    // check for invalid (flag singular)
//};


#endif // LIST_DEBUG_H
