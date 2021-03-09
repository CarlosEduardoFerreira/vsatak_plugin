#ifndef POLAR_DECODER_H
#define POLAR_DECODER_H

#include "decoder.hpp"
#include "polar_fbg.hpp"

namespace Qtoken {

class PolarDecoder : public Decoder {
public:
    PolarDecoder(const int& frames = 1, const int& N = 128, const int& K = 32);
    virtual ~PolarDecoder() = default;
    ChunkOperator get_coder() override { return polar_decoder; };
    // Decode an std::vector<int> with entries representing bits
    std::vector<int> decode(std::vector<int> bits);
    // Decode a char* representation of a byte stream
    std::vector<char> decode(const std::vector<char> &bytes);

private:
    const int N;       // Number of codeword bits
    const int K;       // Number of info bits
    const int frames;  // Frames to be decoded
    std::unique_ptr<module::Decoder_polar_SC_fast_sys<>> decoder;
    ChunkOperator polar_decoder;
    PolarFBG frozen_bits;
};

}  // namespace Qtoken

#endif // POLAR_DECODER_H