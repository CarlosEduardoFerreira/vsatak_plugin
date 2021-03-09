#include <gtest/gtest.h>

#include "concurrent_decoder.hpp"
#include "concurrent_encoder.hpp"
#include "decoder.hpp"
#include "encoder.hpp"
#include "entanglement_decoder.hpp"
#include "entanglement_encoder.hpp"
#include "pipeline.cpp"
#include "pipeline.hpp"
#include "polar_decoder.hpp"
#include "polar_encoder.hpp"
#include "test_utils.hpp"
#include "types.hpp"

// FIXME: add expect statements with GMock
class MockDec : public Decoder {
    ChunkOperator get_coder() {
        return [](Writer& w) {};
    }
};

class MockEnc : public Encoder {
    ChunkOperator get_coder() {
        return [](Writer& w) {};
    }
};

class PipelineSingleCompositionTests
    : public ::testing::TestWithParam<std::tuple<Encoder*, Decoder*>> {};

TEST_P(PipelineSingleCompositionTests, PipelineFixtures) {
    Encoder* enc = std::get<0>(GetParam());
    Decoder* dec = std::get<1>(GetParam());

    // Create writer and arbitrary chunker
    Writer w;
    w.first = alloc_rand_chunker();
    w.second = new CryptoReceipt();
    Chunker ch_cp(*(w.first));

    // Encode pipeline
    Pipeline<Encoder> p_encode;
    p_encode.add_coder(*enc);
    p_encode.run(w);

    EXPECT_FALSE(*(w.first) == ch_cp);

    // Decode pipeline
    Pipeline<Decoder> p_decode;
    p_decode.add_coder(*dec);
    p_decode.run(w);

    EXPECT_TRUE(*(w.first) == ch_cp);

    delete w.first;
    delete w.second;
    delete enc;
    delete dec;
}

INSTANTIATE_TEST_CASE_P(
    PipelineFixtures, PipelineSingleCompositionTests,
    ::testing::Values(
        std::make_tuple(new PolarEncoder, new PolarDecoder),
        std::make_tuple(new ConcurrentEncoder, new ConcurrentDecoder),
        std::make_tuple(new EntanglementEncoder, new EntanglementDecoder)));

class PipelineComposeAllTests
    : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(PipelineComposeAllTests, PipelineFixtures) {
    int file_size = std::get<0>(GetParam());
    int chunk_size = std::get<1>(GetParam());

    Writer w;
    w.first = alloc_rand_chunker(file_size, chunk_size);
    w.second = new CryptoReceipt();
    Chunker ch_cp(*(w.first));

    Pipeline<Encoder> p_encode;
    p_encode.add(3, PolarEncoder(), ConcurrentEncoder(), EntanglementEncoder());

    Pipeline<Decoder> p_decode;
    p_decode.add(3, EntanglementDecoder(), ConcurrentDecoder(), PolarDecoder());

    p_encode.run(w);

    EXPECT_FALSE(*(w.first) == ch_cp);

    p_decode.run(w);

    EXPECT_TRUE(*(w.first) == ch_cp);

    delete w.first;
    delete w.second;
}

INSTANTIATE_TEST_CASE_P(PipelineFixtures, PipelineComposeAllTests,
                        ::testing::Values(std::make_tuple(13, 5),
                                          std::make_tuple(400, 51),
                                          std::make_tuple(1000, 200)));

TEST(PipelineTest, RunEmptyPipeline) {
    Writer w;
    w.first = alloc_rand_chunker();
    w.second = new CryptoReceipt();

    Pipeline<Encoder> p;
    p.run(w);

    delete w.first;
    delete w.second;
}

TEST(PipelineTest, RunMockSingleComposition) {
    Writer w;
    w.first = alloc_rand_chunker();
    w.second = new CryptoReceipt();

    // Encode pipeline
    Pipeline<Encoder> p_encode;
    MockEnc enc;
    p_encode.add_coder(enc);
    p_encode.run(w);

    // Decode pipeline
    Pipeline<Encoder> p_decode;
    MockEnc dec;
    p_encode.add_coder(dec);
    p_encode.run(w);

    delete w.first;
    delete w.second;
}