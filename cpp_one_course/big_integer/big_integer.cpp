#include"big_integer.h"
#include<iostream>

big_integer::big_integer(): big_integer(0) {}

big_integer::big_integer(big_integer const& other): data(other.data),
    is_negative(other.is_negative) { }

big_integer::big_integer(int a): data() {
    data.push_back((uint32_t)a);
    update_negative();
}

big_integer::big_integer(std::string const &str): big_integer(0) {
    size_t i = 0; bool big_int_is_negative = false;
    if (str[0] == '-') {
        big_int_is_negative = true;
        i++;
    }
    for (; i < str.size(); i++) {
        (*this) *= 10;
        (*this) += int(str[i] - '0');
    }
    if (big_int_is_negative) {
       (*this) = -(*this);
    }
}

big_integer& big_integer::operator=(big_integer const& other) {
    data = other.data;
    is_negative = other.is_negative;
    return *this;
}

big_integer& big_integer::operator+=(big_integer const& other) {
    return add_with_shift(other, 0);
}

big_integer& big_integer::operator-=(big_integer const& other) {
    return *this += (-other);
}

big_integer& big_integer::operator*=(uint32_t a) {
    bool res_is_negative = is_negative;
    *this = abs();
    uint64_t buf, r = 0, mask_last32 = ((1ULL << 32) - 1);
    resize(data.size() + 1);
    for (size_t i = 0; i < data.size(); i++) {
        buf = 1ULL * data[i] * a + r;
        data[i] = buf & (mask_last32);
        r = buf >> 32;
    }
    if (res_is_negative)
        *this = -(*this);
    normalize();
    return *this;
}

big_integer& big_integer::operator*=(big_integer const& other) {
    bool res_is_negative = ((is_negative) ^ (other.is_negative));
    big_integer sum(0), a = (*this).abs(), b = other.abs();
    for (size_t i = 0; i < b.data.size(); i++) {
        sum.add_with_shift(big_integer(a) *= b.data[i], i);
    }
    return *this = (res_is_negative ? -sum : sum);
}

big_integer& big_integer::operator/=(big_integer const& other) {
    return (*this) = quotient_and_remainder(other).first;
}

big_integer& big_integer::operator%=(big_integer const& other) {
    return (*this) = quotient_and_remainder(other).second;
}

big_integer& big_integer::operator&=(big_integer const& other) {
    resize(other.data.size() + 1);
    for (size_t i = 0; i < other.data.size(); i++) {
        data[i] &= other.data[i];
    }
    data.back() &= (other.is_negative? setted_one: setted_zero);
    update_negative();
    normalize();
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& other) {
    resize(other.data.size() + 1);
    for (size_t i = 0; i < other.data.size(); i++) {
        data[i] |= other.data[i];
    }
    data.back() |= (other.is_negative? setted_one: setted_zero);
    update_negative();
    normalize();
    return *this;
}

big_integer& big_integer::operator^=(big_integer const& other) {
    resize(other.data.size() + 1);
    for (size_t i = 0; i < other.data.size(); i++) {
        data[i] ^= other.data[i];
    }
    data.back() ^= (other.is_negative? setted_one: setted_zero);
    update_negative();
    normalize();
    return *this;
}

