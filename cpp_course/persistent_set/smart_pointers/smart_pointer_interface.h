#ifndef SMART_POINTER_INTERFACE_H
#define SMART_POINTER_INTERFACE_H

template<typename T>
class smart_pointer_interface {
public:
    typedef T DATA_TYPE;

    virtual DATA_TYPE* get() = 0;

    virtual DATA_TYPE& operator*() = 0;

    virtual operator bool() = 0;

    virtual bool operator==(smart_pointer_interface const& other) = 0;

    virtual bool operator!=(smart_pointer_interface const& other) = 0;

    virtual ~smart_pointer_interface() = 0;
}

#endif // SMART_POINTER_INTERFACE_H
