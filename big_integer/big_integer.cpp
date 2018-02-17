#include"big_integer.h"
#include<iostream>

const uint32_t big_integer::setted_one, big_integer::setted_zero;

big_integer::big_integer(): big_integer(0) {}

big_integer::big_integer(big_integer const& other): data(other.data), is_negative(other.is_negative) { }

big_integer::big_integer(int a): data() {
    data.push_back((uint32_t)a);
    update_negative();
}

big_integer::big_integer(uint32_t a): data() {
    data.push_back(a);
    is_negative = false;
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
    add_with_shift(big_integer(other), 0);
    normalize();
    return (*this);
}

big_integer& big_integer::operator-=(big_integer const& other) {
    add_with_shift(-other, 0);
    normalize();
    return (*this);
}

big_integer& big_integer::mul_short(uint32_t a) {
    bool res_is_negative = is_negative;
    //*this = abs();
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
        sum.add_with_shift(big_integer(a).mul_short(b.data[i]), i);
        sum.normalize();
    }
    *this = (res_is_negative ? -sum : sum);
    normalize();
    return *this;
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
        right_mask_nr = ((1u << (31 - r) << 1)) - 1, left_mask_r = ~right_mask_nr,
        right_mask_r = ((1u << r) - 1), left_mask_nr = ~right_mask_r;
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
        right_mask_nr = ((1u << (31 - r) << 1))-1, left_mask_r = ~right_mask_nr,
        right_mask_r = ((1u << r) - 1), left_mask_nr = ~right_mask_r;
    uint32_t index_old_end = data.size()-1;
    resize(data.size() + cnt_setted + 1);
    uint32_t left, right;
    for (int i = index_old_end; i >= 0; i--) {
        if (r != 0)
            left = (data[i] & left_mask_r) >> (32 - r);
        else left = 0;
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
    big_integer temp = (*this);
    temp.invert();
    temp.add_with_shift(1, 0);
    temp.normalize();
    return temp;
}

void big_integer::invert() {
    resize(data.size() + 1);
    for (size_t i = 0; i < data.size(); i++) {
        data[i] = ~data[i];
    }
    update_negative();
    normalize();
}

