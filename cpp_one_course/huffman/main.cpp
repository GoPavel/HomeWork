#include<fstream>
#include<iostream>
#include<cstring>
#include"huffman_lib.h"

using namespace std;

static const uint32_t max_size_block = 640;

int main () {
    ifstream in1("original.txt", ios::in | ios::binary);
    ofstream out1("codetest.txt", ios::out | ios::binary);

    if(!in1 || !out1)
        throw runtime_error("Fail with files");

    frequency_detector freq_det;
    uint8_t block[max_size_block];

    while(in1) {
        memset(block, 0, max_size_block);
        in1.read((char *)block, max_size_block);
        freq_det.add_block(block, (uint32_t)in1.gcount());
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
    out1.write((char *)code_tree.data(), code_tree.size());

    in1.clear();
    in1.seekg(0);

    std::vector<uint8_t> encode_block;
    int i = 0;
    while(in1) {
        in1.read((char *)block, max_size_block);
        encode_block = enc.encode_block(block, (uint32_t)in1.gcount());
//        cout << "Block#" << i++ << "\n";
//        for(uint32_t i = 0; i < encode_block.size(); ++i) {
//            cout << (int)encode_block[i] << " ";
//        }
//        cout << "\n";
         out1.write((char *)encode_block.data(), encode_block.size());
    }

    ifstream in2("code.txt", ios::in | ios::binary);
    ofstream out2("decode.txt", ios::out | ios::binary);

    if (!in2 || !out2) {
        throw runtime_errror("Fail with files");
    }


    uint32_t size_code_tree, uint8_t* code_tree[1000];
    in2.read((char*) &size_code_tree, sizeof(uint32_t));
    in2.read((char*) code_tree, size_code_tree);
    decoder dec(code_tree, size_code_tree);

    uint32_t bitsize_block, bytesize_block;
    uint8_t code_block[max_size_block];
    vector<uint8_t> decode_block;
    while(in2) {
        in2.read((char *)bitsize_block, sizeof(uint32_t));
        bytesize_block = (bitsize_block + 7) / 8;
        memset(code_block, 0, bytesize_block);
        in2.read((char *)code_block, bytesize_block);
        decode_block = dec.decode_block(code_block);
        out2.write((char *) decode_block.data(), decode_block.size());
    }



    return 0;
}
