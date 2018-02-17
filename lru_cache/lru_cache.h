#ifndef LRU_CACHE_H
#define LRU_CACHE_H
#include <iostream>
#include <cassert>

template <typename A, typename B> class cache {
    typedef A key_type;
    typedef B mapped_type;
    typedef std::pair<key_type, mapped_type> value_type;
public:
    size_t count_node, max_count_node;
    struct iterator;
private:
    struct node;
    node *root_node, *begin_que_node, *end_node;
public:
    cache(size_t max_count_node): count_node(0), max_count_node(max_count_node),
        root_node(nullptr), begin_que_node(nullptr), end_node(nullptr){
        begin_que_node = end_node = new node();
    }

    cache(cache const& other) = delete;

    cache const& operator=(cache const& other) = delete;

    ~cache() = default;

    iterator find(key_type);

    std::pair<iterator, bool> insert(value_type);

    void erase(iterator);

    iterator begin() const {
//        return iterator(begin_node);
    }

    iterator end() const {
        return iterator(end_node);
    }

private:    // <set> //
    node* set_insert(node*) {

    }

    node* set_delete(node*);

    static node* set_next(node const*) {}

    static node* set_prev(node const*) {}

    // </set> //
    // <que> //
    node* que_back() {
        assert(end_node->next != nullptr);
        return end_node->next;
    }

    void que_delete(node* a) {
        a->next->prev = a->prev;
        if (a->prev != nullptr) {
            a->prev->next = a->next;
        }
    }

    void que_push_front(node* new_node) {
        new_node->next = begin_que_node;
        begin_que_node->prev = new_node;
        return;
    }
    // </que> //
};

template<typename A, typename B>
struct cache<A, B>::node {
public:
   key_type key;
   mapped_type mapped;

   node* left;
   node* right;
   node* parent;

   node* next;
   node* prev;

   node() = default;

   node(key_type key, mapped_type mapped):key(key), mapped(mapped),
       left(nullptr), right(nullptr), parent(nullptr), next(nullptr), prev(nullptr){}

   node(value_type value): node(value.first, value.second) { }

   static node* set_next(node const*) {}

   static node* set_prev(node const*) {}
};

template <typename A, typename B>
struct cache<A, B>::iterator {
private:
    node* inside;
public:
    iterator() = default;

    iterator(node *inside): inside(inside) {}

    iterator(iterator const &other) = default;

    iterator const&  operator=(iterator const &other) {
        inside = other.inside;
        return *this;
    }

    value_type const operator*() const {
        return std::make_pair(inside->key, inside->mapped);
    }

    iterator& operator++() {
        inside = node::set_next(inside);
        return *this;
    }
    iterator operator++(int) {
        iterator t(*this);
        ++(*this);
        return t;
    }

    iterator& operator--() {
        inside = node::set_prev(inside);
        return *this;
    }
    iterator operator--(int) {
        iterator t(*this);
        ++(*this);
        return t;
    }

    template<A, B> friend bool operator==(cache<A, B>::iterator a, cache<A, B>::iterator b);
    template<A, B> friend bool operator!=(cache<A, B>::iterator a, cache<A, B>::iterator b);
};

template <typename A, typename B> bool operator==(typename cache<A, B>::iterator a, typename cache<A, B>::iterator b) {
    return a.inside == b.inside;
}

template <typename A, typename B> bool operator!=(typename cache<A, B>::iterator a, typename cache<A, B>::iterator b) {
    return a.inside != b.inside;
}

#endif // LRU_CACHE_H
