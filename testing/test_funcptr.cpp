#include<iostream>
#include<vector>

using namespace std;

// typedef int bin_oper(int, int);
using bin_oper = int(*)(int, int);


int sum(int a, int b) {
    return a + b;
}

int mul(int a, int b) {
    return a * b;
}

// int (*selector(bool flag))(int a, int b) {
//     if(flag)
//         return mul;
//     else return sum;
// }

bin_oper selector(bool flag) {
    if (flag)
        return mul;
    else return sum;
}

int oper(vector<int> a, bin_oper f) {
    if (a.size() > 1) {
        int res = f(a[0], a[1]);
        for(size_t i = 2; i < a.size(); ++i) {
            res = f(res, a[i]);
        }
        return res;
    }
    return 666;
}
int main() {
cout <<
"                   _                           _ _  __\n"
" ___   _     _    (_)___    _ __ ___  _   _   | (_)/ _| ___\n"
"/ __|_| |_ _| |_  | / __|  | '_ ` _ \\| | | |  | | | |_ / _ \\\n"
"| (_|_   _|_   _| | \\__ \\  | | | | | | |_| |  | | |  _|  __/\n"
"\\___| |_|   |_|   |_|___/  |_| |_| |_|\\__, |  |_|_|_|  \\___|\n"
"                                      |___/\n";

    vector<int> arr; bool flag;
    cout << "0 -- sum, 1 -- mul\n";
    cin >> flag;
    for(int buf;;) {
        cin >> buf;
        if (!buf) break;
        arr.push_back(buf);
    }
    cout << oper(arr, selector(flag)) << endl;
    cout << endl;
    return 0;
}
