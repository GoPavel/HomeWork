#define RANDOM_RANDOM
#include <cassert>
#include <algorithm>
#include <ctime>
#include <vector>
#include <gtest/gtest.h>

#include "persistent_set.h"
#include "smart_pointers/shared_ptr.h"
#include "smart_pointers/linked_ptr.h"

template class shared_ptr<int>;

template class linked_ptr<int>;

#define TEST_SHARED_PTR
#ifdef TEST_SHARED_PTR
TEST(correctness, my_shared_ptr) {
    {
        shared_ptr<int> a(new int(0));
        EXPECT_EQ(0, *a);
    }
    {
        shared_ptr<int> a(new int(1));
        shared_ptr<int> aa(a);
        EXPECT_EQ(1, *a);
        EXPECT_EQ(1, *aa);
    }
    {
        shared_ptr<int> a(new int(1));
        shared_ptr<int> aa(a);
        shared_ptr<int> aaa(aa);
        EXPECT_EQ(1, *a);
        EXPECT_EQ(1, *aa);
        EXPECT_EQ(1, *aaa);
    }
    {
        shared_ptr<int> a(new int(2));
        shared_ptr<int> aa(a);
        EXPECT_TRUE(a == a);
        EXPECT_TRUE(a == aa);
        EXPECT_TRUE(aa == a);
        EXPECT_TRUE(aa == aa);
        EXPECT_FALSE(a != a);
        EXPECT_FALSE(a != aa);
        EXPECT_FALSE(aa != a);
        EXPECT_FALSE(aa != aa);
        a = shared_ptr<int>(new int(21));
        EXPECT_FALSE(a == aa);
        EXPECT_TRUE(a != aa);
    }
    {
        int *ptr = new int(3);
        shared_ptr<int> a(ptr);
        EXPECT_EQ(ptr, a.get());
    }
    {
        shared_ptr<int> a;
        EXPECT_FALSE(bool(a));
        a = shared_ptr<int>(new int(4));
        EXPECT_TRUE(bool(a));
    }
    {
        shared_ptr<int> a(nullptr);
        EXPECT_FALSE(a);
    }
    {
        shared_ptr<int> a(new int(5));
        a = a;
        EXPECT_EQ(bool(a), true);
        shared_ptr<int> b(nullptr);
        b = b;
        EXPECT_EQ(bool(b), false);
    }

}
#endif
#define TEST_LINKED_PTR
#ifdef TEST_LINKED_PTR
TEST(correctness, my_linked_ptr) {
    {
        linked_ptr<int> a(new int(0));
        EXPECT_EQ(0, *a);
    }
    {
        linked_ptr<int> a(new int(1));
        linked_ptr<int> aa(a);
        EXPECT_EQ(1, *a);
        EXPECT_EQ(1, *aa);
    }
    {
        linked_ptr<int> a(new int(1));
        linked_ptr<int> aa(a);
        linked_ptr<int> aaa(aa);
        EXPECT_EQ(1, *a);
        EXPECT_EQ(1, *aa);
        EXPECT_EQ(1, *aaa);
    }
    {
        linked_ptr<int> a(new int(2));
        linked_ptr<int> aa(a);
        EXPECT_TRUE(a == a);
        EXPECT_TRUE(a == aa);
        EXPECT_TRUE(aa == a);
        EXPECT_TRUE(aa == aa);
        EXPECT_FALSE(a != a);
        EXPECT_FALSE(a != aa);
        EXPECT_FALSE(aa != a);
        EXPECT_FALSE(aa != aa);
        a = linked_ptr<int>(new int(21));
        EXPECT_FALSE(a == aa);
        EXPECT_TRUE(a != aa);
    }
    {
        int *ptr = new int(3);
        linked_ptr<int> a(ptr);
        EXPECT_EQ(ptr, a.get());
    }
    {
        linked_ptr<int> a;
        EXPECT_FALSE(bool(a));
        a = linked_ptr<int>(new int(4));
        EXPECT_TRUE(bool(a));
    }
    {
        linked_ptr<int> a(nullptr);
        EXPECT_FALSE(a);
    }
    {
        linked_ptr<int> a(new int(5));
        a = a;
        EXPECT_EQ(bool(a), true);
        linked_ptr<int> b(nullptr);
        b = b;
        EXPECT_EQ(bool(b), false);
    }
    {
        typedef linked_ptr<int> lp;
        lp a2;
        lp a0(new int(6));
        lp a1(a0);
        a2 = a1;
        lp b1(a1);
    }
    {
        typedef linked_ptr<int> lp;
        lp a2;
        {
           lp a0(new int(7));
           lp a1(a0);
           a2 = a1;
           lp b1(a1);
        }
        (*a2) = 20;
    }
}
#endif
template class persistent_set<int>;
template class persistent_set<int, shared_ptr>;
template class persistent_set<int, linked_ptr>;

