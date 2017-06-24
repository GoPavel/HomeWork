#include<fstream>
#include<iostream>
#include<cstring>
#include"huffman_lib.h"

using namespace std;

static const uint32_t max_size_block = 640;

int main () {
    ifstream in("intest.txt", ios::in | ios::binary);
    ofstream out("outtest.txt", ios::in | ios::binary);

    frequency_detector freq_det;
    uint8_t block[max_size_block];

    while(in) {
        memset(block, 0, max_size_block);
        in.read((char *)block, max_size_block);
        freq_det.add_block(block, (uint32_t)in.gcount());
    }
//    cout << freq_det.to_string() << endl;

    encoder enc(freq_det);
//    cout << enc.to_string_tree() << endl;

    vector<uint8_t> code_tree = enc.encode_tree();
//    cout << "\n";
//    for (uint32_t i = 0; i < code_tree.size(); ++i) {
//        cout << (int)code_tree[i] << " ";
//    }
//    cout << "\n";
    out.write((char *)code_tree.data(), code_tree.size());

    in.clear();
    in.seekg(0);

    std::vector<uint8_t> encode_block;
    int i = 0;
    while(in) {
        in.read((char *)block, max_size_block);
        encode_block = enc.encode_block(block, (uint32_t)in.gcount());
//        cout << "Block#" << i++ << "\n";
//        for(uint32_t i = 0; i < encode_block.size(); ++i) {
//            cout << (int)encode_block[i] << " ";
//        }
//        cout << "\n";
         out.write((char *)encode_block.data(), encode_block.size());
    }

    uint32_t size_code_tree, uint8_t* code_tree[1000];
    out.read((char*) &size_code_tree, sizeof(uint32_t));
    out.read((char*) code_tree, size_code_tree);
    decoder dec(code_tree, size_code_tree);





    return 0;
}
