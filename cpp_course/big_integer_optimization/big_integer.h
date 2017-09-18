#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#include"my_vector_uint32.h"
#include<algorithm>
#include<string>
#include<cassert>

class big_integer {
public:
    big_integer();
    big_integer(big_integer const&);
    big_integer(int);
    big_integer(uint32_t);
	explicit big_integer(std::string const& str);

    big_integer& operator= (big_integer const& other); //Copy Constructor

    big_integer& operator+=(big_integer const& other);
    big_integer& operator-=(big_integer const& other);
	big_integer& operator*=(big_integer const& other);
    big_integer& operator/=(big_integer const& other);
    big_integer& operator%=(big_integer const& other);

    big_integer& operator&=(big_integer const& other);
    big_integer& operator|=(big_integer const& other);
    big_integer& operator^=(big_integer const& other);

    big_integer& operator>>=(int size_shift);  // if size_shift < 0 AB
    big_integer& operator<<=(int size_shift);  // if size_shift < 0 AB

    big_integer operator+() const; //NB why?
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++(); // pref
    big_integer operator++(int); // post

    big_integer& operator--(); // pref
    big_integer operator--(int); // post

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

	friend std::string to_string(big_integer const& a);
private:
    my_vector_uint32 data;
    bool is_negative;
    big_integer& mul_short(uint32_t a); // why I can't make private operator*=
    std::pair<big_integer, big_integer> quotient_and_remainder(big_integer const& b) const;
    std::pair<big_integer, uint32_t> quotient_and_remainder_for_short_unsigned_divisor(uint32_t b);
    big_integer(my_vector_uint32 vec);
    static uint32_t const setted_one = 0xFFFFFFFF, setted_zero = 0x00000000;
	big_integer& normalize();
	void update_negative();
	void resize(uint32_t a);
    void invert();
	big_integer& add_with_shift(big_integer const &a, uint32_t shift);
	int compare(big_integer const& other) const; // a - b ? 0
	big_integer abs() const;
    int compare_prefix(big_integer const& other, int32_t begin_index) const;
	static big_integer uint64_to_big_integer(uint64_t a);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);
big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
//std::ostream& operator<<(std::ostream& s, big_integer const& a);

#endif //BIG_INTEGER_H
