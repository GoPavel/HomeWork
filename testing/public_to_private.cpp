#include <iostream>

using namespace std;

class A {
private:
public:
    int a;
    A(): a(5) {}
};

class B: public A {
public:
    int b;
    B(): A(), b(3) {}
private:
    using A::a;
};

class C: public B {

};

int main() {
    C b;
    cout << b.b << endl;
    cout << b.a << endl;
    return 0;
}
