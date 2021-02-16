#include <gtest/gtest.h>

#include "polar_decoder.hpp"
#include "polar_encoder.hpp"
#include "test_utils.hpp"

std::vector<int> polar_test_frames = {5, 99};
std::vector<std::pair<int, int>> polar_K_N_pairs = {{32, 128}, {64, 128}};

class PolarBitlistEncodingFixture
    : public ::testing::TestWithParam<std::tuple<int, std::pair<int, int>>> {};

class PolarEncoderChunkerFixture
    : public ::testing::TestWithParam<std::tuple<int, int>> {};

// Test encoding on a list of bits (std::vector<int>)
TEST_P(PolarBitlistEncodingFixture, PolarEncodingBitList) {
    int frames = std::get<0>(GetParam());
    std::pair<int, int> K_N = std::get<1>(GetParam());

    // Construct a random info vector for testing
    std::vector<int> info(K_N.first * frames);
    std::generate(info.begin(), info.end(), std::rand);
    std::for_each(info.begin(), info.end(), [](int& n) { n = n % 2; });

    PolarEncoder encoder(frames, K_N.second, K_N.first);
    PolarDecoder decoder(frames, K_N.second, K_N.first);

    std::vector<int> enc(encoder.encode(info));
    std::vector<int> dec(decoder.decode(enc));

    EXPECT_EQ(info, dec);
    EXPECT_TRUE(info != enc);
}

INSTANTIATE_TEST_CASE_P(
    PolarEncodingBitList, PolarBitlistEncodingFixture,
    ::testing::Combine(::testing::ValuesIn(polar_test_frames),
                       ::testing::ValuesIn(polar_K_N_pairs)));

// Test encoding on entire chunks built from randomly generated files
TEST_P(PolarEncoderChunkerFixture, PolarEncodingChunker) {
    int file_size = std::get<0>(GetParam());
    int chunk_size = std::get<1>(GetParam());

    Chunker* ch = alloc_rand_chunker(file_size, chunk_size);
    Chunker ch_cp(*ch);

    // Create crypto receipt
    CryptoReceipt* cr = new CryptoReceipt();

    // Encode original file
    PolarEncoder encoder;
    Writer w(ch, cr);
    ChunkOperator pencoder = encoder.get_coder();
    pencoder(w);

    // Encoding sanity check
    EXPECT_TRUE(!(*ch == ch_cp));

    // Decode original file
    PolarDecoder decoder;
    ChunkOperator pdecoder = decoder.get_coder();
    pdecoder(w);

    // Check decoding
    EXPECT_TRUE(*ch == ch_cp);

    delete cr;
    delete ch;
}

INSTANTIATE_TEST_CASE_P(PolarEncodingChunker, PolarEncoderChunkerFixture,
                        ::testing::Values(std::make_tuple(13, 5),
                                          std::make_tuple(1953, 200),
                                          std::make_tuple(15625, 9625)));
