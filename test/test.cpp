#include <gtest/gtest.h>

#include "DynArray.h"

namespace Test{
    //------------------------------------------------------------------------------------------------------------------
    class DynArray_1D : public ::testing::Test{
    };
    TEST(DynArray_1D, T1){
        //--------------------------------------------------------------------------------------------------------------
        vla::dynarray<int> a;
        vla::dynarray<int> b(3U);
        vla::dynarray<int> c(3U, 1);
        vla::dynarray<int> d{1, 2, 3};
        vla::dynarray<int> e(d);
        vla::dynarray<int> f(std::move(e));
        vla::dynarray<int> g; g = f;
        vla::dynarray<int> h; h = std::move(f);
        //--------------------------------------------------------------------------------------------------------------
        EXPECT_EQ(b[0U], 0); EXPECT_EQ(b.at(0U), 0); //!<b(0) = 0. //!<dynarray b.
        EXPECT_EQ(b[1U], 0); EXPECT_EQ(b.at(1U), 0); //!<b(1) = 0.
        EXPECT_EQ(b[2U], 0); EXPECT_EQ(b.at(2U), 0); //!<b(2) = 0.
        EXPECT_EQ(c[0U], 1); EXPECT_EQ(c.at(0U), 1); //!<c(0) = 1. //!<dynarray c.
        EXPECT_EQ(c[1U], 1); EXPECT_EQ(c.at(1U), 1); //!<c(1) = 1.
        EXPECT_EQ(c[2U], 1); EXPECT_EQ(c.at(2U), 1); //!<c(2) = 1.
        EXPECT_EQ(d[0U], 1); EXPECT_EQ(d.at(0U), 1); //!<d(0) = 1. //!<dynarray d.
        EXPECT_EQ(d[1U], 2); EXPECT_EQ(d.at(1U), 2); //!<d(1) = 2.
        EXPECT_EQ(d[2U], 3); EXPECT_EQ(d.at(2U), 3); //!<d(2) = 3.
        EXPECT_EQ(g[0U], 1); EXPECT_EQ(g.at(0U), 1); //!<g(0) = 1. //!<dynarray g.
        EXPECT_EQ(g[1U], 2); EXPECT_EQ(g.at(1U), 2); //!<g(1) = 2.
        EXPECT_EQ(g[2U], 3); EXPECT_EQ(g.at(2U), 3); //!<g(2) = 3.
        EXPECT_EQ(h[0U], 1); EXPECT_EQ(h.at(0U), 1); //!<h(0) = 1. //!<dynarray h.
        EXPECT_EQ(h[1U], 2); EXPECT_EQ(h.at(1U), 2); //!<h(1) = 2.
        EXPECT_EQ(h[2U], 3); EXPECT_EQ(h.at(2U), 3); //!<h(2) = 3.
        //--------------------------------------------------------------------------------------------------------------
    }
    TEST(DynArray_1D, T2){
        //--------------------------------------------------------------------------------------------------------------
        vla::dynarray<int> a{1, 2, 3};
        vla::dynarray<int> b{3, 2, 1};
        vla::dynarray<int> c{3, 2, 1};
        //--------------------------------------------------------------------------------------------------------------
        EXPECT_TRUE(a != b);
        EXPECT_TRUE(b == c);
        EXPECT_TRUE(a.front() == b.back()); //!<a(0) = 1, b(2) = 1 <=> a(0) = b(2).
        EXPECT_TRUE(b.front() == a.back()); //!<b(0) = 3, a(2) = 3 <=> b(0) = a(2).
        //--------------------------------------------------------------------------------------------------------------
    }
    //------------------------------------------------------------------------------------------------------------------
    class DynArray_ND : public ::testing::Test{
    };
    //------------------------------------------------------------------------------------------------------------------
}
int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv); return RUN_ALL_TESTS();
}