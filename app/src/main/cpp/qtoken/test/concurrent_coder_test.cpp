#include <gtest/gtest.h>

#include "concurrent_decoder.hpp"
#include "concurrent_encoder.hpp"
#include "test_utils.hpp"

// Encoded frames can not always be decoded if more than 1 info frame is
// overlaid
std::vector<int> conc_overlay_tests = {1};
// The number of input frames must be a multiple of the number of frames to
// be overlaid
std::vector<int> conc_test_frames = {1, 20, 90};
std::vector<std::pair<int, int>> conc_K_N_pairs = {{4, 32}, {8, 512}};

class ConcurrentEncoderChunkerFixture
    : public ::testing::TestWithParam<std::tuple<int, int>> {};

class ConcurrentEncoderBitlistFixture
    : public ::testing::TestWithParam<
          std::tuple<int, std::pair<int, int>, int>> {};

// Test encoding on a list of bits
TEST_P(ConcurrentEncoderBitlistFixture, ConcurrentEncodingBitList) {
    int frames = std::get<0>(GetParam());
    std::pair<int, int> K_N = std::get<1>(GetParam());
    int overlaid_frames = std::get<2>(GetParam());

    // Create randomized bit list
    std::vector<int> rand_bits(frames * K_N.first);
    std::generate(rand_bits.begin(), rand_bits.end(), std::rand);
    std::for_each(rand_bits.begin(), rand_bits.end(),
                  [](int& n) { n = n % 2; });

    Bitlist info;
    for (auto bit : rand_bits) {
        info.push_back(bit);
    }

    // Instantiate encoder/decoder
    ConcurrentEncoder encoder(overlaid_frames, frames, K_N.second, K_N.first);
    ConcurrentDecoder decoder(overlaid_frames, frames, K_N.second, K_N.first);

    // Encode step
    Bitlist output(encoder.encode(info));
    // Decode step
    Bitlist decoded(decoder.decode(output));
    // Assert here
    EXPECT_EQ(info, decoded);
}

INSTANTIATE_TEST_CASE_P(
    ConcurrentEncodingBitList, ConcurrentEncoderBitlistFixture,
    ::testing::Combine(::testing::ValuesIn(conc_test_frames),
                       ::testing::ValuesIn(conc_K_N_pairs),
                       ::testing::ValuesIn(conc_overlay_tests)));

// Test encoding on entire chunks built from randomly generated files
TEST_P(ConcurrentEncoderChunkerFixture, ConcurrentEncodingChunker) {
    int file_size = std::get<0>(GetParam());
    int chunk_size = std::get<1>(GetParam());

    Chunker* ch = alloc_rand_chunker(file_size, chunk_size);
    Chunker ch_cp(*ch);

    // Create crypto receipt
    CryptoReceipt* cr = new CryptoReceipt();

    // Encode original file
    ConcurrentEncoder encoder;
    Writer w(ch, cr);
    ChunkOperator cencoder = encoder.get_coder();
    cencoder(w);

    // Encoding sanity check
    EXPECT_TRUE(!(*ch == ch_cp));

    // Decode original file
    ConcurrentDecoder decoder;
    ChunkOperator cdecoder = decoder.get_coder();
    cdecoder(w);

    // Check decoding
    EXPECT_TRUE(*ch == ch_cp);

    delete cr;
    delete ch;
}

INSTANTIATE_TEST_CASE_P(ConcurrentEncodingChunker,
                        ConcurrentEncoderChunkerFixture,
                        ::testing::Values(std::make_tuple(13, 5),
                                          std::make_tuple(400, 51),
                                          std::make_tuple(1000, 200)));
