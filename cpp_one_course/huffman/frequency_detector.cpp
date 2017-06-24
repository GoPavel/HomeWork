#include"huffman_lib.h"

using namespace std;

frequency_detector::frequency_detector(): freq(256) {}

void frequency_detector::add_block(uint8_t const* block, const uint32_t size_block) {
    for(uint32_t i = 0; i < size_block; ++i) {
        freq[block[i]]++;
    }
    return;
}

string frequency_detector::to_string() {
    string str = "";
    for (uint32_t i = 0; i < 256; ++i) {
        if (freq[i]) {
            str += char(i);
            str += " => cnt = ";
            str += to_string(freq[i]);
            str += "\n";
        }
    }
    return str;
}
