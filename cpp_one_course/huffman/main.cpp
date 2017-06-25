#include<fstream>
#include<iostream>
#include<cstring>
#include"huffman_cod.h"

using namespace std;

static const uint32_t max_size_block = 40000;

int main (int argc, char *argv[]) {
    try {
        if (argc != 4) {
            if(argc == 2 && strcmp(argv[1], "help") == 0) {
                cout << "enc file1 file2 -- utility'll encode file1 and write in file2\n"
                     << "dec file1 file2 -- utility'll decode file1 and write in file2" << endl;
                return 0;
            } else
                throw runtime_error("error arguments");
        }
        if (strcmp(argv[1], "enc") == 0) {
            ifstream in(argv[2], ios::in | ios::binary);
            ofstream out(argv[3], ios::out | ios::binary);

            if(!in || !out)
                throw runtime_error("Error opening file");

            frequency_detector freq_det;
            uint8_t block[max_size_block];

            while(in) {
                memset(block, 0, max_size_block);
                in.read(reinterpret_cast<char *>(block), max_size_block);
                if (in.gcount() == 0)
                    break;
                freq_det.add_block(block, static_cast<uint32_t>(in.gcount()));
            }

            encoder enc(freq_det);

            vector<uint8_t> code_tree = enc.encode_tree();
            out.write(reinterpret_cast<char *>(code_tree.data()), code_tree.size());

            in.clear();
            in.seekg(0);

            std::vector<uint8_t> encode_block;
            while(in) {
                in.read(reinterpret_cast<char *>(block), max_size_block);
                if (in.gcount() == 0)
                    break;
                encode_block = enc.encode_block(block, (uint32_t)in.gcount());
                 out.write(reinterpret_cast<char *>(encode_block.data()), encode_block.size());
            }

        } else if (strcmp(argv[1], "dec") == 0){
            ifstream in(argv[2], ios::in | ios::binary);
            ofstream out(argv[3], ios::out | ios::binary);

            if (!in || !out) {
                throw runtime_error("Fail with files");
            }


            uint32_t size_code_tree;
            uint8_t code_tree[1000];

            in.clear();
            in.seekg(0);

            in.read(reinterpret_cast<char *>(&size_code_tree), sizeof(uint32_t));
            if (in.gcount() == 0)
                throw runtime_error("Error read");
            in.read(reinterpret_cast<char *>(code_tree), size_code_tree);
            if (in.gcount() == 0)
                throw runtime_error("Error read");
            decoder dec(code_tree, size_code_tree);

            uint32_t bitsize_block, bytesize_block;
            uint8_t code_block[max_size_block + 8];
            vector<uint8_t> decode_block;
            while(in) {
                if (in.read(reinterpret_cast<char *>(&bitsize_block), sizeof(uint32_t)).gcount() == 0)
                    break;
                bytesize_block = (bitsize_block + 7) / 8;
                memset(code_block, 0, bytesize_block);
                in.read(reinterpret_cast<char *>(code_block), bytesize_block);
                if (in.gcount() == 0)
                    throw runtime_error("Error read");
                decode_block = dec.decode_block(code_block, bitsize_block);

                out.write(reinterpret_cast<char *>(decode_block.data()), decode_block.size());
            }
        } else {
            throw runtime_error("Unkown command");
        }

    } catch (runtime_error &ex) {
        cout << ex.what() << endl;
    }
    return 0;
}
