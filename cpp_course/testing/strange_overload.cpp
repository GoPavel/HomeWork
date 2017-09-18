#define __BAD__
#include <iostream>

using namespace std;

struct T{ int a;};

#ifndef __BAD__
    T& f(T &a) {
        cout << "T: " << endl;
        return a;
    }

    const T& f(const T &a) {
        cout << "const T: " << endl;
        return a;
}
#else
    T f(T a) {
        cout << "T: " << endl;
        return a;
    }

    const T f(const T a) {
        cout << "const T: " << endl;
        return a;
    }
#endif
int main() {
    T a;
    const T b = T();
    f(a);
    f(b);
    return 0;
}
