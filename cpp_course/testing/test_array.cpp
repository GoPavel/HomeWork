#include<iostream>

using namespace std;

int sum(int (&a)[2][2] ) {
    cout << "Hi! It's good!";
    return 0;
}

int main() {
    cout <<
    "                   _                           _ _  __\n"
    " ___   _     _    (_)___    _ __ ___  _   _   | (_)/ _| ___\n"
    "/ __|_| |_ _| |_  | / __|  | '_ ` _ \\| | | |  | | | |_ / _ \\\n"
    "| (_|_   _|_   _| | \\__ \\  | | | | | | |_| |  | | |  _|  __/\n"
    "\\___| |_|   |_|   |_|___/  |_| |_| |_|\\__, |  |_|_|_|  \\___|\n"
    "                                      |___/\n";

    int arr[3][2] = {1, 2, 3, 4};
    cout << sum(arr) << '\n';

    cout << endl;
    return 0;
}
