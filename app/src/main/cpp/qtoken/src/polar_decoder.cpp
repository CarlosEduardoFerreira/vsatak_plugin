#include "polar_decoder.hpp"

/**
 * Creates a decoder containing functions for decoding binary or bytestream
 * frames
 * @param frames number of "blocks" size N to be decoded
 * @param N code word size in bits - Must be a power of 2
 * @param K info size in bits - A decent ratio is N/K = 4
 */
PolarDecoder::PolarDecoder(const int& frames, const int& N, const int& K)
    : N(N)
    , K(K)
    , frozen_bits(N, K)
    , frames(frames)
    /**
     * Lambda function for polar decoding an entire chunker object
     * Note: The lambda function will always be constructed with default N and K
     * Additionally the frame count will be adjusted to match the passed chunker
     * object
     * @param w writer containing chunker and crypto receipt
     */
    , polar_decoder([N, K](const Writer& w) {
        Chunker* ch = w.first;
        CryptoReceipt* cr = w.second;

        int file_size = ch->get_file_size();
        int chunk_size = ch->get_chunk_size();
        int frame_size = N / CHAR_BIT;  // Size of a frame in bytes

        int dec_chunk_size = 1;
        if (cr->get("polar_chunk_size") != "" &&
            cr->get("polar_file_size") != "") {
            dec_chunk_size = std::stoi(cr->get("polar_chunk_size"));
        } else {
            // FIXME: need standardized error handling
            exit(EXIT_FAILURE);
        }

        std::vector<std::vector<char>>& chunks = ch->get_chunks();
        PolarDecoder decoder(chunk_size / frame_size);

        for (int i = 0; i < chunks.size(); i++) {
            chunks[i] = std::move(decoder.decode(chunks[i]));
        }

        ch->resize_chunks(dec_chunk_size);
    }) {}

/**
 * Polar decodes an std::vector<int> with entries representing binary
 * @param bits input binary is not changed
 * @return std::vector<int> containing decoded output binary
 */
std::vector<int> PolarDecoder::decode(std::vector<int> bits) {
    std::vector<int> dec_bits(K * frames);
    frozen_bits.generateFrozenBits();
    std::vector<bool> fbits(frozen_bits.getFrozenBits());
    decoder = std::unique_ptr<module::Decoder_polar_SC_fast_sys<>>(
        new module::Decoder_polar_SC_fast_sys<>(K, N, fbits, frames));

    // Snap bits to valid LLRs
    // There is no hard in hard out decode for polar codes
    // Since we are encoding at the application level we have to make our
    // encoded bits look like LLRs
    std::vector<float> enc_floats(bits.begin(), bits.end());
    for (int i = 0; i < enc_floats.size(); i++)
        enc_floats[i] = enc_floats[i] == 1 ? -.5 : .5;

    // TODO: *Research the use case for decode_siho_cw this might be the
    // application level decode we want decoder->decode_siho_cw(b.enc_bits,
    // b.dec_bits);
    decoder->decode_siho(enc_floats, dec_bits);

    // Output bits snap to 0 and INT_MIN
    // Here we convert the vector back to a bit format
    for (int i = 0; i < dec_bits.size(); i++) {
        dec_bits[i] = dec_bits[i] < 0 ? 1 : 0;
    }

    return dec_bits;
}

/**
 * Decodes a char* representation of a byte stream
 * @param bytes byte stream to be decoded - input is not changed
 * @return decoded byte stream is allocated on the returned char*
 */
std::vector<char> PolarDecoder::decode(std::vector<char> bytes) {
    std::vector<int> dec(decode(bytes_to_bits<std::vector<int>>(bytes)));
    return bits_to_bytes<std::vector<int>>(dec);
}
