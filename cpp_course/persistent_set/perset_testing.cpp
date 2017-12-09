#include <cassert>
#include <gtest/gtest.h>

#include "persistent_set.h"

template class persistent_set<int>;

TEST(correctness, default_constructor_dectructor) {
    persistent_set<int> a;
}
