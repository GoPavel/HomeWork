#include <cassert>
#include <algorithm>
#include <ctime>
#include <vector>
#include <gtest/gtest.h>

#include "persistent_set.h"

template class persistent_set<int>;

const int mod = 1000000;

TEST(correctness, default_constructor_dectructor) {
    persistent_set<int> a;
}

TEST(correctness, copy_empty) {
    persistent_set<int> a;
    persistent_set<int> b = a;

    persistent_set<int> c;
    c = a;
}

TEST(correctness, insert_and_size) {
    {
        persistent_set<int> a; size_t cnt = 0;
        EXPECT_EQ(cnt, a.size());
        a.insert(1); ++cnt;
        EXPECT_EQ(cnt, a.size());
        a.insert(2); ++cnt;
        EXPECT_EQ(cnt, a.size());
        a.insert(3); ++cnt;
        EXPECT_EQ(cnt, a.size());
    }
    {
       persistent_set<int> a; size_t cnt = 0;
       a.insert(1); ++cnt;
       a.insert(1);
       EXPECT_EQ(a.size(), cnt);
    }
    {
        const int size_pull = 10;
        std::vector<int> int_pull;
        for (int i = 0; i < size_pull; ++i) {
            int_pull.push_back(i);
        }
        std::random_shuffle(int_pull.begin(), int_pull.end());
        persistent_set<int> a;
        for (auto it : int_pull) {
            a.insert(it);
        }
        EXPECT_EQ(a.size(), size_pull);
    }
   {
       const int size_pull = 10;
       std::vector<int> int_pull;
       for (int i = 0; i < size_pull; ++i) {
           int_pull.push_back(i);
       }
       for (int i = 0; i < size_pull; ++i) {
           int_pull.push_back(i);
       }
       std::random_shuffle(int_pull.begin(), int_pull.end());
       persistent_set<int> a;
       for (auto it : int_pull) {
           a.insert(it);
       }
       EXPECT_EQ(a.size(), size_pull);
   }
}
