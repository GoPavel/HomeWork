#include <iostream>

using namespace std;

class A {
public:
    int a = 0;
    void f() {
        cout << "A.a = 0 = " << a << endl;
    }
};

class B: public A {
public:
    int a = 1;
    void f() {
        cout << "B.a = 1 = " << a << endl;
    }
};

int main() {
    A a;
    a.f();
    B b;
    b.f();
    A &aa = b;
    aa.f();
    return 0;
}
