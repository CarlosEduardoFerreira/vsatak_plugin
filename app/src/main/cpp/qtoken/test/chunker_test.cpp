#include <gtest/gtest.h>

#include "chunker.hpp"
#include "test_utils.hpp"

class ChunkerBuildingFixture
    : public ::testing::TestWithParam<std::tuple<int, int>> {};

// Test encoding on entire chunks built from randomly generated files
TEST_P(ChunkerBuildingFixture, BuildChunker) {
    int file_size = std::get<0>(GetParam());
    int chunk_size = std::get<1>(GetParam());

    Chunker* ch = alloc_rand_chunker(file_size, chunk_size);
    Chunker ch_cp(*ch);

    EXPECT_TRUE(*ch == ch_cp);

    delete ch;
}

INSTANTIATE_TEST_CASE_P(
    BuildChunker, ChunkerBuildingFixture,
    ::testing::Values(std::make_tuple(7, 5), std::make_tuple(7, 10),
                      std::make_tuple(1000, 200), std::make_tuple(1953, 555),
                      std::make_tuple(5111, 67), std::make_tuple(15625, 9625)));

TEST(ChunkerTest, HandlesEmptyFile) {
    // TODO
}