// It's stable and major version.
#ifndef LIST_DEBUG_H
#define LIST_DEBUG_H
#include <vector>
#include <cassert>

template <typename T>
class list_debug {

private:
    class any_iterator {
    public:
        virtual void invalid() = 0;
        virtual void update_owner(list_debug<T> const*) = 0;
        virtual any_iterator *& get_next() = 0;
    };
    template <typename CT> class my_iterator;
    class node_base {
    public:
        node_base *next, *prev;
        any_iterator * begin_iter, *end_iter;

        node_base(): next(nullptr), prev(nullptr), begin_iter(nullptr), end_iter(nullptr) {}

        static void connect(node_base *a, node_base *b) { // a <=> b b.next = b;
            a->prev = b;
            b->next = a;
        }

        void push_back (any_iterator* iter) {
            if (begin_iter == nullptr) {
                begin_iter = end_iter = iter;
            } else {
                end_iter->get_next() = iter;
                end_iter = iter;
            }
        }

        void sub_iter(any_iterator* iter) {
            if (iter  == begin_iter) {
                begin_iter = begin_iter->get_next();
            } else {
                any_iterator* i_prev = begin_iter;
                any_iterator* i = begin_iter->get_next();
                while(i != nullptr) {
                    if (i == iter) {
                        if (end_iter == i) {
                            end_iter = i_prev;
                        }
                        i_prev->get_next() = i->get_next();
                        break;
                    }
                    i_prev = i;
                    i = i->get_next();
                }
            }
            iter->get_next() = nullptr;
        }

        void update_owner_iters(list_debug<T> const* new_owner) {
            any_iterator *i = begin_iter;
            while (i != nullptr) {
                i->update_owner(new_owner);
                i = i->get_next();
            }
        }

        void invalid_all () {
            any_iterator* i = begin_iter;
            while (i != nullptr) {
                i->invalid();
                i = i->get_next();
            }
        }

        virtual ~node_base() {
            invalid_all();
        }
    };

    class node: public node_base {
    public:
        T data;

        node() = delete;
        node(T data): node_base(), data(data) { }

        ~node() override { }
    };
    node_base *begin_node, *end_node;//fake node

    T const& get_data(node_base *cur) const {
        return dynamic_cast<node *>(cur)->data;
    }

    T &get_data(node_base *cur){
        return dynamic_cast<node *>(cur)->data;
    }

public:
    typedef my_iterator<T> iterator;
    typedef my_iterator<const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    list_debug():begin_node(), end_node() {
        begin_node = new node_base();
        try {
            end_node = new node_base();
        } catch (...) {
            delete begin_node;
        }
        node_base::connect(begin_node, end_node);
    }
    list_debug(list_debug const& other):list_debug() {
        for (node_base *i = other.begin_node->prev; i != other.end_node; i = i->prev) {
            push_back(dynamic_cast<node *>(i)->data);
        }
    }

    void clear() {
        while(!empty()) {
            pop_back();
        }
    }

    list_debug& operator=(list_debug const& other) {
        list_debug temp;
        for (node_base *i = other.begin_node->prev; i != other.end_node; i = i->prev) {
            temp.push_back(dynamic_cast<node*>(i)->data);
        }
        clear();
        for (node_base *i = temp.begin_node->prev; i != temp.end_node; i = i->prev) {
            push_back(dynamic_cast<node*>(i)->data);
        }
        return *this;
    }

    iterator begin() {
        return iterator(begin_node->prev, this);
    }
    const_iterator begin() const {
        return const_iterator(begin_node->prev, this);
    }

    iterator end() {
        return iterator(end_node, this);
    }
    const_iterator end() const {
        return const_iterator(end_node, this);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }


