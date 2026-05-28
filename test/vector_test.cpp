#include <gtest/gtest.h>
#include "Vector.h"

// =============================================================
// commit #12: Vector<T> vienetų testai
// =============================================================

// --- Konstruktoriai ---

TEST(VectorKonstruktoriai, Tuščias) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorKonstruktoriai, SuDydžiu) {
    Vector<int> v(5, 42);
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < 5; ++i)
        EXPECT_EQ(v[i], 42);
}

TEST(VectorKonstruktoriai, InitializerList) {
    Vector<int> v = {1, 2, 3, 4, 5};
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorKonstruktoriai, Copy) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = a;
    EXPECT_EQ(b.size(), 3);
    EXPECT_EQ(b[0], 1);
    // nepriklausomi
    b[0] = 99;
    EXPECT_EQ(a[0], 1);
}

TEST(VectorKonstruktoriai, Move) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = std::move(a);
    EXPECT_EQ(b.size(), 3);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(a.size(), 0);
    EXPECT_EQ(a.data(), nullptr);
}

TEST(VectorKonstruktoriai, CopyOperator) {
    Vector<int> a = {10, 20};
    Vector<int> b;
    b = a;
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[1], 20);
    b[0] = 99;
    EXPECT_EQ(a[0], 10);
}

TEST(VectorKonstruktoriai, MoveOperator) {
    Vector<int> a = {10, 20};
    Vector<int> b;
    b = std::move(a);
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(a.size(), 0);
}

// --- Capacity ---

TEST(VectorCapacity, Reserve) {
    Vector<int> v;
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100);
    EXPECT_EQ(v.size(), 0);
}

TEST(VectorCapacity, ReserveNemažinaCapacity) {
    Vector<int> v;
    v.reserve(100);
    v.reserve(10);
    EXPECT_GE(v.capacity(), 100);
}

TEST(VectorCapacity, ShrinkToFit) {
    Vector<int> v;
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 2);
}

TEST(VectorCapacity, ShrinkToFitTuščias) {
    Vector<int> v;
    v.reserve(50);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.data(), nullptr);
}

// --- Element access ---

TEST(VectorAccess, OperatorBracket) {
    Vector<int> v = {10, 20, 30};
    EXPECT_EQ(v[0], 10);
    v[1] = 99;
    EXPECT_EQ(v[1], 99);
}

TEST(VectorAccess, At) {
    Vector<int> v = {10, 20, 30};
    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(2), 30);
}

TEST(VectorAccess, AtIšmetaException) {
    Vector<int> v = {1, 2, 3};
    EXPECT_THROW(v.at(10), std::out_of_range);
}

TEST(VectorAccess, FrontBack) {
    Vector<int> v = {5, 6, 7};
    EXPECT_EQ(v.front(), 5);
    EXPECT_EQ(v.back(), 7);
}

TEST(VectorAccess, Data) {
    Vector<int> v = {1, 2, 3};
    int* p = v.data();
    EXPECT_EQ(p[0], 1);
    EXPECT_EQ(p[2], 3);
}

// --- Modifiers ---

TEST(VectorModifiers, PushBack) {
    Vector<int> v;
    for (int i = 0; i < 10; ++i)
        v.push_back(i);
    EXPECT_EQ(v.size(), 10);
    EXPECT_EQ(v[9], 9);
}

TEST(VectorModifiers, PopBack) {
    Vector<int> v = {1, 2, 3};
    v.pop_back();
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), 2);
}

TEST(VectorModifiers, Clear) {
    Vector<int> v = {1, 2, 3};
    v.clear();
    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorModifiers, InsertViduryje) {
    Vector<int> v = {1, 2, 4, 5};
    v.insert(v.begin() + 2, 3);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
}

TEST(VectorModifiers, EraseVienas) {
    Vector<int> v = {1, 2, 3, 4};
    v.erase(v.begin() + 1);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[1], 3);
}

TEST(VectorModifiers, EraseRangas) {
    Vector<int> v = {1, 2, 3, 4, 5};
    v.erase(v.begin() + 1, v.begin() + 3);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[1], 4);
}

TEST(VectorModifiers, Resize) {
    Vector<int> v = {1, 2, 3};
    v.resize(5, 99);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[3], 99);
    EXPECT_EQ(v[4], 99);
    v.resize(2);
    EXPECT_EQ(v.size(), 2);
}

TEST(VectorModifiers, EmplaceBack) {
    Vector<std::string> v;
    v.emplace_back("hello");
    v.emplace_back("world");
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "hello");
}

TEST(VectorModifiers, Swap) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {4, 5};
    a.swap(b);
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], 4);
    EXPECT_EQ(b.size(), 3);
    EXPECT_EQ(b[0], 1);
}

// --- Iteratoriai ---

TEST(VectorIteratoriai, RangeFor) {
    Vector<int> v = {1, 2, 3, 4, 5};
    int sum = 0;
    for (const auto& x : v) sum += x;
    EXPECT_EQ(sum, 15);
}

TEST(VectorIteratoriai, StdSort) {
    Vector<int> v = {5, 3, 1, 4, 2};
    std::sort(v.begin(), v.end());
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorIteratoriai, StdFind) {
    Vector<int> v = {10, 20, 30};
    auto it = std::find(v.begin(), v.end(), 20);
    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 20);
}

TEST(VectorIteratoriai, Cbegin) {
    const Vector<int> v = {1, 2, 3};
    auto it = v.cbegin();
    EXPECT_EQ(*it, 1);
}

// --- Palyginimo operatoriai ---

TEST(VectorOperatoriai, Lygybė) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {1, 2, 3};
    Vector<int> c = {1, 2, 4};
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(VectorOperatoriai, Nelygybė) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {1, 2, 4};
    EXPECT_TRUE(a != b);
}

TEST(VectorOperatoriai, MažiauUž) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {1, 2, 4};
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

// --- Capacity doubling ---

TEST(VectorCapacity, DoubleStrategy) {
    Vector<int> v;
    v.push_back(1);
    EXPECT_EQ(v.capacity(), 1);
    v.push_back(2);
    EXPECT_EQ(v.capacity(), 2);
    v.push_back(3);
    EXPECT_EQ(v.capacity(), 4);
    v.push_back(4);
    v.push_back(5);
    EXPECT_EQ(v.capacity(), 8);
}
