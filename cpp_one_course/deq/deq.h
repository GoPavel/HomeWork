#ifndef _MY_DEQ_
#define _MY_DEQ_
#include<cassert>

template <typename T>
class deque {
private:
    T* data;
    ize_t capacity, size;
    size_t head, tail // [head, tail)

    template <typename U>
    class iterator {
    private:
        T* ptr;

        size_t index() {
            return ptr - data;
        }
    public:
        iterator(size_t index): ptr(data + index) { }

        iterator(iterator const& other): ptr(other.ptr) {}

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
            if (index() + n < capacity) {
                ptr += n;
            } else {
                ptr = data + (n - (index() - capacity));
            }
            return *this;
        }
        iterator& operator-=(size_t n) {
            if(index() - n >= 0) {
                ptr -= n;
            } else {
                ptr = (data + capacity) - (n - index());
            }
            return *this;
        }

        U& operator*() const {
            return *ptr;
        }

        U& operator[](size_t index) const {
            iterator copy(*this);
            return *(copy += index);
        }

        friend bool operator==(iterator const& a, iterator const& b) {
            return a.ptr == b.ptr;
        }
        friend bool operator!=(iterator const& a, iterator const& b) {
            return a.ptr != b.ptr;
        }

        friend bool operator<(iterator const& a, iterator const& b);
        friend bool operator>(iterator const& a, iterator const& b);
        friend bool operator<=(iterator const& a, iterator const& b);
        friend bool operator>=(iterator const& a, iterator const& b);

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
    };

public:
    deque();

    deque(deque const& other);

    ~deque();

    bool empty() const;

    size_t size() const;

    void clear();

    void push_back(T const& new_element);

    void push_front(T const& new_elemen);

    void pop_back();

    void pop_front();

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    iterator rbegin();
    const_iterator rbegin() const;

    iterator rend();
    const_iterator rend() const;

    T& back();
    T const& back() const;

    T& front();
    T const& front() const;

    iterator insert(const_iterator const& it, T const& new_element);

    iterator erase(const_iterator const& it, T const& new_element);

    friend void swap(deque& a, deque& b);
};

#endif
