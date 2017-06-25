#include"huffman_lib.h"

using namespace std;

vector<bool> convert_byte_to_bool(uint8_t const *code, uint32_t size_code) {
    vector<bool> bit;
    bit.reserve(size_code);
    for(uint32_t i = 0; i < size_code; ++i) {
        for(uint32_t j = 0; j < 8; ++j)
            bit.push_back((code[i] >> j) & 1);
    }
    return bit;
}

decoder::decoder(uint8_t const* code_tree, uint32_t const size_code_tree) {
    vector<bool> code_tree_by_bit = convert_byte_to_bool(code_tree, size_code_tree);
    tree.push_back(Node(-1, -1, -1));
    int v = 0; uint32_t i = 0;
    bool move = false;
    while(v != -1) {
        if(!move) {
            if(code_tree_by_bit[i] == 0) {
                tree.push_back(Node(v, -1, -1));
                tree[v].l = tree.size()-1;
                v = tree[v].l;
            } else {
                move = true;
            }
            ++i;
        } else {
            if (tree[v].p == -1) {
                break;
//                throw  runtime_error("Building huffman's tree is impossible, garbage in file");
            }
            if(v == tree[tree[v].p].l) {
                v = tree[v].p;
                tree.push_back(Node(v, -1, -1));
                tree[v].r = tree.size()-1;
                v = tree[v].r;
                move = false;
            } else {
                v = tree[v].p;
            }
        }
    }

    while (i % 8 != 0)
        ++i;
    i = i / 8;

    vector<uint8_t> lists;
    for(; i < size_code_tree; ++i) {
        lists.push_back(code_tree[i]);
    }

    if (tree.size() > 600)
        throw runtime_error("Huffman's tree is large too, garbage in file");

    sym_of_node.resize(tree.size());
    for(uint32_t i = 0, j = 0; i < tree.size(); ++i) {
        if(tree[i].l == -1) {
            sym_of_node[i] = lists[j++];
        }
    }
}

vector<uint8_t> decoder::decode_block(uint8_t const* code_block, const uint32_t bitsize_block) {
    vector<uint8_t> decode_block;
    vector<bool> bitcode_block = convert_byte_to_bool(code_block, (bitsize_block + 7) / 8);
    uint32_t v = 0;
    for (uint32_t i = 0; i < bitsize_block; ++i) {
        if (bitcode_block[i] == 0)
            v = tree[v].l;
        else
            v = tree[v].r;
        if (tree[v].l == -1) {
            decode_block.push_back(sym_of_node[v]);
            v = 0;
        }
    }
    return decode_block;
}
