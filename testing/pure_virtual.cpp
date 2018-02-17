#include <iostream>

using namespace std;

class A {
public:
    virtual void f() = 0;
    virtual void g();
};
void A::f() {cout << "++\n";}

class B: public A {
    void f() {cout << "--\n";}
};

class C: public A {
    void f() {} ;
} c;
int main() {
    B b;
    A* p = &b;
    p->A::f();
    return 0;
}
