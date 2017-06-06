#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H
#include<vector>
#include<string>

class big_integer {
public:
    big_integer(); /*TODO*/
    big_integer(big_integer const&); /*TODO*/
    big_integer(int); /*TODO*/
	explicit big_integer(std::string const& str); /*TODO*/
    
    big_integer& operator= (big_integer const& other); /*TODO*/ //Copy Constructor

    big_integer& operator+=(big_integer const& other); /*TODO*/
    big_integer& operator-=(big_integer const& other);
	big_integer& operator*=(uint32_t a);
	big_integer& operator*=(big_integer const& other); /*TODO*/
    big_integer& operator/=(big_integer const& other); /*TODO*/
    big_integer& operator%=(big_integer const& other); /*TODO*/

    big_integer& operator&=(big_integer const& other); /*TODO*/
    big_integer& operator|=(big_integer const& other); /*TODO*/
    big_integer& operator^=(big_integer const& other); /*TODO*/

    big_integer& operator>>=(int size_shift); /*TODO*/ // if size_shift < 0 AB
    big_integer& operator<<=(int size_shift); /*TODO*/ // if size_shift < 0 AB

    big_integer operator+() const; //NB why?
    big_integer operator-() const; /*TODO*/
    big_integer operator~() const; /*TODO*/

    big_integer& operator++(); /*TODO*/// pref
    big_integer operator++(int); // post

    big_integer& operator--(); /*TODO*/// pref
    big_integer operator--(int); // post

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

	friend std::string to_string(big_integer const& a); /*TODO*/
	std::pair<big_integer, big_integer>
		quotient_and_remainder(big_integer const& b) const;
private:
	uint32_t const setted_one = 0xFFFFFFFF, setted_zero = 0x00000000;
	void normalize();
	bool is_negative;
	std::vector<uint32_t> data;
	void update_negative();
	void resize(uint32_t a);
	big_integer& add_with_shift(big_integer const &a, uint32_t shift);
	int compare(big_integer const& other) const; // a - b ? 0
	big_integer abs() const;
	int compare_prefix(big_integer const& other, size_t begin_index);
	static big_integer uint64_to_big_integer(uint64_t a);
//	int normalize_divisor(big_integer& a);
//	uint32_t try_div(big_integer const& a, big_integer const& b);
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
