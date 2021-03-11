#ifndef CONCURRENT_ENCODER_H
#define CONCURRENT_ENCODER_H

#include <unordered_map>

#include "encoder.hpp"
#include "../tools/lfsr.hpp"

namespace Qtoken {

class ConcurrentEncoder : public Encoder {
public:
    // Encodes a block of data divisable into frames
    // Each K bit frame of the block is encoded into an N bit codeword
    // A number of these frames can be overlaid into the same N bit codeword
    // using different hash functions Codewords are recombined back into an
    // output block Note: Decoding is not deterministic as hashes may overlap
    ConcurrentEncoder(const int& overlays = 1, const int& frames = 1,
                      const int& N = 32, const int& K = 4);
    ~ConcurrentEncoder() = default;
    ChunkOperator get_coder() override { return concurrent_encoder; };
    // It is expected that the info bits will be padded to the nearest frame
    // and to a number of frames divisable by overlays
    std::vector<char> encode(const std::vector<char>& bytes);
    Bitlist encode(Bitlist bits);

private:
    const int N;         // Codeword frame bits
    const int K;         // Info bits
    const int frames;    // Frames to be encoded
    const int overlays;  // Messages per codeword
    ChunkOperator concurrent_encoder;

    // Overlay msg onto the codeword at given frame number with the given hash
    void add_message(Bitlist& encoded_bits, Bitlist info_frame, int frame_num,
                     int hash_num);
    // These maps take ints 1-N to other pseudo random ints 1-N
    std::vector<std::unordered_map<int, int>> hash_tables;
    // This map takes prefixes of our K bit codeword to an int between 1-N
    std::unordered_map<Bitlist, int> prefix_map;

    void init_hash_list();
    void init_prefix_map();
};

}  // namespace Qtoken

#endif // CONCURRENT_ENCODER_H