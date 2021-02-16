#include "polar_encoder.hpp"
#include <iostream>

using namespace Qtoken;

/**
 * Creates an encoder containing functions for encoding binary or bytestream
 * frames
 * @param frames number of "blocks" size N to be encoded
 * @param N code word size in bits - Must be a power of 2
 * @param K info size in bits - A decent ratio is N/K = 4
 */
PolarEncoder::PolarEncoder(const int& frames, const int& N, const int& K)
    : N(N)
    , K(K)
    , frozen_bits(N, K)
    , frames(frames)
    /**
     * Lambda function for polar encoding an entire chunker object
     * Note: The lambda function will always be constructed with default N and K
     * Additionally the frame count will be adjusted to match the passed chunker
     * object
     * @param w writer containing chunker and crypto receipt
     */
    , polar_encoder([K](const Writer& w) {
        Chunker* ch = w.first;
        CryptoReceipt* cr = w.second;

        // Write the file size and chunk size to the crypto receipt
        int file_size = ch->get_file_size();
        int chunk_size = ch->get_chunk_size();

        cr->put(Key("polar_file_size"), Val(std::to_string(file_size)));
        cr->put(Key("polar_chunk_size"), Val(std::to_string(chunk_size)));

        int frame_size = K / CHAR_BIT;  // Size of a frame in bytes
        int new_chunk_size =
            (chunk_size % frame_size)
                ? chunk_size + frame_size - (chunk_size % frame_size)
                : chunk_size;

        ch->resize_chunks(new_chunk_size);
        int num_frames = ch->get_chunk_size() / frame_size;

        std::vector<std::vector<char>>& chunks = ch->get_chunks();
        PolarEncoder encoder(num_frames);

        for (int i = 0; i < chunks.size(); i++) {
            chunks[i] = std::move(encoder.encode(chunks[i]));
        }
    }){};

/**
 * Polar encodes an std::vector<int> with entries representing binary
 * @param bits input binary is not changed
 * @return std::vector<int> containing encoded output binary
 */
std::vector<int> PolarEncoder::encode(std::vector<int> bits) {
    std::vector<int> enc_bits(N * frames);
    frozen_bits.generateFrozenBits();
    std::vector<bool> fbits(frozen_bits.getFrozenBits());

    encoder = std::unique_ptr<module::Encoder_polar_sys<>>(
        new module::Encoder_polar_sys<>(K, N, fbits, frames));
    encoder->encode(bits, enc_bits);

    return enc_bits;
}

/**
 * Encodes a char* representation of a byte stream
 * @param bytes byte stream to be encoded - input is not changed
 * @return encoded byte stream is allocated on the returned char*
 */
std::vector<char> PolarEncoder::encode(std::vector<char> bytes) {
    std::vector<int> enc(encode(bytes_to_bits<std::vector<int>>(bytes)));
    return bits_to_bytes(enc);
}