    bool empty() const{
        return begin_node->prev == end_node;
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
    void pop_front() {
        assert(!empty());
        node_base *_node = begin_node->prev;
        node_base::connect(begin_node, _node->prev);
        delete _node;
    }
    void insert(const_iterator iter, T const& data) { // iterator for prev
        assert(this == iter.owner);
        node *_node = new node(data);
        node_base::connect(iter._node->next, _node);
        node_base::connect(_node, iter._node);
    }

    iterator erase(const_iterator const &iter) {
        assert(this == iter.owner);
        node_base *temp(iter._node->prev);
        node_base::connect(iter._node->next, iter._node->prev);
        delete iter._node;
        return iterator(temp, this);
    }

    void splice(const_iterator pos, list_debug& other, const_iterator first, const_iterator last) {
        assert(pos.owner == this);
        assert(first.owner == &other);
        assert(last.owner == &other);
        bool check_first_to_last = false;
        const_iterator it = first;
        for(; it != other.end(); ++it) {
            if (it == last) {
                check_first_to_last = true;
                break;
            }
        }
        if (it == other.end()) {
            check_first_to_last = true;
        }
        assert(check_first_to_last);
        bool pos_between_first_last = false;
        for(const_iterator it = first; it != last; ++it) {
            if (it == pos) {
                pos_between_first_last = true;
                break;
            }
        }
        assert(!pos_between_first_last);
        if (first == last)
            return;

        for(const_iterator it = first; it != last; ++it) {
            it._node->update_owner_iters(this);
        }

        node_base* begin_path = first._node;
        node_base* end_path = last._node->next;
        node_base::connect(begin_path->next, end_path->prev);

        node_base::connect(pos._node->next, begin_path);
        node_base::connect(end_path, pos._node);

    }

    friend void swap(list_debug & a, list_debug & b) {
        for(typename list_debug<T>::node_base *i = a.begin_node->prev; i != a.end_node; i = i->prev) {
            i->update_owner_iters(&b);
        }
        for(typename list_debug<T>::node_base *i = b.begin_node->prev; i != b.end_node; i = i->prev) {
            i->update_owner_iters(&a);
        }
        std::swap(a.begin_node, b.begin_node);
        std::swap(a.end_node, b.end_node);
    }

    ~list_debug() {
        clear();
        delete begin_node;
        delete end_node;
    }
};
template <typename T>
template <typename CT>
class list_debug<T>::my_iterator: public list_debug<T>::any_iterator {
private:
    friend list_debug<T>::node_base;
    friend list_debug<T>;
    node_base *_node;
    bool is_invalid;
    list_debug<T> const* owner;
    typename list_debug<T>::any_iterator *next;

    void invalid() override {
        assert(is_invalid == false);
        is_invalid = true;
    }

   typename list_debug<T>::any_iterator*& get_next() override {
         return next;
   }

    void update_owner(list_debug<T> const* new_owner)  override {
        owner = new_owner;
    }

public:
    my_iterator(): _node(nullptr), is_invalid(true), next(nullptr) {}

    my_iterator(my_iterator const &other):
        _node(other._node), is_invalid(other.is_invalid), owner(other.owner), next(nullptr) {
        assert(other.is_invalid == false);
        _node->push_back(this);
    }

    template <typename OTHER_TYPE>
    my_iterator(const my_iterator<OTHER_TYPE> &other,
                typename std::enable_if<std::is_same<typename std::remove_const<CT>::type, OTHER_TYPE>::value>::type * = nullptr)
        : _node(other._node), is_invalid(other.is_invalid), owner(other.owner), next(nullptr) {
        assert(other.is_invalid == false);
        _node->push_back(this);
    }

    my_iterator(node_base *_node, list_debug<T> const*_owner)
        : _node(_node), is_invalid(false) {
        owner = _owner;
        next = nullptr;
        _node->push_back(this);
    }

    my_iterator &operator=(my_iterator const& other) {
        assert(other.is_invalid == false);
        if (is_invalid == false) {
            _node->sub_iter(this);
        }
        _node = other._node;
        is_invalid = other.is_invalid;
        owner = other.owner;
        _node->push_back(this);
        return *this;
    }

    CT& operator*() const{
        assert(is_invalid == false);
        assert(_node != owner->end_node);
        assert(_node != owner->begin_node);
        return dynamic_cast<node*>(_node)->data;
    }

    my_iterator& operator++() {
        assert(is_invalid == false);
        assert(_node != owner->end_node);
        _node->sub_iter(this);
        _node = _node->prev;
        _node->push_back(this);
        return *this;
    }
    my_iterator operator++(int) {
        assert(is_invalid == false);
        assert(_node != owner->end_node);
        my_iterator temp(*this);
        ++(*this);
        return temp;
    }

    my_iterator& operator--() {
        assert(is_invalid == false);
        assert(_node != owner->begin_node->prev);
        _node->sub_iter(this);
        _node = _node->next;
        _node->push_back(this);
        return *this;
    }

    my_iterator operator--(int) {
        assert(is_invalid == false);
        assert(_node != owner->begin_node->prev);
        my_iterator temp(*this);
        --(*this);
        return temp;
    }

    ~my_iterator() {
        if (is_invalid == false) {
            _node->sub_iter(this);
        }
    }

    friend bool operator==(my_iterator const &a, my_iterator const &b)  {
        assert(a.is_invalid == false);
        assert(b.is_invalid == false);
        return a._node == b._node;
    }

    friend bool operator!=(my_iterator const &a, my_iterator const &b) {
        assert(a.is_invalid == false);
        assert(b.is_invalid == false);
        return a._node != b._node;
    }

    friend void swap(my_iterator &a, my_iterator &b) {
        assert(a.owner == b.owner);
        assert(a.is_invalid == false);
        assert(b.is_invalid == false);
        a._node->sub_iter(&a);
        b._node->sub_iter(&b);
        std::swap(a._node, b._node);
        a._node->push_back(&a);
        b._node->push_back(&b);
    }

    typedef std::ptrdiff_t difference_type;
    typedef CT value_type;
    typedef CT* pointer;
    typedef CT& reference;
    typedef std::bidirectional_iterator_tag iterator_category;

private:

};

#endif // LIST_DEBUG_H
