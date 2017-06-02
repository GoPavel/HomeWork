#include<iostream>
#include<vector>
#include"big_integer.h"

using namespace std;


int main() {
	/*big_integer a("72057594037927936"), b("4294967296");
	a += 1;
	b += 1;*/


	big_integer a(0), b;
	a += (-1);
	a += (1);
	a += (1073741824);
	a += a;
	a += a;
	a += -1; // pos
	a = -1; // neg end equal data
	a = ~a;
	a = ~a; 
	a = 7;
	a = ~a;
	a = ~a;
	a = 0;
	a = ~a;
	a = ~a;
	//int _test = []
	vector<int> test({1, 2, 4, 6, 1321, 43543, 567567587});
	a = 0;
	for (size_t i = 0; i < test.size(); i++) {
		a += (-test[i]);
		a -= (-test[i]);
	}
	a = -1;
	a >>= 33;
	a = 0;
	a >>= 1;
	a >>= 32;
	a = 0;
	a >>= 33;
	a = -1;
	a <<= 1;
	a <<= 32;
	a <<= 33;
	a >>= 33;
	a >>= 32;
	a >>= 1;
	a = 5; b = -5;
	a <<= 36; b <<= 36;
	bool flag = a < b;
	flag = a <= b;
	flag = a > b;
	flag = a >= b;
	flag = a != b;
	flag = a == b;
	a = 3;
	a *= 4;
	a = 3; b = 4;
	a <<= 16, b <<= 16;
	a *= b;
	a >>= 32;
	a = 0;
	a--;
	a++;
	----a;
	++++a;
	a = 6, b = 3;
	pair<big_integer, big_integer> pr = a.quotient_and_remainder(b);
	
	return 0;
}