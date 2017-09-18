#include<iostream>

using namespace std;

class A {
public:
    // virtual void f() {};
};

class B: virtual public A {
public:
    // void f() {};
};

int main() {
    A* a = new B();
    B* b = dynamic_cast<B*> (a);
    return 0;
}