big_integer& big_integer::operator>>=(int size_shift) {
    uint32_t cnt_deleted = size_shift / 32, r = size_shift % 32,
        right_mask_nr = ((1 << (31 - r) << 1)) - 1, left_mask_r = ~right_mask_nr,
        right_mask_r = ((1 << r) - 1), left_mask_nr = ~right_mask_r;
    if (cnt_deleted >= data.size()) return (*this) = (is_negative? -1: 0);
    data[0] = (data[cnt_deleted] & left_mask_nr) >> r;
    for (size_t i = cnt_deleted+1, right, left; i < data.size(); i++) {
        left = (data[i] & left_mask_nr) >> r;
        right = (data[i] & right_mask_r) << (32 - r);
        data[i - cnt_deleted - 1] = (data[i - cnt_deleted - 1] & (~left_mask_r)) | right;
        data[i - cnt_deleted] = (data[i - cnt_deleted] & (~right_mask_nr)) | left;
    }
    if (is_negative) {
        data[data.size() - 1 - cnt_deleted] &= (~left_mask_r);
        data[data.size() - 1 - cnt_deleted] |= (left_mask_r);
        for (size_t i = 0; i < cnt_deleted; i++) {
            data[data.size()-i-1] = setted_one;
        }
    }
    else {
        data[data.size() - 1 - cnt_deleted] &= (~left_mask_r);
        for (size_t i = 0; i < cnt_deleted; i++) {
            data[data.size()-1 - i] = setted_zero;
        }
    }
    normalize();
    return *this;
}

big_integer& big_integer::operator<<=(int size_shift) {
    uint32_t cnt_setted = size_shift / 32, r = size_shift % 32,
        right_mask_nr = ((1 << (31 - r) << 1))-1, left_mask_r = (0xFFFFFFFF - right_mask_nr),
        right_mask_r = ((1 << r) - 1), left_mask_nr = (0xFFFFFFFF - right_mask_r);
    uint32_t index_old_end = data.size()-1;
    resize(data.size() + cnt_setted + 1);
    for (int i = index_old_end, left, right; i >= 0; i--) {
        left = (data[i] & left_mask_r) >> (32 - r);
        right = (data[i] & right_mask_nr) << r;
        data[i + cnt_setted + 1] = ((data[i + cnt_setted + 1] & (~right_mask_r)) | left);
        data[i + cnt_setted] = ((data[i + cnt_setted] & (~left_mask_nr)) | right);
    }
    data[cnt_setted] &= (~right_mask_r);
    for (size_t i = 0; i < cnt_setted; i++)
        data[i] = setted_zero;
    normalize();
    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer temp = ~(*this);
    return temp += 1;
}

big_integer big_integer::operator~() const {
    big_integer temp = *this;
    temp.resize(temp.data.size() + 1);
    for (size_t i = 0; i < temp.data.size(); i++) {
        temp.data[i] = ~temp.data[i];
    }
    temp.update_negative();
    temp.normalize();
    return temp;
}

big_integer& big_integer::operator++() {
    return (*this) += 1;
}

big_integer big_integer::operator++(int) {
    big_integer temp = *this;
    *this += 1;
    return temp;
}

big_integer& big_integer::operator--() {
    return (*this) -= 1;
}

big_integer big_integer::operator--(int) {
    big_integer temp = *this;
    (*this) += -1;
    return temp;
}

big_integer::big_integer(std::vector<uint32_t> vec): data(vec), is_negative(false) {}

void big_integer::normalize() {
    if (is_negative)
        for (int i = data.size() - 1; i > 0; i--) {
            if (data[i] == setted_one)
                data.pop_back();
            else break;
        }
    else
        for (int i = data.size() - 1; i > 0; i--) {
            if (data[i] == setted_zero)
                data.pop_back();
            else break;
        }
}

void big_integer::update_negative() {
    is_negative = ((data.back() >> 31) & 1);
}

void big_integer::resize(uint32_t new_size) {
    if (new_size > data.size()) {
        uint32_t cnt_new_word = new_size - data.size();
        if (is_negative)
            for (size_t i = 0; i < cnt_new_word; i++)
                data.push_back(setted_one);
        else
            for (size_t i = 0; i < cnt_new_word; i++)
                data.push_back(setted_zero);
    }
}

