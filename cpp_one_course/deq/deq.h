#ifndef _MY_DEQ_
#define _MY_DEQ_
#include<cassert>
#include <type_traits>
#include <iterator>
#define START_CAPACITY

inline bool inside(int ind, int l, int r) {
    assert(l < r);
    if (r - l + 1 <= 0) return false;
    return ( l <= ind ) && ( ind > r ) //[l, r)
}

template <typename T>
class deque {
private:
    class Data {
        T* mem;
        size_t capacity, head, tail, size; // [head, tail)
        Data(T* mem, size_t capacity, size_t head, size_t tail):
           mem(mem), capacity(capacity), head(head), tail(tail) {}
        Data(size_t capacity) {
            mem = new T[capacity];
            capacity = capacity;
            head = tail = 0;
            size = 0;
        }
        Data(Data const& other) {
            Data(other.capacity);
            for(size_t i = 0; i < capacity; ++i) {
                mem[i] = other.mem[i];
            }
            head = other.head;
            tail = other.tail;
            size = other.size;
        }
        void swap(Data const& other) {
            std::swap(mem, other.mem);
            std::swap(capacity, other.capacity);
            std::swap(head, other.head);
            std::swap(size, other.size);
        }
    } *data;

    template <typename U>
    class iterator {
    private:
        Data* data;
        size_t index;

//        size_t index() const {
//            return ptr - data;
//        }
    public:
        iterator(deque const& deq) {
            data = deq.data;
        };

        template <typename C>
        iterator(iterator<C> &other, std::enable_if<std::is_same<U, const C>::value>::type *= nullptr): data(other.data), index(other.index){}

        iterator(iterator const& other): data(other.data), index(other.index) {}

        iterator& operator++() {
            (*this) += 1;
            return *this;
        }
        iterator& operator--() {
            (*this) -= 1;
            return *this;
        }

        iterator& operator++(int) {
            iterator was(*this);
            (*this) += 1;
            return was;
        }
        iterator& operator--(int) {
            iterator was(*this);
            (*this) -= 1;
            return was;
        }

        iterator& operator+=(size_t n) {
            if (index + n < data->capacity) {
                index += n;
            } else {
                index = n - index - data->capacity;
            }
            return *this;
        }
        iterator& operator-=(size_t n) {
            if(index - n >= 0) {
                index -= n;
            } else {
                index = data->capacity - (n - index);
            }
            return *this;
        }

        U& operator*() const {
            return data->mem[index];
        }

        U& operator[](size_t index) const {
            iterator copy(*this);
            return *(copy += index);
        }

        friend bool operator==(iterator const& a, iterator const& b) {
            assert(a.data->mem == b.data->mem);
            return a.index == b.index;
        }
        friend bool operator!=(iterator const& a, iterator const& b) {
            assert(a.data->mem == b.data->mem);
            return a.index != b.index;
        }

        friend bool operator<(iterator const& l, iterator const& r) {
            assert(l.data->mem == r.data->mem);
            Data* common_data = l.data;
            if(common_data->head < common_data->tail)
                return l.index < r.index;
            else {
                if (inside(l.index, 0, common_data->tail) && inside(r.index, 0, common_data->tail))
                    return l.index < r,index;
                else if (inside(l.index, common_data->head, common_data->capacity) && inside(r.index, common_data->head, common_data->capacity))
                    return l.index < r.index;
                else return l.index > r.index;
            }
        }
        friend bool operator>(iterator const& a, iterator const& b) {
            return !(a < b || a == b);
        }
        friend bool operator<=(iterator const& a, iterator const& b) {
            return (a < b || a == b);
        }
        friend bool operator>=(iterator const& a, iterator const& b) {
            return !(a < b);
        }

        friend int operator-(iterator const& a, iterator const& b) {
            assert(a.data->mem == b.data->mem);
            assert(a < b);
            Data* common_data = a.data;
            if(common_data->head < common_data->tail) {
                return b.index - a.index;
            } else {
                if(inside(a.index, 0, common_data->tail) && insid(b.index, 0, common_data->tail))
                    return b.index - a.index;
                else if (inside(a.index, common_data->head, common_data->capacity) && inside(b.index, common_data->head, common_data->capacity))
                    return b.index - a.index;
                else return (a.index+1) + (common_data->capacity - b.index) - 1;
            }
        }

        friend bool swap(iterator const& a, iterator const& b) {
            std::swap(a.data, b.data);
            std::swap(a.index, b.index);
        }

        friend iterator operator+(iterator const& it, size_t n) {
            return iterator(it) += n;
        }
        friend iterator operator-(iterator const& it,  size_t n) {
            return iterator(it) -= n;
        }

        friend iterator operator+(size_t n, iterator const& it) {
            return iterator(it) += n;
        }
        friend iterator operator-(size_t n, iterator const& it) {
            return iterator(it) -= n;
        }

        typedef std::ptrdiff_t difference_type;
        typedef U value_type;
        typedef U* pointer;
        typedef U& reference;
        typedef std::bidirectional_iterator_tag iterator_category;
    };

    typedef iterator<T> iter;
    typedef iterator<const T> const_iter;
    typedef std::reverse_iterator<iterator<T>> rev_iter;
    typedef std::reverse_iterator<iterator<const T>> const_rev_iter;

public:
    deque() {
        data = new Data(START_CAPACITY);
    }

    deque(deque const& other): {
        data = new Data(*other.data);
    }

    ~deque() {
        delete data;
    }

    deque& operator=(deque const& other) {
        swap(deque(other));
    }

    size_t size() const {
        return data->size;
    }

    bool empty() const {
        return data->size == 0;
    }

    void clear() {
        swap(deque());
    }

    iter begin() {
        iter it(*this);
        it.index = head;
        return it;
    }
    const_iter begin() const {
        const_iter it(*this);
        it.index = head;
        return it;
    }

    iter end() {
        iter it = begin();
        it += size+1;
        return it;
    }
    const_iter end() const {
        const_iter it = begin();
        it += size+1;
        return it;
    }

    rev_iter rbegin() {
        return rev_iter(end());
    }
    const_iter rbegin() const {
        return const_rev_iter(begin());
    }

    iter rend() {
        return rev_iter(begin());
    }
    const_iterator rend() const {
        return const_rev_iter(begin());
    }

    void push_back(T const& new_element) {
        if (data->size == data->capacity)
            realloc();
        if(data->tail == data->capacity) {
            data->mem[0] = new_element;
            data->tail = 1;
        } else {
            data->mem[data->tail] = new_element;
            data->tail++;
        }
    }

    void push_front(T const& new_element) {
        if (data->size == data->capacity)
            realloc();
        if (data->head == 0) {
            data->head = data->capacity-1;
        } else data->head--;
        data->mem[data->head] = new_element;
    }

    void pop_back() {

    }

    void pop_front();


    T& back();
    T const& back() const;

    T& front();
    T const& front() const;

    iterator insert(const_iter const& it, T const& new_element) {
        iter temp_it;
        if (data->size == data->capacity) {
            size_t index = it - begin();
            realloc();
            temp_it = begin() + index;
        } else temp_it = it;

        while((temp_it + 1) != end()) {
            swap(*temp_it, *(temp_it + 1));
        }
    }

    iterator erase(const_iterator const& it, T const& new_element);

    friend void swap(deque& a, deque& b) {
        std::swap(a.data, b.data);
    }
};

#endif
