#include <iostream>

using namespace std;

template <typename T> void foo(T) { cout << "1\n"; }
template <> void foo(int *) { cout << "2\n"; }
template <typename T> void foo(T*) {cout << "3\n"; }

template <typename T> void bar(T) { cout << "4\n"; }
template <typename T> void bar(T*) {cout << "5\n"; }
template <> void bar(int *) { cout << "6\n"; }


int main() {
    int a = 4;
    foo(&a);
    bar(&a);
    return 0;
}
