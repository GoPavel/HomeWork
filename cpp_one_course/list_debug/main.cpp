#include <iostream>
#include "list_debug.h"
#include <vector>
#include <list>

using namespace std;

template class list_debug<int>;

template class list_debug<string>;

template class list_debug<vector<int>>;

template class list_debug<vector<string>>;

template class list_debug<list_debug<vector<string>>>;

template <typename T>
void equal(list_debug<T> const &my, list<T> const &other, string const &message) {
    cout << "equal: " << message << endl;
    typename list_debug<T>::iterator my_iter = my.begin();
    typename list<T>::iterator other_iter = other.begin();
    int cnt_iter;
    for(; other_iter != other.end(), cnt_iter = 0; ++my_iter, ++other_iter, ++cnt_iter) {
        if((*my_iter) != (*other_iter)) {
            cout << "Fail Test\n" << endl;
            cout << "cnt_iter = " << cnt_iter << endl;
            break;
        }
    }
    cout << "end equal" << endl;
}

template <typename T>
void add_val(list_debug<T> &a, T val, size_t cnt) {
    for (size_t i = 0; i < cnt; i++) {
        a.push_back(val);
    }
}

template <typename T>
bool print(list_debug<T> &a) {
    for (typename list_debug<T>::iterator it = a.begin(); it != a.end(); ++it) {
        cout << (*it) << " ";
    }
    cout << endl;
    return true;
}

template <typename T>
typename list_debug<T>::iterator &inc(typename list_debug<T>::iterator &iter, size_t cnt) {
    for (size_t i = 0; i < cnt; ++i) {
        ++iter;
    }
    return iter;
}

template <typename T>
typename list_debug<T>::iterator &dec(typename list_debug<T>::iterator &iter, size_t cnt) {
    for (size_t i = 0; i < cnt; ++i) {
        --iter;
    }
    return iter;
}

