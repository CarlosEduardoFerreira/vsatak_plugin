#include <gtest/gtest.h>

#include "polar_fbg.hpp"

class FBGFixture : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(FBGFixture, GenerateFrozenBits) {
    int K = std::get<0>(GetParam());
    int N = std::get<1>(GetParam());

    Qtoken::PolarFBG frozen_bits(N, K);
    frozen_bits.generateFrozenBits();
    std::vector<bool> bits(frozen_bits.getFrozenBits());

    // Ensure frozen bit vector is of the correct size
    EXPECT_EQ(bits.size(), N);

    // Ensure proper number of frozen bits are set
    int count = std::count(bits.begin(), bits.end(), true);
    EXPECT_EQ(count, N - K);
}

INSTANTIATE_TEST_CASE_P(GenerateFrozenBits, FBGFixture,
                        ::testing::Values(std::make_tuple(32, 128),
                                          std::make_tuple(32, 64),
                                          std::make_tuple(64, 128)));
