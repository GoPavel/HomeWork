#include <iostream>
#include "list_debug.h"

using namespace std;

template class list_debug<int>;

int main()
{
    cout << "Hello World!" << endl;
    list_debug<int> c;
        for (int i = 0; i < 10; ++i) c.push_back(i);
        for (int i = 1; i < 10; ++i) c.push_front(i);
        for (int i = 10; i < 20; ++i) c.insert(c.begin(), i);
        for (auto it = c.begin(); it != c.end(); ++it) {
            cout << *it << " ";
        }
        list_debug<int> p;
        for (int i = 0; i < 10; ++i) p.push_back(-1);
        cout << endl;

        auto it = c.begin();
        for (int i = 0; i < 10; ++i) it++;

        for (auto it = p.begin(); it != p.end(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
        for (auto it = c.begin(); it != c.end(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
        c.splice(it, p, p.begin(), p.end());

        cout << endl;
        for (auto it = c.begin(); it != c.end(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
    return 0;
}
