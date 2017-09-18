#include <iostream>

using namespace std;

class A {
public:
    virtual void f() {
        cout << "A::f()" << endl;
    }
    virtual void f(int a) {
        a++;
        cout << "A::f(int)" << endl;
    }
};

class B: public A {
public:
    using A::f;
    // void f() {
    //     cout << "B::f()" << endl;
    // }
    //
    void f(int a) {
        a++;
        cout << "B::f(int)" << endl;
    }
};

int main() {
    A a;
    a.f();
    a.f(3);

    B b;
    b.f();
    b.f(4);

    A & aa = b;
    aa.f();
    aa.f(1);
    return 0;
}
