#ifndef MY_VECTOR_UINT32
#define MY_VECTOR_UINT32
#include<vector>
#include<cassert>
#include<cstdint>
#include<algorithm>

struct shared_vector {
    std::vector<uint32_t> data;
    uint32_t cnt_link;
};

class my_vector_uint32 {
    static const uint32_t size_small = 5;
public:
    my_vector_uint32();
    my_vector_uint32(my_vector_uint32 const& other);

    ~my_vector_uint32();

    uint32_t size() const;
    uint32_t& back();
    const uint32_t& back() const;
    uint32_t& operator[](uint32_t ind);
    const uint32_t& operator[](uint32_t ind) const;
    my_vector_uint32& operator=(my_vector_uint32 const& other);
    void push_back(uint32_t val);
    void pop_back();
    void resize(uint32_t new_size);
    void reserve(uint32_t new_cap);
    void reverse();
    void clear();
private:
    struct Big {
        static uint32_t size(my_vector_uint32 const& a);
        static void copy(my_vector_uint32& a, my_vector_uint32 const& b);//a = b.clone
        static void safe_delete(my_vector_uint32& _this);
        static void separate(my_vector_uint32& _this);
        static void small_to_big(my_vector_uint32& _this);
    };
    struct Small {
        static uint32_t size(my_vector_uint32 const& a);
        static void copy(my_vector_uint32& a, my_vector_uint32 const& b); //a = b.clone
    };
    bool is_small;

    union {
        shared_vector* big;
        uint32_t small[size_small + 1];
    };
};

#endif
