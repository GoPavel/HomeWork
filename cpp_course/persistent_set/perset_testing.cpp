#define RANDOM_RANDOM
#include <cassert>
#include <algorithm>
#include <ctime>
#include <vector>
#include <gtest/gtest.h>

#include "persistent_set.h"

template class persistent_set<int>;

inline void random_perset(persistent_set<int> &a, size_t size_pull) {
    srand(time(0));
    std::vector<size_t> int_pull;
    for (size_t i = 0; i < size_pull; ++i) {
        int_pull.push_back(i);
    }

    std::random_shuffle(int_pull.begin(), int_pull.end());

    for (auto i: int_pull) {
        a.insert(i);
    }
}

TEST(correctness, default_constructor_dectructor) {
    persistent_set<int> a;
}

TEST(correctness, copy_empty) {
    persistent_set<int> a;
    persistent_set<int> b = a;

    persistent_set<int> c;
    c = a;
}

TEST(correctness, insert_and_size_easy) {
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
        const int size_pull = 5;
        std::vector<int> int_pull;
        for (int i = 0; i < size_pull; ++i) {
            int_pull.push_back(i);
        }

        std::random_shuffle(int_pull.begin(), int_pull.end());

        persistent_set<int> a;
        for (auto i: int_pull) {
            a.insert(i);
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

    {
        const int size_pull = 10;
        std::vector<int> int_pull;
        for (int i = 0; i < size_pull; ++i) {
            int_pull.push_back(i);
        }
        persistent_set<int> a, b[size_pull + 1];
        for (int i = 0; i < size_pull; ++i) {
            b[i] = a;
            a.insert(i);
        }
        b[size_pull] = a;

        for (int i = 0; i <= size_pull; ++i) {
            EXPECT_EQ(i, b[i].size());
        }
    }
}

TEST(correctness, insert_and_size_hard) {
#ifdef RANDOM_RANDOM
    srand(time(0));
#endif
    {
        const int size_pull = 5000;
        std::vector<int> int_pull;
        for (int i = 0; i < size_pull; ++i) {
            int_pull.push_back(i);
        }

        std::random_shuffle(int_pull.begin(), int_pull.end());

        persistent_set<int> a;
        for (auto i: int_pull) {
            a.insert(i);
        }

        EXPECT_EQ(a.size(), size_pull);
    }
    {
       const int size_pull = 5000, cnt_iter = 5;
       std::vector<int> int_pull;
       for (int i = 0; i < size_pull; ++i) {
           int_pull.push_back(i);
       }

       persistent_set<int> a;
       for (int j = 0; j < cnt_iter; ++j) {
           std::random_shuffle(int_pull.begin(), int_pull.end());
           for (auto it : int_pull) {
               a.insert(it);
           }
       }
       EXPECT_EQ(a.size(), size_pull);
    }

    {
        const int size_pull = 1000;
        std::vector<int> int_pull;
        for (int i = 0; i < size_pull; ++i) {
            int_pull.push_back(i);
        }
        persistent_set<int> a, b[size_pull + 1];
        for (int i = 0; i < size_pull; ++i) {
            b[i] = a;
            a.insert(i);
        }
        b[size_pull] = a;

        for (int i = 0; i <= size_pull; ++i) {
            EXPECT_EQ(i, b[i].size());
        }

        persistent_set<int> c;

        std::vector<int> mod;
        mod.push_back(2);
        mod.push_back(3);
        mod.push_back(7);

        std::random_shuffle(mod.begin(), mod.end());

        for (int i = 0; i <= size_pull; ++i) {
            if (i % mod[0] == 0) {
                b[i] = c;
            }
        }

        for (int i = 0; i <= size_pull; ++i) {
            if (i % mod[0] != 0) {
                EXPECT_EQ(i, b[i].size());
            }
        }

        for (int i = 0; i <= size_pull; ++i) {
            if (i % mod[1] == 0) {
                b[i] = c;
            }
        }

        for (int i = 0; i <= size_pull; ++i) {
            if (i % mod[0] != 0 && (i % mod[1] != 0)) {
                EXPECT_EQ(i, b[i].size());
            }
        }


        for (int i = 0; i <= size_pull; ++i) {
            if (i % mod[2] == 0) {
                b[i] = c;
            }
        }

        for (int i = 0; i <= size_pull; ++i) {
            if ((i % mod[2] != 0) && (i % mod[1] != 0) && (i % mod[0] != 0)) {
                EXPECT_EQ(i, b[i].size());
            }
        }

    }
}

TEST(correctness, copy_easy) {
    {
        const int size_set = 70;
        persistent_set<int> a;
        random_perset(a, size_set);
        EXPECT_EQ(size_set, a.size());

        persistent_set<int> b = a;
        EXPECT_EQ(size_set, b.size());

        b = a;
        EXPECT_EQ(size_set, b.size());

        EXPECT_EQ(a.to_string(), b.to_string());
     }
    {
        const int size_set = 100;
        persistent_set<int> a;
        random_perset(a, size_set);
        EXPECT_EQ(size_set, a.size());

        persistent_set<int> b = a;
        EXPECT_EQ(size_set, b.size());

        b = a;
        EXPECT_EQ(size_set, b.size());

        EXPECT_EQ(a.to_string(), b.to_string());
     }
    {
        const int size_set = 200;
        persistent_set<int> a;
        random_perset(a, size_set);
        EXPECT_EQ(size_set, a.size());

        persistent_set<int> b = a;
        EXPECT_EQ(size_set, b.size());

        b = a;
        EXPECT_EQ(size_set, b.size());

        EXPECT_EQ(a.to_string(), b.to_string());
     }
}

TEST(correctness, iterator) {
    {
        const int size_set = 2;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 20;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 40;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 80;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 160;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 320;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 10;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
    {
        const int size_set = 20;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
    {
        const int size_set = 40;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
    {
        const int size_set = 80;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
    {
        const int size_set = 160;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
    {
        const int size_set = 320;
        persistent_set<int> a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
}

TEST(correctness, erase_easy) {
    {
        persistent_set<int> a;
        a.insert(0);
        a.erase(a.begin());
        EXPECT_EQ(a.size(), 0);
    }
    {
        persistent_set<int> a;
        a.insert(0);
        a.insert(1);
        a.insert(2);
        a.erase(a.begin());
        EXPECT_EQ(a.size(), 2);
        a.erase(a.begin());
        EXPECT_EQ(a.size(), 1);
        a.erase(a.begin());
        EXPECT_EQ(a.size(), 0);
    }
    {
        const int size_set = 100;
        persistent_set<int> a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
}
TEST(correctness, erase_hard) {
    {
        const int size_set = 1000;
        persistent_set<int> a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 2000;
        persistent_set<int> a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 4000;
        persistent_set<int> a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 4000;
        persistent_set<int> a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 4000;
        persistent_set<int> a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 4000;
        persistent_set<int> a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
}
TEST(correctness, practice) {
    {
        const int size_set = 3000;
        persistent_set<int> a;
        std::vector<int> b;
        random_perset(a, size_set);
        for (int i = 0; i < size_set; ++i) {
            if (i % 2 == 0) {
                a.erase(a.find(i));
            } else {
                b.push_back(i);
            }
        }
        EXPECT_EQ(a.size(), size_set / 2);
        EXPECT_EQ(b.size(), size_set / 2);
        auto ptr_b = b.begin();
        for (auto ptr_a = a.begin(); ptr_a != a.end(); ++ptr_a, ++ptr_b) {
            EXPECT_EQ(*ptr_b, *ptr_a);
        }
    }
    {
        const int size_set = 7000;
        persistent_set<int> a;
        std::vector<int> b;
        random_perset(a, size_set);
        for (int i = 0; i < size_set; ++i) {
            if (i % 7 == 0) {
                a.erase(a.find(i));
            } else {
                b.push_back(i);
            }
        }
        EXPECT_EQ(a.size(), 6000);
        EXPECT_EQ(b.size(), 6000);
        auto ptr_b = b.begin();
        for (auto ptr_a = a.begin(); ptr_a != a.end(); ++ptr_a, ++ptr_b) {
            EXPECT_EQ(*ptr_b, *ptr_a);
        }
    }
}
