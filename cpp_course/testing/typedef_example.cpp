#include <iostream>

struct Type {
    int a;
};

typedef Type Type_clone;

void f(Type &a) {
    std::cout << a.a << std::endl;
}

void f_clone(Type_clone &a) {
    std::cout << a.a << std::endl;
}


int main() {
    Type a;
    a.a = 5;
    Type_clone b;
    b.a = 3;
    std::cout << static_cast<Type_clone>(a).a << std::endl;
    std::cout << static_cast<Type>(b).a << std::endl;
    f(a);
    f(b);
    f_clone(a);
    f_clone(b);
    return 0;
}
