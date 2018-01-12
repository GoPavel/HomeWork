#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#include <utility>

template<typename DATA_TYPE>
class shared_ptr {
private: /// member
    DATA_TYPE* data_ptr;
    int* counter_ptr;
public: /// method
    shared_ptr() noexcept
        : data_ptr(nullptr), counter_ptr(nullptr) { }

    shared_ptr(std::nullptr_t) noexcept
        : shared_ptr() { }

    shared_ptr(DATA_TYPE* data_ptr)
        : data_ptr(data_ptr) {
        try {
            counter_ptr = new int(1);
        } catch(...) {
            delete data_ptr;
            throw;
        }
    }

    shared_ptr(shared_ptr const& other) noexcept
        : data_ptr(other.data_ptr), counter_ptr(other.counter_ptr) {
        if (data_ptr) {
            ++(*counter_ptr);
        }
    }

    shared_ptr& operator= (shared_ptr const& other) noexcept {
        shared_ptr tmp(other);
        swap(*this, tmp);
        return *this;
    }

    shared_ptr(shared_ptr &&other) noexcept
        : data_ptr(other.data_ptr), counter_ptr(other.counter_ptr) {
        other.data_ptr = nullptr;
        other.counter_ptr = nullptr;
    }

    shared_ptr& operator= (shared_ptr &&other) noexcept {
        swap(*this, other);
        return *this;
    }

    ~shared_ptr() {
        remove_binding();
    }

    explicit operator bool() const noexcept {
        return data_ptr != nullptr;
    }

    DATA_TYPE& operator* () const noexcept {
        return *data_ptr;
    }

    DATA_TYPE* operator->() const noexcept {
        return data_ptr;
    }

    DATA_TYPE* get() const noexcept {
        return data_ptr;
    }

    friend bool operator== (shared_ptr const &a, shared_ptr const &b) noexcept {
        return a.data_ptr == b.data_ptr;
    }

    friend bool operator!= (shared_ptr const &a, shared_ptr const &b) noexcept {
        return a.data_ptr != b.data_ptr;
    }

    friend void swap(shared_ptr &a, shared_ptr &b) noexcept {
        std::swap(a.data_ptr, b.data_ptr);
        std::swap(a.counter_ptr, b.counter_ptr);
    }

private: /// method
    void remove_binding() noexcept {
        if (data_ptr) {
            --(*counter_ptr);
            if ((*counter_ptr) == 0) {
                delete data_ptr;
                delete counter_ptr;
            }
        }
    }
};

#endif // SHARED_PTR_H
