#include"big_integer.h"
#include<iostream>

big_integer::big_integer(): big_integer(0) {}

big_integer::big_integer(big_integer const& other): data(other.data) { }

big_integer::big_integer(int a): data() {
	data.push_back((uint32_t)a);
}

big_integer::big_integer(std::string const &str): big_integer(0) {
	for (size_t i = 0; i < str.size(); i++) {
		(*this) *= 10;
		(*this) += uint32_t(str[i] - '0');
	}
}


big_integer::~big_integer() {
	data.~vector();
}

big_integer& big_integer::operator=(big_integer const& other) {
	(*this).swap(big_integer(other));
	return *this;
}

void big_integer::swap(big_integer& other) {
	using std::swap;
	swap(data, other.data);
}

big_integer& big_integer::operator+=(big_integer const& other) {
	uint32_t r = 0, cnt = other.data.size();
	resize(cnt+1);
	for (uint32_t i = 0; i < cnt; i++) {
		data[i] += other.data[i] + r;
		if (data[i] < other.data[i] || (r && data[i] == other.data[i]))
			r = 1;
	}
	if (other.is_negative())
		data[cnt] += setted_one + r;
	else data[cnt] += setted_zero + r;
	normalize();
	return *this;
}

big_integer& big_integer::operator-=(big_integer const& other) {
	return *this += (-other);
}

big_integer& big_integer::operator*=(int a) {
	bool res_is_negative = (a < 0 || is_negative()) && !(a < 0 && is_negative());
	*this = abs();
	uint64_t buf, r = 0, mask_last32 = ((1ULL << 33) - 1);
	resize(data.size() + 1);
	for (size_t i = 0; i < data.size(); i++) {
		buf = data[i];
		buf *= a;
		buf += r;
		data[i] = buf & (mask_last32);
		r = (buf & (~mask_last32)) >> 32;
	}
	if (res_is_negative)
		*this = -(*this);
	normalize();
	return *this;
}

big_integer& big_integer::operator*=(big_integer const& other) {
	bool res_is_negative = (is_negative() || other.is_negative()) && 
		!(is_negative() && other.is_negative());
	big_integer temp(0);
	big_integer m1 = (*this).abs(), m2 = other.abs();
	for (size_t i = 0; i < m2.data.size(); i++) {
		temp += ((m1 << i) *= m2.data[i]);
	}
	return *this = (res_is_negative ? -temp : temp);
}

big_integer& big_integer::operator&=(big_integer const& other) {
	resize(other.data.size() + 1);
	for (size_t i = 0; i < data.size(); i++) {
		data[i] &= other.data[i];
	}
	normalize();
	return *this;
}

big_integer& big_integer::operator|=(big_integer const& other) {
	resize(other.data.size() + 1);
	for (size_t i = 0; i < data.size(); i++) {
		data[i] |= other.data[i];
	}
	normalize();
	return *this;
}

big_integer& big_integer::operator^=(big_integer const& other) {
	resize(other.data.size() + 1);
	for (size_t i = 0; i < data.size(); i++) {
		data[i] ^= other.data[i];
	}
	normalize();
	return *this;
}


