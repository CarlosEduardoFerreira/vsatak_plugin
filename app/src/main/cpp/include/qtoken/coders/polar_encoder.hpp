#ifndef POLAR_ENCODER_H
#define POLAR_ENCODER_H

#include "coders/encoder.hpp"
#include "coders/polar_fbg.hpp"

namespace Qtoken {

class PolarEncoder : public Encoder {
public:
    PolarEncoder(const int& frames = 1, const int& N = 128, const int& K = 32);
    ~PolarEncoder() = default;
    ChunkOperator get_coder() override { return polar_encoder; };
    // Encode an std::vector<int> with entries representing bits
    std::vector<int> encode(std::vector<int> bits);
    // Encode a char* representation of a byte stream
    std::vector<char> encode(const std::vector<char>& bytes);

private:
    const int N;       // Number of codeword bits
    const int K;       // Number of info bits
    const int frames;  // Frames to be encoded
    std::unique_ptr<module::Encoder_polar_sys<>> encoder;
    ChunkOperator polar_encoder;
    PolarFBG frozen_bits;
};

}  // namespace Qtoken

#endif // POLAR_ENCODER_H