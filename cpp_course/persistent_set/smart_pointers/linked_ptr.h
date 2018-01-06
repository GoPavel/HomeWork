#ifndef LINKED_PTR_H
#define LINKED_PTR_H
#include "smart_pointer_interface.h"

template<typename DATA_TYPE>
class linked_ptr {
private: /// member
    DATA_TYPE* data_ptr;
    mutable linked_ptr* next;
    mutable linked_ptr* prev;
public: /// method
    linked_ptr() noexcept
        : data_ptr(nullptr), next(nullptr), prev(nullptr) { }

    linked_ptr(std::nullptr_t) noexcept
        : linked_ptr() { }

    linked_ptr(DATA_TYPE *data_ptr) noexcept
        : data_ptr(data_ptr), next(nullptr), prev(nullptr) { }

    linked_ptr(linked_ptr const &other) noexcept
        : data_ptr(other.data_ptr), next(nullptr) {
        if (data_ptr) {
            prev = const_cast<linked_ptr*>(&other);
            other.next = this;
        } else {
            prev = nullptr;
        }
    }

    linked_ptr& operator= (linked_ptr const &other) noexcept {
        linked_ptr tmp(other);
        swap(*this, tmp);
        return *this;
    }

    linked_ptr(linked_ptr &&other) noexcept
        : data_ptr(other.data_ptr), next(other.next), prev(other.prev) {
        other.data_ptr = nullptr;
        other.next = other.prev = nullptr;
    }

    linked_ptr& operator= (linked_ptr &&other) noexcept {
        swap(*this, other);
        return *this;
    }

    ~linked_ptr() {
        remove_binding();
    }

    operator bool() const noexcept {
        return data_ptr != nullptr;
    }

    DATA_TYPE& operator*() const noexcept {
        return *data_ptr;
    }

    DATA_TYPE* get() const noexcept {
        return data_ptr;
    }

    DATA_TYPE* operator-> () const noexcept {
        return data_ptr;
    }

    friend bool operator== (linked_ptr const& a, linked_ptr const& b) noexcept {
        return a.data_ptr == b.data_ptr;
    }

    friend bool operator!= (linked_ptr const& a, linked_ptr const& b) noexcept {
        return !(a.data_ptr == b.data_ptr);
    }

    friend void swap(linked_ptr &a, linked_ptr &b) noexcept {
        if (a == b) return;
        std::swap(a.data_ptr, b.data_ptr);
        std::swap(a.next, b.next);
        std::swap(a.prev, b.prev);
        if (a.next) { a.next->prev = &a; }
        if (a.prev) { a.prev->next = &a; }
        if (b.next) { b.next->prev = &b; }
        if (b.prev) { b.prev->next = &b; }
    }

private: /// method
    void remove_binding() {
        if (data_ptr) {
            if (next == nullptr && prev == nullptr)
                delete data_ptr;
            else {
                if (next) {
                    next->prev = prev;
                }
                if (prev) {
                    prev->next = next;
                }
            }
        }
    }
};

#endif // LINKED_PTR_H
