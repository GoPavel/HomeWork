#ifndef _HUFFMAN_LIB_H_
#define _HUFFMAN_LIB_H_
#include<vector>
#include<cstdint>
#include<string>

class frequency_detector{
    friend class encoder;
    std::vector<uint64_t> freq;
public:
    frequency_detector();
    void add_block(uint8_t const* block, const uint32_t size_block);
    std::string to_string();
};

class encoder {
    struct Node {
        int p, l, r;
        Node(int _p, int _l, int _r):
            p(_p), l(_l), r(_r) {}
        Node(): p(-1), l(-1), r(-1) {}
    };
    std::vector<Node> tree; // <lists/some/root>
    std::vector<uint8_t> sym;
    std::vector<bool> map_sym_to_code[256];
public:
    std::string to_string_tree();
    encoder(frequency_detector const& cnt);
    std::vector<uint8_t> encode_tree();
    std::vector<uint8_t> encode_block(uint8_t const* block, const uint32_t size_block);
};

class decoder {

};

#endif
