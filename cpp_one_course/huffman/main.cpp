#include<fstream>
#include<iostream>
#include<cstring>
#include"huffman_lib.h"

using namespace std;

static const uint32_t max_size_block = 4;

int main () {
    ifstream in("intest.txt", ios::in | ios::binary);
    ofstream out("outtest.txt");//, ios::in | ios::binary);

    frequency_detector freq_det;
    uint8_t block[max_size_block];

    while(in) {
        memset(block, 0, max_size_block);
        in.read((char *)block, max_size_block);
        freq_det.add_block(block, (uint32_t)in.gcount());
    }

    cout << freq_det.to_string() << endl;

    encoder enc(freq_det);

    vector<uint8_t> code_tree = enc.encode_tree();
    out.write((char *)code_tree.data(), code_tree.size());

    in.clear();
    in.seekg(0);

    std::vector<uint8_t> encode_block;
    while(in) {
        in.read((char *)block, max_size_block);
        encode_block = enc.encode_block(block, (uint32_t)in.gcount());
        out.write((char *)encode_block.data(), encode_block.size());
    }


    return 0;
}
