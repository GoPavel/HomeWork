#include<iostream>

using namespace std;

struct St {
    int val;
    St(int a): val(a) {}
    St operator+=(St a) {
        val += a.val;
        return (*this);
    }
};

const St f(St a, St b) {
    return  a.val + b.val;
}

int main() {
    cout <<
    "                   _                           _ _  __\n"
    " ___   _     _    (_)___    _ __ ___  _   _   | (_)/ _| ___\n"
    "/ __|_| |_ _| |_  | / __|  | '_ ` _ \\| | | |  | | | |_ / _ \\\n"
    "| (_|_   _|_   _| | \\__ \\  | | | | | | |_| |  | | |  _|  __/\n"
    "\\___| |_|   |_|   |_|___/  |_| |_| |_|\\__, |  |_|_|_|  \\___|\n"
    "                                      |___/\n";
    cout << (f(St(2), St(2)) += St(1)).val;

    cout << endl;
    return 0;
}
