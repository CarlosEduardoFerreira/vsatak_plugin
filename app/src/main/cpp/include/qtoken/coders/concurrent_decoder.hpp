#ifndef CONCURRENT_DECODER_H
#define CONCURRENT_DECODER_H

#include <unordered_map>

#include "decoder.hpp"
#include "../tools/lfsr.hpp"

namespace Qtoken {

class ConcurrentDecoder : public Decoder {
public:
    // Decodes a block of data divisable into frames
    // Each K bit frame of the block is encoded into an N bit codeword
    // A number of these frames can be overlaid into the same N bit codeword
    // using different hash functions Codewords are recombined back into an
    // output block
    // Note: Decoding with overlays > 1 is not deterministic as hashes may
    // overlap
    ConcurrentDecoder(const int& overlays = 1, const int& frames = 1,
                      const int& N = 32, const int& K = 4);
    ~ConcurrentDecoder() = default;
    ChunkOperator get_coder() override { return concurrent_decoder; };
    // It is expected that the input will pad to the nearest frame
    // and to a number of frames divisable by overlays
    std::vector<char> decode(const std::vector<char>& bytes);
    Bitlist decode(Bitlist bits);

private:
    const int N;         // Codeword bits
    const int K;         // Info bits
    const int frames;    // Frames to be encoded
    const int overlays;  // Messages per codeword
    ChunkOperator concurrent_decoder;

    // Decode a frame with overlays overlaid info frames
    std::vector<Bitlist> decode_frame(Bitlist cw);
    // These maps take ints 1-N to other pseudo random ints 1-N
    std::vector<std::unordered_map<int, int>> hash_tables;
    // This map takes prefixes of our K info bits to an int between 1-N
    std::unordered_map<Bitlist, int> prefix_map;

    void init_hash_list();
    void init_prefix_map();
};

}  // namespace Qtoken

#endif // CONCURRENT_DECODER_H