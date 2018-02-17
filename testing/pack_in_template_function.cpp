#include <iostream>
#include <string>

using namespace std;

template <typename T>
int counter(T a) {
    return 1;
}

template <typename T,  typename... Other_T>
int counter(T a, Other_T... other) {
    return 1 + counter(other...);
}

int main() {
    cout << counter(int(2), int(2), double(2.0), string("Hi")) << endl;
    return 0;
}
