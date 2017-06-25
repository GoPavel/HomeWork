#include<fstream>
#include<iostream>
#include<cstring>
#include"huffman_lib.h"

using namespace std;

static const uint32_t max_size_block = 40000;

int main () {
{
    ifstream in1("orwell.txt", ios::in | ios::binary);
    ofstream out1("code.txt", ios::out | ios::binary);

    if(!in1 || !out1)
        throw runtime_error("Fail with files");

    frequency_detector freq_det;
    uint8_t block[max_size_block];

    while(in1) {
        memset(block, 0, max_size_block);
        in1.read(reinterpret_cast<char *>(block), max_size_block);
        freq_det.add_block(block, (uint32_t)in1.gcount());
    }
//    cout << "> frequency: \n"<< freq_det.to_string() << endl;

    encoder enc(freq_det);
//    cout << "> tree \n" <<  enc.to_string_tree() << endl;

    vector<uint8_t> code_tree = enc.encode_tree();
//    cout << "\n";
//    for (uint32_t i = 0; i < code_tree.size(); ++i) {
//        cout << (int)code_tree[i] << " ";
//    }
//    cout << "\n";
    out1.write(reinterpret_cast<char *>(code_tree.data()), code_tree.size());

    in1.clear();
    in1.seekg(0);

    std::vector<uint8_t> encode_block;
    while(in1) {
        in1.read(reinterpret_cast<char *>(block), max_size_block);
        encode_block = enc.encode_block(block, (uint32_t)in1.gcount());
//        static int i = 0;
//        cout << "Block#" << i++ << "\n";
//        for(uint32_t i = 0; i < encode_block.size(); ++i) {
//            cout << (int)encode_block[i] << " ";
//        }
//        cout << "\n";
         out1.write(reinterpret_cast<char *>(encode_block.data()), encode_block.size());
    }
}
    ifstream in2("code.txt", ios::in | ios::binary);
    ofstream out2("decode.txt", ios::out | ios::binary);

    if (!in2 || !out2) {
        throw runtime_error("Fail with files");
    }


    uint32_t size_code_tree;
    uint8_t code_tree[1000], byte;

//    cout << "\n<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>\n"<< "Inside decode.txt:\n";
//    while (in2) {
//        in2.read(reinterpret_cast<char *>(&byte), sizeof(uint8_t));
//        cout << (int)byte << " ";
//    }
//    cout << endl;

    in2.clear();
    in2.seekg(0);

    in2.read(reinterpret_cast<char *>(&size_code_tree), sizeof(uint32_t));
//    cout << "size_code_tree " << size_code_tree << endl;
    in2.read(reinterpret_cast<char *>(code_tree), size_code_tree);
    decoder dec(code_tree, size_code_tree);

    uint32_t bitsize_block, bytesize_block;
    uint8_t code_block[max_size_block + 8];
    vector<uint8_t> decode_block;
    while(in2) {
        if (in2.read(reinterpret_cast<char *>(&bitsize_block), sizeof(uint32_t)).gcount() == 0)
            break;
        bytesize_block = (bitsize_block + 7) / 8;
        memset(code_block, 0, bytesize_block);
        in2.read(reinterpret_cast<char *>(code_block), bytesize_block);
        decode_block = dec.decode_block(code_block, bitsize_block);

//        static int i = 0;
//        cout << "Block#" << i++ << endl;
//        for (int i = 0; i < decode_block.size() ; ++i) {
//            cout << (char)decode_block[i] << " ";
//        }
//        cout << endl;

        out2.write(reinterpret_cast<char *>(decode_block.data()), decode_block.size());
    }

    return 0;
}