big_integer big_integer::operator~() const {
    big_integer temp = *this;
    temp.invert();
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

big_integer::big_integer(std::vector<uint32_t> vec): data(vec), is_negative(false) {
    normalize();
}

// delete insignificant word
//NB big_integer must have corret sign flag
//NB big_integer after normalize have more, than nil word
big_integer& big_integer::normalize() {
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
    return (*this);
}

// NB big_integer must have insignificant digit
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

// NB after this func haven't normalize
big_integer& big_integer::add_with_shift(big_integer const &other, uint32_t shift) {
    //big_integer temp = other;
    uint32_t i = 0; uint64_t buf, r = 0;
    resize(std::max(data.size(), other.data.size() + shift) + 1);
    for(;i < other.data.size(); i++) {
        buf = r + data[shift + i] + other.data[i]; // TODO? left sum earler right sum
        r = buf >> 32;
        data[shift + i] = buf;
    }
    for(;i + shift < data.size(); i++) {
        if(other.is_negative) {
            buf = r + data[i + shift] + setted_one;
        } else buf = r + data[i + shift] + setted_zero;
        r = buf >> 32;
        data[shift + i] = buf;
    }
    update_negative();
    return (*this);
}

// NB big_integers must be normalize
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

std::pair<big_integer, uint32_t>
big_integer::quotient_and_remainder_for_short_unsigned_divisor(uint32_t b) {
    assert((*this) > 0);
    big_integer &a = (*this);
    int32_t shift = 2;
    a.data.push_back(0);
    uint64_t prefix_a; uint32_t digit_quotient;
    std::vector<uint32_t> quotient_vector;
    for (int32_t large_word_index = a.data.size() - 1; (large_word_index - (shift-1)) >= 0; large_word_index--) {
        prefix_a = a.data[large_word_index];
        prefix_a <<= 32;
        prefix_a |= a.data[large_word_index -1];
        digit_quotient = uint32_t(prefix_a / b);
        quotient_vector.push_back(digit_quotient);
        a.add_with_shift(-uint64_to_big_integer(uint64_t(b) * digit_quotient), large_word_index - (shift-1));
        for(int32_t i = a.data.size()-1; i >= large_word_index; i--) {
            if (a.data[i] == 0)
                a.data.pop_back();
            else break;
        }
    }
    std::reverse(quotient_vector.begin(), quotient_vector.end());
    big_integer quotient(quotient_vector);
    quotient.normalize();
    return std::make_pair(quotient, a.data[0]);
}

// +1 -- me > other // -1  -- me < other // 0 -- me == other
int big_integer::compare_prefix(big_integer const& other, int32_t end_index) const {
    int32_t len_prefix = data.size() - end_index;
    for (int32_t i = data.size() - 1, j = len_prefix-1; j >= 0 && i >= end_index ; --i, --j) {
        static uint32_t digit_other;
        digit_other = (j >= (int32_t)other.data.size()? 0: other.data[j]);
        if (data[i] > digit_other )
            return 1;
        if (data[i] < digit_other)
            return -1;
    }
    return 0;
}

std::pair<big_integer, big_integer> big_integer::quotient_and_remainder(big_integer const& B) const {
    if (B == 0)
        throw std::runtime_error("Division by zero");
    bool res_is_negative = (this->is_negative ^ B.is_negative);
    big_integer a = this->abs(), b = B.abs();
    big_integer quotient, remainder;
    if (a == 0) {
        quotient = 0;
        remainder = 0;
    } else if (a.data.size() < b.data.size()) {
        quotient = 0;
        remainder = a;
    } else if (b.data.size() < 2) {
        std::pair<big_integer, uint32_t> temp = a.quotient_and_remainder_for_short_unsigned_divisor(b.data[0]);
        quotient = temp.first;
        remainder = big_integer(temp.second);
    } else {
        big_integer subtraction;
        std::vector<uint32_t> quotient_vector;
        a.data.push_back(0);
        int32_t len_prefix = b.data.size() + 1;
        uint32_t  trial;
        quotient_vector.reserve(a.data.size() - b.data.size() + 2);
        __uint128_t prefix_a, prefix_b = (__uint128_t(b.data[b.data.size() - 1]) << 32) | b.data[b.data.size() - 2];
        for (int32_t large_word_index = a.data.size() - 1; (large_word_index - (len_prefix-1)) >= 0; large_word_index--) {
            prefix_a = a.data[large_word_index];
            prefix_a <<= 32;
            prefix_a |= a.data[large_word_index -1];
            prefix_a <<= 32;
            prefix_a |= a.data[large_word_index -2];
            prefix_a /= prefix_b;
            trial = ((prefix_a >> 32) == 0 ? uint32_t(prefix_a) : setted_one);
            subtraction = b;
            subtraction.mul_short(trial);
            while(a.compare_prefix(subtraction, large_word_index - (len_prefix-1)) < 0) {
                subtraction -= b;
                trial--;
            }
            quotient_vector.push_back(trial);
            a.add_with_shift(-(subtraction), large_word_index - (len_prefix-1));
            for(int32_t i = a.data.size()- 1; i >= large_word_index; i--) {
                a.data.pop_back();
            }
        }
        std::reverse(quotient_vector.begin(), quotient_vector.end());
        a.normalize();
        remainder = a;
        quotient = big_integer(quotient_vector);
        quotient.normalize();
    }
   if (res_is_negative) {
       quotient = -quotient;
   }
   if (this->is_negative) {
       remainder = -remainder;
   }
   quotient.normalize();
   remainder.normalize();
   return std::make_pair(quotient, remainder);
}

big_integer big_integer::uint64_to_big_integer(uint64_t a) {
   big_integer temp(0);
   if ((a >> 32) != 0) {
       temp += uint32_t(a >> 32);
       temp <<= 32;
   }
   temp += uint32_t(a);
   return temp;
}

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
    if (a == 0) {
        return "0";
    }
    std::string str;
    big_integer number;
    big_integer temp = a.abs();
    while(temp != 0) {
        number = temp % 10;
        temp /= 10;
        str += std::to_string(number.data[0]);
    }
    std::reverse(str.begin(), str.end());
    return (a.is_negative? "-": "") + str;
}