big_integer& big_integer::operator>>=(int size_shift) {
	uint32_t cnt_deleted = size_shift / 32, r = size_shift % 32,
		right_mask_nr = ((1 << (31 - r) << 1)) - 1, left_mask_r = (0xFFFFFFFF - right_mask_nr),
		right_mask_r = ((1 << r) - 1), left_mask_nr = (0xFFFFFFFF - right_mask_r);
	data[0] = (data[cnt_deleted] & left_mask_nr) >> r;
	for (size_t i = cnt_deleted+1, right, left; i < data.size(); i++) {
		left = (data[i] & left_mask_nr) >> r;
		right = (data[i] & right_mask_r) << (32 - r);
		data[i - cnt_deleted - 1] = (data[i - cnt_deleted - 1] & (~left_mask_r)) | right;
		data[i - cnt_deleted] = (data[i - cnt_deleted] & (~right_mask_nr)) | left;
	}
	if (is_negative()) {
		for (size_t i = 0; i < cnt_deleted; i++) {
			data[data.size()-i-1] = setted_one;
		}
	}
	else {
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
	data[cnt_setted] = data[cnt_setted] & (~right_mask_r);
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
	for (size_t i = 0; i < data.size(); i++) {
		temp.data[i] = ~temp.data[i];
	}
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




bool big_integer::is_negative() const {
	return (data.back() >> 31) & 1;
}

void big_integer::normalize() {
	if (is_negative())
		for (int i = data.size() - 1; i > 0; i--) {
			if (data[i] == setted_one && data[i-1] == setted_one)
				data.pop_back();
			else break;
		}
	else
		for (int i = data.size() - 1; i > 0; i--) {
			if (data[i] == setted_zero && data[i-1] == setted_zero)
				data.pop_back();
			else break;
		}
}

void big_integer::resize(uint32_t new_size) {
	if (new_size > data.size()) {
		uint32_t cnt_new_word = new_size - data.size();
		if (is_negative())
			for (size_t i = 0; i < cnt_new_word; i++)
				data.push_back(setted_one);
		else
			for (size_t i = 0; i < cnt_new_word; i++)
				data.push_back(setted_zero);
	}
}

int big_integer::compare(big_integer const& other) const {
	if (is_negative() && !other.is_negative())
		return -1;
	if (!is_negative() && other.is_negative())
		return +1;
	int both_are_negative = (is_negative() ? -1 : +1);
	if (data.size() != other.data.size()) {
		return (data.size() > other.data.size() ? +1 : -1) * both_are_negative;
	}
	else {
		for (int i = data.size() - 1; i >= 0; i--) {
			if (data[i] > other.data[i]) {
				return both_are_negative;
			}
		}
		return 0;
	}
}


big_integer big_integer::abs() const { 
	return (is_negative() ? -(*this) : *this);
}

// +1 -- me > other // -1  -- me < other
int big_integer::compare_prefix(big_integer const& other, size_t begin_index) {
	if (begin_index < other.data.size()-1) {
		std::cout << "int big_integer::compare_prefix(big_integer const& other, size_t begin_index) {\n it's fail";
		return -1;
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

std::pair<big_integer, big_integer>
big_integer::quotient_and_remainder(big_integer const& B) const {
	bool res_is_negative = (this->is_negative() || B.is_negative()) && !(this->is_negative() && B.is_negative());
	big_integer a = this->abs(), b = B.abs(), temp;
	std::vector<uint32_t> Q;
	if (a < b) return std::make_pair(0, 0);
	int ind_large_word_a = a.data.size()-1, shift = 0;
	while (true) {
		shift = 0;
		int cmp = a.compare_prefix(b, ind_large_word_a);
		if (cmp >= 0) {
			shift = 1;
		}
		else {
			break;
		}
		uint64_t pre_a = a.data[ind_large_word_a - 1] + (a.data[ind_large_word_a] << 32),
			pre_b = (shift == 0? 
				(b.data[b.data.size() - 1] << 32) + b.data[b.data.size() - 2]
				: b.data[data.size()-1]);
		uint32_t try_number = pre_a / pre_b;
		temp = b * try_number;
		while (true) {
			cmp = a.compare_prefix(temp, ind_large_word_a);
			if (cmp >= 0)
				break;
			else {
				temp -= b;
				try_number--;
			}
		}
		Q.push_back(try_number);
		temp = -temp;
		uint32_t r = 0, i, index;
		for (i = 0, r = 0, index; i < temp.data.size(); i++) {
			index = ind_large_word_a - shift - (b.data.size() - 1) + i;
			a.data[index] += temp.data[i];
			if (a.data[index] < temp.data[i] || (r && a.data[index] == temp.data[i]))
				r = 1;
		}
		if (r)
			a.data[ind_large_word_a - shift - (b.data.size() - 1) + i] += r; //// i belive
		while (ind_large_word_a >= 0  && a.data[ind_large_word_a] == 0)
			ind_large_word_a--;
	}
	big_integer quotien(Q.back()), remainder;
	for (size_t i = Q.size() - 1; i--;)
		quotien.data.push_back(Q[i]);
	remainder = a;
	return std::make_pair(quotien, remainder);
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