#define TEST_PERSET
typedef persistent_set<int, linked_ptr> perset;

inline void random_perset(perset &a, size_t size_pull) {
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

#ifdef TEST_PERSET

TEST(correctness, default_constructor_dectructor) {
    perset a;
}

TEST(correctness, copy_empty) {
    perset a;
    perset b = a;

    perset c;
    c = a;
}

TEST(correctness, insert_and_size_easy) {
    {
        perset a; size_t cnt = 0;
        EXPECT_EQ(cnt, a.size());
        a.insert(1); ++cnt;
        EXPECT_EQ(cnt, a.size());
        a.insert(2); ++cnt;
        EXPECT_EQ(cnt, a.size());
        a.insert(3); ++cnt;
        EXPECT_EQ(cnt, a.size());
    }
    {
       perset a; size_t cnt = 0;
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

        perset a;
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
       perset a;
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
        perset a, b[size_pull + 1];
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

        perset a;
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

       perset a;
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
        perset a, b[size_pull + 1];
        for (int i = 0; i < size_pull; ++i) {
            b[i] = a;
            a.insert(i);
        }
        b[size_pull] = a;

        for (int i = 0; i <= size_pull; ++i) {
            EXPECT_EQ(i, b[i].size());
        }

        perset c;

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
        perset a;
        random_perset(a, size_set);
        EXPECT_EQ(size_set, a.size());

        perset b = a;
        EXPECT_EQ(size_set, b.size());

        b = a;
        EXPECT_EQ(size_set, b.size());

        EXPECT_EQ(a.to_string(), b.to_string());
     }
    {
        const int size_set = 100;
        perset a;
        random_perset(a, size_set);
        EXPECT_EQ(size_set, a.size());

        perset b = a;
        EXPECT_EQ(size_set, b.size());

        b = a;
        EXPECT_EQ(size_set, b.size());

        EXPECT_EQ(a.to_string(), b.to_string());
     }
    {
        const int size_set = 200;
        perset a;
        random_perset(a, size_set);
        EXPECT_EQ(size_set, a.size());

        perset b = a;
        EXPECT_EQ(size_set, b.size());

        b = a;
        EXPECT_EQ(size_set, b.size());

        EXPECT_EQ(a.to_string(), b.to_string());
     }
}

TEST(correctness, iterator) {
    {
        const int size_set = 2;
        perset a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 20;
        perset a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 40;
        perset a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 80;
        perset a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 160;
        perset a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 320;
        perset a;
        random_perset(a, size_set);
        int i = 0;
        for (auto it = a.begin(); it != a.end(); ++it) {
            EXPECT_EQ(i, *it);
            ++i;
        }
    }
    {
        const int size_set = 10;
        perset a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
    {
        const int size_set = 20;
        perset a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
    {
        const int size_set = 40;
        perset a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
    {
        const int size_set = 80;
        perset a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
    {
        const int size_set = 160;
        perset a;
        random_perset(a, size_set);
        int i = size_set;
        for (auto it = a.end(); it != a.begin(); ) {
            --it; --i;
            EXPECT_EQ(i, *it);
        }
    }
    {
        const int size_set = 320;
        perset a;
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
        perset a;
        a.insert(0);
        a.erase(a.begin());
        EXPECT_EQ(a.size(), 0);
    }
    {
        perset a;
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
        perset a;
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
        perset a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 2000;
        perset a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 4000;
        perset a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 4000;
        perset a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 4000;
        perset a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 4000;
        perset a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(a.begin());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
    {
        const int size_set = 4000;
        perset a;
        random_perset(a, size_set);
        for (int i = 1; i <= size_set; ++i) {
            a.erase(--a.end());
            EXPECT_EQ(size_set - i, a.size());
        }
    }
}
TEST(correctness, practice) {
    {
        const int size_set = 3000;
        perset a;
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
        perset a;
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
#endif
