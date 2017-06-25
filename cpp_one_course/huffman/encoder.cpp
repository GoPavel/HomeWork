#include"huffman_lib.h"
#include<map>

using namespace std;

inline void convert_bool_to_byte(vector<uint8_t>& a, vector<bool>& b){
    while(b.size() % 8 != 0)
    b.push_back(0);
    for(uint32_t i = 0; i < (b.size() / 8); i++){
        static uint8_t buf;
        buf = 0;
        for(uint32_t j = 0; j < 8; ++j) {
            buf |= (b[i * 8 + j] << j);
        }
        a.push_back(buf);
    }
}

inline void convert_uint32_to_byte(vector<uint8_t> &a, uint32_t b){
    char *p = reinterpret_cast<char *>(&b);
    for (int i = 0; i  < 4; i++) {
        a.push_back(p[i]);
    }
}

encoder::encoder(frequency_detector const& cnt) {
    multimap<uint64_t, int> freq_to_node;
    for(int i = 0; i < 256; ++i) {
        if(cnt.freq[i] != 0) {
            freq_to_node.insert(make_pair(cnt.freq[i], (int)tree.size()));
            tree.push_back(Node());
            sym.push_back(i);
        }
    }

    pair<uint64_t, int> a, b;
    while(freq_to_node.size() > 1) {
        a = (*freq_to_node.begin());
        freq_to_node.erase(freq_to_node.begin());
        b = (*freq_to_node.begin());
        freq_to_node.erase(freq_to_node.begin());
        freq_to_node.insert(make_pair(a.first + b.first, (int)tree.size()));
        tree.push_back(Node(-1, a.second, b.second));
        tree[a.second].p = tree[b.second].p = (int)tree.size()-1;
    }

    int v = tree.size() -1;
    vector<bool> code;
    while(v != -1) {
        if(tree[v].l != -1) {
            v = tree[v].l;
            code.push_back(0);
        } else {
            map_sym_to_code[sym[v]] = code;
            while(v == tree[tree[v].p].r) {
                code.pop_back();
                v = tree[v].p;
            }
            code.pop_back();
            v = tree[v].p;
            if (v != -1) {
                code.push_back(1);
                v = tree[v].r;
            }
        }
    }
}

string encoder::to_string_tree() {
    string res = "count vertex: ";
    res += to_string(tree.size());
    res += "\n vertex: v -> (p, l, r)\n";
    for (uint32_t i  = 0; i < tree.size(); ++i) {
        res += to_string(i) + " -> (" + to_string(tree[i].p) + " " +
                to_string(tree[i].l) + " " + to_string(tree[i].r) + ")";
        if (tree[i].l == -1)
            res += char((int)sym[i]);
        res += "\n";
    }
    res += "\n";
    return res;
}

//return  <steps|lists>
// we'll know cnt lists, when build tree

vector<uint8_t> encoder::encode_tree() {
    vector<bool> steps; // 0 -- D, 1 -- U
    vector<uint8_t> sym_of_lists;
    int v = tree.size() -1;
    bool move = false;
    while(v != -1) {
        if(!move) {
            if (tree[v].l != -1) {
                steps.push_back(false);
                v = tree[v].l;
            } else {
                sym_of_lists.push_back(sym[v]);
                move = true;
            }
        } else {
            if (v == tree[tree[v].p].l) {
                v = tree[v].p;
                v = tree[v].r;
                steps.push_back(true);
                move = false;
            } else {
                v = tree[v].p;
            }
        }
    }
    steps.push_back(true);
    vector<uint8_t> result_code;
    convert_uint32_to_byte(result_code, sym_of_lists.size() + (steps.size() + 7) / 8);
    convert_bool_to_byte(result_code, steps);
    for(uint32_t i = 0; i < sym_of_lists.size(); ++i) {
        result_code.push_back(sym_of_lists[i]);
    }
    return result_code;
}

vector<uint8_t> encoder::encode_block(uint8_t const* block, const uint32_t size_block) {
    vector<bool> bit_code;
    for(uint32_t i = 0; i < size_block; ++i) {
        vector<bool> &code_symbol = map_sym_to_code[block[i]];
        for(uint32_t j = 0; j < code_symbol.size(); ++j) {
            bit_code.push_back(code_symbol[j]);
        }
    }

    vector<uint8_t> result_code;
    convert_uint32_to_byte(result_code, (uint32_t)bit_code.size());
    convert_bool_to_byte(result_code, bit_code);
    return result_code;
}
