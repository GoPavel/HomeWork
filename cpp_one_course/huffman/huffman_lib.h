#ifndef _HUFFMAN_LIB_H_
#define _HUFFMAN_LIB_H_
#include<vector>
#include<cstdint>
#include<string>
#include<stdexcept>

using namespace std;

class frequency_detector{
    friend class encoder;
    vector<uint64_t> freq;
public:
    frequency_detector();
    void add_block(uint8_t const* block, const uint32_t size_block);
    string to_string();
};

struct Node {
    friend class encoder;
    friend class decoder;
    int p, l, r;
private:
    Node(int _p, int _l, int _r): p(_p), l(_l), r(_r) {}
    Node(): p(-1), l(-1), r(-1) {}
};

class encoder {
    vector<Node> tree; // <lists/some/root>
    vector<uint8_t> sym;
    vector<bool> map_sym_to_code[256];
public:
    string to_string_tree();
    encoder(frequency_detector const& cnt);
    vector<uint8_t> encode_tree();
    vector<uint8_t> encode_block(uint8_t const* block, const uint32_t bytesize_block);
};

class decoder {
    vector<Node> tree;
    vector<uint8_t> sym_of_node;
public:
   decoder(uint8_t const* byte_code, uint32_t const size_code);
   vector<uint8_t> decode_block(uint8_t const* code_block, const uint32_t bitsize_code);
};
#endif
