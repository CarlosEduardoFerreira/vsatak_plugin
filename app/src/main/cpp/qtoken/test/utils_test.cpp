#include <gtest/gtest.h>
#include <vector>

#include "utils.hpp"

class UtilsTestFixture : public ::testing::TestWithParam<int> {};

// FIXME: This can be more comprehensive
// Maybe a test fixture which takes types?
TEST_P(UtilsTestFixture, BitByteConversionTests) {
    int size = GetParam();
    std::vector<char> in_bytes(size);

    std::srand(std::time(nullptr));
    for (int i = 0; i < size; i++) {
        in_bytes[i] = char(std::rand());
    }
    std::vector<char> out_bytes(bits_to_bytes<std::vector<int>>(
        bytes_to_bits<std::vector<int>>(in_bytes)));

    EXPECT_TRUE(in_bytes == out_bytes);
}

INSTANTIATE_TEST_SUITE_P(BitByteConversionTests, UtilsTestFixture,
                         ::testing::Values(1, 2, 24, 512, 1025));

TEST(UtilsTest, SHA256Generation) {
    char ch[6] = "hello";
    std::string hash = sha256(ch);

    ASSERT_EQ(
        hash,
        "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824");
}

TEST(UtilsTest, SHA256GenerationNullChars) {
    char ch[10] = {'h', 'e', 'l', 'l', 'o', 0, 1, 'i', 'd', 'd'};

    auto hash = sha256(ch);

    ASSERT_EQ(
        hash,
        "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824");
}

TEST(UtilsTest, XORByteArraysGreater) {
    char a[11] = "hello big ";
    char b[6] = "world";
    std::vector<char> a_v(a, a + 11);
    std::vector<char> b_v(b, b + 6);

    std::vector<char> output = XOR_bytes(a_v, b_v);

    // hand calculated
    ASSERT_EQ(output[0], 31);
    ASSERT_EQ(output[1], 10);
    ASSERT_EQ(output[2], 30);
    ASSERT_EQ(output[3], 0);
    ASSERT_EQ(output[4], 11);
    ASSERT_EQ(output[5], ' ');
    ASSERT_EQ(output[6], 'b');
    ASSERT_EQ(output[7], 'i');
    ASSERT_EQ(output[8], 'g');
    ASSERT_EQ(output[9], ' ');
}

TEST(UtilsTest, XORByteArraysLesser) {
    char a[6] = "hello";
    char b[11] = " big world";
    std::vector<char> a_v(a, a + 6);
    std::vector<char> b_v(b, b + 11);

    std::vector<char> output = XOR_bytes(a_v, b_v);

    // hand calculated
    ASSERT_EQ(output[0], 72);
    ASSERT_EQ(output[1], 7);
    ASSERT_EQ(output[2], 5);
    ASSERT_EQ(output[3], 11);
    ASSERT_EQ(output[4], 79);
    ASSERT_EQ(output[5], 'w');
    ASSERT_EQ(output[6], 'o');
    ASSERT_EQ(output[7], 'r');
    ASSERT_EQ(output[8], 'l');
    ASSERT_EQ(output[9], 'd');
}

TEST(UtilsTest, XORByteArraysEqual) {
    char a[6] = "hello";
    char b[6] = "world";
    std::vector<char> a_v(a, a + 6);
    std::vector<char> b_v(b, b + 6);

    std::vector<char> output = XOR_bytes(a_v, b_v);

    // hand calculated
    ASSERT_EQ(output[0], 31);
    ASSERT_EQ(output[1], 10);
    ASSERT_EQ(output[2], 30);
    ASSERT_EQ(output[3], 0);
    ASSERT_EQ(output[4], 11);
}
