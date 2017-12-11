#include <cassert>
#include <gtest/gtest.h>

#include "persistent_set.h"

template class persistent_set<int>;

TEST(correctness, default_constructor_dectructor) {
    persistent_set<int> a;
}
<<<<<<< HEAD

TEST(correctness, copy_empty) {
    persistent_set<int> a;
    persistent_set<int> b = a;

    persistent_set<int> c;
    c = a;
}

TEST(correctness, insert_and_size) {
    persistent_set<int> a; int cnt = 0;
    EXPECT_EQ(1, a.size());
    a.insert(1);
}
=======
>>>>>>> origin/master
