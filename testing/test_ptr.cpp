#include<iostream>

using namespace std;

int main() {
    cout <<
    "                   _                           _ _  __\n"
    " ___   _     _    (_)___    _ __ ___  _   _   | (_)/ _| ___\n"
    "/ __|_| |_ _| |_  | / __|  | '_ ` _ \\| | | |  | | | |_ / _ \\\n"
    "| (_|_   _|_   _| | \\__ \\  | | | | | | |_| |  | | |  _|  __/\n"
    "\\___| |_|   |_|   |_|___/  |_| |_| |_|\\__, |  |_|_|_|  \\___|\n"
    "                                      |___/\n";

    int a[5] = {1, 2, 3, 4, 5};
    int *p1 = a;
    int *p2 = a + 4;
    cout << "array: ";
    for (int i = 0; i < 5; ++i)
        cout << a[i] << " ";
    cout << "\n";
    cout << "p1: " << *p1 << "\n";
    cout << "p2: " << *p2 << "\n";

    cout << (p2 - p1) << "\n";

    cout << endl;
    return 0;
}
