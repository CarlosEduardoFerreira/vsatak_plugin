#include <gtest/gtest.h>

#include "lfsr.hpp"

TEST(LFSRTest, generates_5_bit_till_reset) {
    uint8_t curr_state = 0;
    uint8_t seed = 0x22;
    std::unordered_map<int, int> m;
    LFSR l(seed);

    // Generate map m:(0, P-1) -> (1, P-1) where P is total
    // count of all 5 bit binary sequences
    int i = 1;
    m[int(seed & 31)] = 0;
    while (curr_state != LFSR_RESET || i == 1) {
        curr_state = l.next5();
        m[int(curr_state)] = i;
        i++;
    }

    ASSERT_EQ(m.size(), 32);
}

class LFSRHashTableFixture : public ::testing::TestWithParam<int> {};

// Build variable size hash tables by concatenating
// a psuedo random table of size 32
TEST_P(LFSRHashTableFixture, LFSRTest) {
    int size = GetParam();
    uint8_t seed = 0x22;
    LFSR l(seed);

    std::unordered_map<int, int> hash = l.build_map(size);

    ASSERT_EQ(hash.size(), size);
}

INSTANTIATE_TEST_CASE_P(LFSRTest, LFSRHashTableFixture,
                        ::testing::Values(32, 64, 128));