int main() {

    cout << "TEST #list\n----------------------------------------" << endl;
#define Q (10)
    cout << "Tests pop/push_back/front\n" << endl;
    cout << "Test#1" << endl;
    list_debug<int> L;
    for (int i = 0; i < Q; i++)
        L.push_back(i);
    for (int i = 0; i < Q; i++) {
        cout << L.front() << " ";
        L.pop_front();
    }
    cout << endl << endl;

    cout << "Test#2" << endl;
    for (int i = 0; i < Q; i++)
        L.push_front(i);
    for (int i = 0; i < Q; i++) {
        cout << L.back() << " ";
        L.pop_back();
    }
    cout << endl << endl;

    cout << "Test#3" << endl;
    for (int i = 0; i < Q; i++)
        L.push_back(i);
    for (int i = 0; i < Q; i++) {
        cout << L.back() << " ";
        L.pop_back();
    }
    cout << endl << endl;

    cout << "Test#4" << endl;
    for (int i = 0; i < Q; i++)
        L.push_front(i);
    for (int i = 0; i < Q; i++) {
        cout << L.front() << " ";
        L.pop_front();
    }
    cout << endl << endl;

    cout << "Test#5" << endl;
    for (int i = 0; i < Q; i++) {
        L.push_front(i);
        L.push_back(i);
    }
    for (int i = 0; i < 2 * Q; i++) {
        cout << L.front() << " ";
        L.pop_front();
    }
    cout << endl << endl;

    cout << "Test #Iterator\n" << endl;

    for (int i = 0; i < Q; i++) {
        L.push_front(i);
        L.push_back(i + Q);
    }

    for (typename list_debug<int>::iterator it = L.begin(); it != L.end(); ++it) {
        cout << (*it) << " ";
    }
    cout << endl;

    for (typename list_debug<int>::const_iterator it = L.begin(); it != L.end(); ++it) {
        cout << (*it) << " ";
    }
    cout << endl;

    for (typename list_debug<int>::iterator it = L.begin(); it != L.end(); it++) {
        cout << (*it) << " " << endl;
    }
    cout << endl;

    for (typename list_debug<int>::const_iterator it = L.begin(); it != L.end(); it++) {
        cout << (*it) << " ";
    }
    cout << endl;

    cout << endl;
    typename list_debug<int>::iterator it = L.end();
    do {
        --it;
        cout << (*it) << " ";
    } while (it != L.begin());
    cout << endl;

    cout << "Test #Reverse Iterator\n" << endl;


    for (typename list_debug<int>::reverse_iterator it = L.rbegin(); it != L.rend(); ++it) {
        cout << (*it) << " ";
    }
    cout << endl;

    for (typename list_debug<int>::const_reverse_iterator it = L.rbegin(); it != L.rend(); ++it) {
        cout << (*it) << " ";
    }
    cout << endl;

    for (typename list_debug<int>::reverse_iterator it = L.rbegin(); it != L.rend(); it++) {
        cout << (*it) << " ";
    }
    cout << endl;

    for (typename list_debug<int>::const_reverse_iterator it = L.rbegin(); it != L.rend(); it++) {
        cout << (*it) << " ";
    }
    cout << endl;

    cout << endl;
    typename list_debug<int>::reverse_iterator rit = L.rend();
    do {
        --rit;
        cout << (*rit) << " ";
    } while (rit != L.rbegin());
    cout << endl << endl;

    cout << "Test #copy/delete\n" << endl; {

    list_debug<int> a1, a2, a3;
    add_val(a1, 1, 5); print(a1);
    add_val(a2, 2, 5); print(a2);
    add_val(a3, 3, 5); print(a3);
    list_debug<int> b1 = a1;
    cout << "b1(a1) = "; print(b1);
    b1 = a2;
    cout << "b1 = a1 = "; print(b1);
    cout << "for string" << endl;
{
    list_debug<string> a1, b2;
    add_val(a1, string("str"), 7);
    list_debug<string> b1(a1);
    print(b1);
    b1.push_back("Hello");
    b1.push_back(",");
    b1.push_back("world");
    b1.push_back("!");
    b2 = b1;
    print(b2);
}

{
    list_debug<int> a1;
    add_val(a1, 5, 5);
    auto iter = a1.begin();
}
    cout << endl << endl;

}

    cout << "Test #insert/erase" << endl;
    list_debug<int> r;
    r.insert(r.begin(), 3);
    r.insert(r.begin(), 2);
    r.insert(r.begin(), 1);
    r.insert(r.end(), 4);
    r.insert(r.end(), 5);
    r.insert(r.end(), 6);
    print(r);
    auto it1 = r.begin(), it2 = --r.end();
    cout << "it1 = " << *it1 << " it2 = " << *it2 << endl;
    swap(it1, it2); cout << "swap\n";
    cout << "it1 = " << *it1 << " it2 = " << *it2 << endl;

    list_debug<int> q;
    q.insert(q.begin(), 0);
    int j = 0;
    for (auto it1 = q.begin(), it2 = q.begin(); j < 100; j += 10) {
        it2 = it1;
        q.insert(it1, j + 10);
        --it2;
        for (int i = 9; i >= 1; --i) {
            q.insert(it1, i + j);
        }
        swap(it1, it2);
    }
    print(q);

    typedef typename list_debug<int>::iterator iter;
    {
      iter it = q.begin();
      q.erase(it);
    }
    print(q);

    q.push_front(100);
    iter it3 = q.begin();
    for(int i = 0; i < 50; ++i) {
        iter temp(it3);
        ++++it3;
        q.erase(temp);
    }
    print(q);
    cout << endl;

    cout << "Test #splice\n" <<  endl; {

        cout << "begin" << endl;
        list_debug<int> a, b, c;
        add_val(a, 1, 5);
        add_val(b, 2, 5);
        add_val(c, 3, 5);
        cout << "a: "; print(a);
        cout << "b: "; print(b);
        cout << "c: "; print(c);
        cout << endl;

        cout << "b.splice(b.begin(), a, a.begin(), a.end());" << endl;
        b.splice(b.begin(), a, a.begin(), a.end());
        cout << "a: "; print(a);
        cout << "b: "; print(b);
        cout << "c: "; print(c);
        cout << endl;

        cout << "b.splice(b.end(), c, c.begin(), c.end());" << endl;
        b.splice(b.end(), c, c.begin(), c.end());
        cout << "a: "; print(a);
        cout << "b: "; print(b);
        cout << "c: "; print(c);
        cout << endl;

        cout << "a.splice(a.begin(), b, b.begin(), b.end());" << endl;
        a.splice(a.begin(), b, b.begin(), b.end());
        cout << "a: "; print(a);
        cout << "b: "; print(b);
        cout << "c: "; print(c);
        cout << endl;
{
        list_debug<int> a;
        typename list_debug<int>::iterator g = a.begin();
//        typename list_debug<int>::iterator &h = g;
}
        auto it1 = a.begin();
        auto it2 = a.end();

        cout << "b.splice(b.begin(), a, inc<int>(a.begin(), 5), dec<int>(a.end(), 5));" << endl;
        b.splice(b.begin(), a, inc<int>(it1, 5), dec<int>(it2, 5));
        cout << "a: "; print(a);
        cout << "b: "; print(b);
        cout << "c: "; print(c);
        cout << endl;

        cout << "c.splice(c.begin(), a, inc<int>(a.begin(), 5), a.end());" << endl;
        it3 = a.begin();
        c.splice(c.begin(), a, inc<int>(it3, 5), a.end());
        cout << "a: "; print(a);
        cout << "b: "; print(b);
        cout << "c: "; print(c);
        cout << endl;
    }



    cout << endl;
    cout << "END TEST" << endl;
    return 0;
}
