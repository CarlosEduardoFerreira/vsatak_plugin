#include <gtest/gtest.h>

#include "entanglement_decoder.hpp"
#include "entanglement_encoder.hpp"
#include "test_utils.hpp"

class EntanglementEncoderChunkerFixture
    : public ::testing::TestWithParam<std::tuple<int, int>> {};

// Test encoding on entire chunks built from randomly generated files
TEST_P(EntanglementEncoderChunkerFixture, EntanglementEncodingChunker) {
    int file_size = std::get<0>(GetParam());
    int chunk_size = std::get<1>(GetParam());

    Chunker* ch = alloc_rand_chunker(file_size, chunk_size);
    Chunker ch_cp(*ch);

    // Create crypto receipt
    CryptoReceipt* cr = new CryptoReceipt();

    // Encode original file
    EntanglementEncoder encoder;
    Writer w(ch, cr);
    ChunkOperator cencoder = encoder.get_coder();
    cencoder(w);

    // Encoding sanity check
    EXPECT_TRUE(!(*ch == ch_cp));

    // Decode original file
    EntanglementDecoder decoder;
    ChunkOperator cdecoder = decoder.get_coder();
    cdecoder(w);

    // Check decoding
    EXPECT_TRUE(*ch == ch_cp);

    delete cr;
    delete ch;
}

INSTANTIATE_TEST_CASE_P(EntanglementEncodingChunker,
                        EntanglementEncoderChunkerFixture,
                        ::testing::Values(std::make_tuple(13, 5),
                                          std::make_tuple(1953, 200),
                                          std::make_tuple(15625, 9625)));
