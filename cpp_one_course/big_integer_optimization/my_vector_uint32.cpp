#include "my_vector_uint32.h"

my_vector_uint32::my_vector_uint32() : is_small(true) {
      small[0] = 0;
}

my_vector_uint32::my_vector_uint32(my_vector_uint32 const& other): is_small(other.is_small) {
    if (other.is_small) {
        Small::copy(*this, other);
    } else Big::copy(*this, other);
}

my_vector_uint32::~my_vector_uint32() {
    if(!is_small)
        Big::safe_delete(*this);
}

uint32_t my_vector_uint32::size() const{
    if (is_small) {
        return Small::size(*this);
    } else return Big::size(*this);
}

uint32_t& my_vector_uint32::back() {
    if(is_small) {
        return small[small[0]];
    } else {
        Big::separate(*this);
        return big->data.back();
    }
}

uint32_t const& my_vector_uint32::back() const{
    if(is_small)
        return small[small[0]];
    else return big->data.back();
}

uint32_t& my_vector_uint32::operator[](uint32_t ind) {
    if(is_small) {
        assert(!(ind >= size_small));
        return small[ind + 1];
    } else {
        Big::separate(*this);
        return big->data[ind];
    }
}

const uint32_t& my_vector_uint32::operator[](uint32_t ind) const{
    if(is_small) {
        if (ind >= size_small)
            throw "out of smallint's bound";
        return small[ind + 1];
    } else {
        return big->data[ind];
    }
}

my_vector_uint32& my_vector_uint32::operator=(my_vector_uint32 const& other) {
    my_vector_uint32 temp(other);
    if(!is_small)
        Big::safe_delete(*this);
    if(other.is_small) {
        Small::copy(*this, temp);
    } else {
        Big::copy(*this, temp);
    }
    is_small = other.is_small;
    return *this;
}

void my_vector_uint32::push_back(uint32_t val) {
    if(is_small) {
        if (small[0] == size_small) {
            Big::small_to_big(*this);
            big->data.push_back(val);
        } else {
            small[small[0] + 1] = val;
            small[0]++;
        }
    } else {
        Big::separate(*this);
        big->data.push_back(val);
    }
}

void my_vector_uint32::pop_back() {
    if(is_small) {
        if(small[0] == 0)
            throw "pop_back uncorrect for empty vector";
        else small[0]--;
    } else {
        Big::separate(*this);
        big->data.pop_back();
    }
}

void my_vector_uint32::resize(uint32_t new_size) {
    if(is_small) {
        if(new_size > size_small) {
            Big::small_to_big(*this);
            big->data.resize(new_size);
        } else {
            for (uint32_t i = small[0] + 1; i <= new_size; ++i) {
                small[i+1] = 0;
            }
        }
    } else {
        Big::separate(*this);
        big->data.resize(new_size);
    }
}

void my_vector_uint32::reserve(uint32_t new_cap) {
    if(is_small) {
        if (new_cap > size_small) {
            Big::small_to_big(*this);
            big->data.reserve(new_cap);
        }
    } else {
        Big::separate(*this);
        big->data.reserve(new_cap);
    }
    return;
}

void my_vector_uint32::reverse() {
    if(is_small) {
        for(uint32_t i = 0; i < small[0] / 2; i++) {
            std::swap(small[i + 1], small[small[0] - i]);
        }
    } else {
        Big::separate(*this);
        std::reverse(big->data.begin(), big->data.end());
    }
}

void my_vector_uint32::clear() {
    if(is_small) {
        small[0] = 0;
    } else {
        Big::safe_delete(*this);
        is_small = true;
        small[0] = 0;
    }
    return;
}

/////////////BIG///////////////////
uint32_t my_vector_uint32::Big::size(my_vector_uint32 const& a) {
    return a.big->data.size();
}

void my_vector_uint32::Big::copy(my_vector_uint32& a, my_vector_uint32 const& b) {
    a.big = b.big;
    a.big->cnt_link++;
}

void my_vector_uint32::Big::safe_delete(my_vector_uint32& _this) {
    if(_this.big->cnt_link > 1)
        _this.big->cnt_link--;
    else
        delete _this.big;
}

void my_vector_uint32::Big::separate(my_vector_uint32& _this) {
    if(_this.big->cnt_link > 1) {
        shared_vector* temp = new shared_vector();
        _this.big->cnt_link--;
        temp->data = _this.big->data;
        temp->cnt_link = 1;
        std::swap(temp, _this.big);
    }
}

void my_vector_uint32::Big::small_to_big(my_vector_uint32& _this) {
    assert(_this.is_small == true);
    shared_vector* temp = new shared_vector();
    temp->data.reserve(_this.small[0]);
    for(uint32_t i = 0;  i < _this.small[0]; ++i) {
        temp->data.push_back(_this.small[i + 1]);
    }
    temp->cnt_link = 1;
    std::swap(temp, _this.big);
    _this.is_small = false;
    return;
}

///////////////SMALL/////////////////
uint32_t my_vector_uint32::Small::size(my_vector_uint32 const& a) {
    return a.small[0];
}

void my_vector_uint32::Small::copy(my_vector_uint32& a, my_vector_uint32 const& b) {
    for(uint32_t i = 0; i <= size_small; ++i) {
        a.small[i] = b.small[i];
    }
    return;
}
