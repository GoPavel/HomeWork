#include <iostream>
#include "list_debug.h"

using namespace std;

template class list<int>;

int main()
{
    cout << "Hello World!" << endl;
    int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, cnt = 10;
    list<int> L;
    for (int i = 0; i < cnt; i++) {
        L.push_back(a[i]);
    }
    for (int i = 0; i < cnt; i++) {
        cout << L.back() << " ";
        L.pop_back();
    }
    cout << endl;
    for (int i = 0; i < cnt; i++) {
        L.push_front(a[i]);
    }
    for (int i = 0; i < cnt; i++) {
        cout << L.front() << " ";
        L.pop_front();
    }
    cout << endl;
    return 0;
}