big_integer& big_integer::add_with_shift(big_integer const &other, uint32_t shift){
    big_integer temp = (*this);
    uint32_t cnt_other = other.data.size(), i = 0; uint64_t buf, r = 0;
    temp.resize(std::max(temp.data.size(), other.data.size() + shift) + 1);
    for(;i < cnt_other; i++) {
        buf = r + temp.data[shift + i] + other.data[i];
        r = buf >> 32;
        temp.data[shift + i] = buf;
    }
    for(;i + shift < temp.data.size(); i++) {
        if(other.is_negative) {
            buf = r + temp.data[i + shift] + setted_one;
        } else buf = r + temp.data[i + shift] + setted_zero;
        r = buf >> 32;
        temp.data[shift + i] = buf;
    }
    temp.update_negative();
    temp.normalize();
    return (*this) = temp;
}

int big_integer::compare(big_integer const& other) const { // a - b ? 0
    if (is_negative && !other.is_negative)
        return -1;
    if (!is_negative && other.is_negative)
        return +1;
    int both_are_negative = (is_negative ? -1 : +1);
    if (data.size() != other.data.size()) {
        return (data.size() > other.data.size() ? +1 : -1) * both_are_negative;
    }
    else {
        for (int i = data.size() - 1; i >= 0; i--) {
            if (data[i] != other.data[i]) {
                return (data[i] > other.data[i] ? 1: -1) * both_are_negative;
            }
        }
        return 0;
    }
}

big_integer big_integer::abs() const {
    return (is_negative ? -(*this) : *this);
}

// +1 -- me > other // -1  -- me < other
int big_integer::compare_prefix(big_integer const& other, size_t begin_index) {
    if (begin_index < other.data.size()-1) {
        return 666;
    }
    bool flag_cmp = 0;
    for (size_t i = 0; i < other.data.size(); ++i) {
        if (data[begin_index - i] < other.data[other.data.size() - 1 - i]) {
            flag_cmp = -1;
            break;
        }
        if (data[begin_index - i] > other.data[other.data.size() - 1 - i]) {
            flag_cmp = 1;
            break;
        }
    }
    return flag_cmp;
}

std::pair<big_integer, uint32_t>
big_integer::quotient_and_remainder_for_short_divisor(uint32_t b) const {
    bool res_is_negative = this->is_negative;
    big_integer a = this->abs();
    int32_t large_index = a.data.size() - 1;
    uint32_t remainder = 0, shift = 0;
    uint64_t pre_a, dig_q;
    std::vector<uint32_t> quotient_vector;
    while(large_index > -1) {
        shift = 0;
        pre_a = a.data[large_index];
        if (pre_a < b && large_index == 0) {
            remainder = a.data[large_index];
            break;
        }
        if (pre_a < b) {
            shift = 1;
            pre_a <<= 32;
            pre_a += a.data[large_index -1];
        }
        dig_q = pre_a / b;
        quotient_vector.push_back(uint32_t(dig_q));
        a.add_with_shift(-uint64_to_big_integer(dig_q * b), large_index - shift);
        while(large_index > 0 && a.data[large_index] == 0) {
            large_index--;
        }
    }
    for (size_t i = 0; i < quotient_vector.size() / 2; i++) {
        std::swap(quotient_vector[i], quotient_vector[quotient_vector.size() - 1 -i]);
    }
    big_integer quotient(quotient_vector);
    return std::make_pair(quotient, remainder);
}

