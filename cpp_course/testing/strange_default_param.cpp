#include <iostream>

using namespace std;

class A {
public:
    virtual void f(int a = 2) {
        cout << "I am A and a = " << a << endl;
    }
};

class B: public A {
public:
    void f(int a = 3) override {
        cout << "I am B and a = " << a << endl;
    }
};
int main() {
    B b;
    A& a = b;
    a.f();
    return 0;
}
