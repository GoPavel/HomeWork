#include"huffman_lib.h"

using namespace std;

vector<bool> convert_byte_to_bool(uint8_t *code, uint32_t size_code) {
    vector<bool> bit(size_code);
    for(uint32_t i = 0; i < size_code; ++i) {
        for(uint32_t j = 0; j < 8; ++j)
            bit.push_back((code[i] >> j) & 1);
    }
    return bit;
}

decoder::decoder(uint8_t* code_tree, uint32_t size_code_tree) {
    vector<Node> tree;
    code_tree_by_bit = convert_byte_to_bool(code_tree, size_code_tree);
    tree.push_back(Node(-1, -1, -1));
    uint32_t v = 0, i = 0;
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
            if (tree[v].p == -1)
                throw "throw fail, when decoder build tree";
            if(v = tree[tree[v].p].l) {
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

    for(uint32_t i = 0; i < tree.size(); ++i) {
        if(tree[v].l == -1) {

        }
    }
}