std::pair<big_integer, big_integer>
big_integer::quotient_and_remainder(big_integer const& B) const {
    assert(B != 0);
    bool res_is_negative = (this->is_negative ^ B.is_negative);
    big_integer a = this->abs(), b = B.abs();
    std::vector<uint32_t> Q;
    if (a == 0) return std::make_pair(0, 0);
    if (a < b) return std::make_pair(0, a);
    big_integer quotient, remainder;
    if (b.data.size() < 2) {
        std::pair<big_integer, uint32_t> temp = a.quotient_and_remainder_for_short_divisor( b.data[0]);
        quotient = temp.first;
        remainder = big_integer(temp.second);
    //}
    // if (a.data.size() < 3 && b.data.size() < 3) {
    //     uint64_t aa = 0, bb = 0;
    //     for (size_t i = 0; i < a.data.size(); i++) {
    //         aa <<= 32;
    //         aa += a.data[i];
    //     }
    //     for (size_t i = 0; i < b.data.size(); i++) {
    //            bb <<= 32;
    //            bb += b.data[i];
    //     }
    //     uint64_t q = aa / bb, r = aa % bb;
    //     quotien = uint64_to_big_integer(q);
    //     remainder = uint64_to_big_integer(r);
    } else {
        int ind_large_word_a = a.data.size() - 1, shift = 0;
        big_integer temp;
        while (true) {
        shift = 0;
        int cmp = a.compare_prefix(b, ind_large_word_a);
        if (cmp < 0) {
            shift = 1;
        }
        else if (cmp == 666) {
            remainder = a;
            break;
        }
        uint64_t pre_a = a.data[ind_large_word_a - 1] + (a.data[ind_large_word_a] << 32),
            pre_b = (shift == 0 ?
            (b.data[b.data.size() - 1] << 32) + b.data[b.data.size() - 2]
                : b.data[data.size() - 1]);
                uint32_t try_number = pre_a / pre_b;
                temp = (b *= try_number);
                while (true) {
                    cmp = a.compare_prefix(temp, ind_large_word_a);
                    if (cmp < 0)
                        break;
                    else {
                       temp -= b;
                       try_number--;
                   }
               }
               Q.push_back(try_number);
               temp = -temp;
               a.add_with_shift(temp, ind_large_word_a - temp.data.size() -1 -shift);
               while (ind_large_word_a >= 0  && a.data[ind_large_word_a] == 0)
                   ind_large_word_a--;
        }
        quotient = big_integer(Q.back());
        quotient.normalize();
        remainder.normalize();
        for (size_t i = Q.size() - 1; i--;)
            quotient.data.push_back(Q[i]);
   }
   if (res_is_negative) {
       quotient = -quotient;
   }
   else {
       quotient.is_negative = false;
   }
   if (this->is_negative) {
       remainder = -remainder;
   }
   return std::make_pair(quotient, remainder);
}

big_integer big_integer::uint64_to_big_integer(uint64_t a) {
   big_integer temp;
   if (a > (1ULL << 32)) {
       temp += uint32_t(a >> 32);
       temp <<= 32;
       temp += uint32_t(a);
   }
   else {
       temp +=uint32_t(a);
   }
   temp.is_negative = false;
   return temp;
}

//pre a > 0
//int big_integer::normalize_divisor(big_integer& a) {
//	size_t ind = a.data.size()-1, shift = 0;
//	uint32_t half = (1 << 16);
//	while (data[ind] == 0) ind--;
//	if (ind < 0) return -1;
//	while (data[ind] < half) {
//		a <<= 1;
//		shift++;
//	}
//	return shift;
//}

big_integer operator+(big_integer a, big_integer const& b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b) {
    return a += (-b);
}

big_integer operator*(big_integer a, big_integer const & b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const& b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

bool operator==(big_integer const& a, big_integer const& b) {
    return a.compare(b) == 0;
}

bool operator!=(big_integer const& a, big_integer const& b) {
    return a.compare(b) != 0;
}

bool operator<(big_integer const& a, big_integer const& b) {
    return a.compare(b) < 0;
}

bool operator>(big_integer const& a, big_integer const& b) {
    return a.compare(b) > 0;
}

bool operator<=(big_integer const& a, big_integer const& b) {
    return a.compare(b) <= 0;
}

bool operator>=(big_integer const& a, big_integer const& b) {
    return a.compare(b) >= 0;
}

std::string to_string(big_integer const& a) {
    std::string str;
    big_integer number;
    big_integer temp(a);
    while(temp != 0) {
        number = temp % 10;
        temp /= 10;
        str += (number.data[0]);
    }
    return str;
}
