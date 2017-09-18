#include <iostream>

using namespace std;

typedef int arr[3];
typedef long long ll;
typedef long long & c_ll;
typedef int (&c_arr)[3];

c_arr f(c_arr a) {
    a[0]++;
    a[1]++;
    a[2]++;
    return a;
}

int main() {
    ll c = 0;
    c_ll cc = c;
    cout << cc << endl;

    int a[] = {1, 2, 3};
    cout << f(a)[0] <<  endl;
    c_arr b = f(a);
    cout << b[0] << " " << b[1] << " " << b[2] << endl;
    return 0;
}
