#include "concurrent_encoder.hpp"

using namespace Qtoken;

/**
 * Creates an encoder containing functions for decoding binary or bytestream
 * frames
 * @param overlays number of info frames overlayed into a single encoded frame
 * @param frames number of "blocks" size N to be decoded
 * @param N codeword size in bits
 * @param K info size in bits
 */
ConcurrentEncoder::ConcurrentEncoder(const int& overlays, const int& frames,
                                     const int& N, const int& K)
    : overlays(overlays)
    , frames(frames)
    , N(N)
    , K(K)
    /**
     * Lambda function for concurrent decoding an entire chunker object
     * Note: The lambda function will always be constructed with default N and K
     * Additionally the frame count will be adjusted to match the passed chunker
     * object
     * @param w writer containing chunker and crypto receipt
     */
    , concurrent_encoder([K, N](const Writer& w) {
        Chunker* ch = w.first;
        CryptoReceipt* cr = w.second;

        // Write the file size and chunk size to the crypto receipt
        int file_size = ch->get_file_size();
        int chunk_size = ch->get_chunk_size();

        cr->put(Key("conc_file_size"), Val(std::to_string(file_size)));
        cr->put(Key("conc_chunk_size"), Val(std::to_string(chunk_size)));

        int frame_size = (K + CHAR_BIT - 1) / CHAR_BIT;
        int new_chunk_size =
            (chunk_size % frame_size)
                ? chunk_size + frame_size - (chunk_size % frame_size)
                : chunk_size;

        ch->resize_chunks(new_chunk_size);
        int num_frames = (ch->get_chunk_size() * CHAR_BIT) / K;

        std::vector<std::vector<char>>& chunks = ch->get_chunks();
        ConcurrentEncoder encoder(1, num_frames);

        for (int i = 0; i < chunks.size(); i++) {
            chunks[i] = std::move(encoder.encode(chunks[i]));
        }
    }) {
    init_hash_list();
    init_prefix_map();
}

/**
 * Encodes a char* representation of a byte stream
 * @param bytes byte stream to be encoded - input is not changed
 * @return encoded byte stream is allocated on the returned char*
 */
std::vector<char> ConcurrentEncoder::encode(const std::vector<char>& bytes) {
    Bitlist bits = bytes_to_bits<Bitlist>(bytes);
    bits = encode(bits);
    return bits_to_bytes<Bitlist>(bits);
}

/**
 * Concurrent encodes an bitlist with entries representing binary
 * @param bits input binary is not changed
 * @return bitlist containing encoded output binary
 */
Bitlist ConcurrentEncoder::encode(Bitlist bits) {
    Bitlist encoded_bits((frames * N) / overlays);
    Bitlist msg(K);
    int frame_set_size = overlays * K;
    int info_frames = bits.size() / K;

    // For each set of frames to be overlaid into an encoded frame
    for (int i = 0; i < info_frames / overlays; i++) {
        // For each frame in the set
        for (int j = 0; j < overlays; j++) {
            // Copy frame j
            for (int k = 0; k < K; k++) {
                msg[k] = bits[(frame_set_size * i) + (K * j) + k];
            }
            // Add it to output frame i with hash j
            add_message(encoded_bits, msg, i, j);
        }
    }

    return encoded_bits;
}

/**
 * Adds a single frame of info bits to the final encoded output of enocded_bits.
 * Info bits are encoded onto frame frame_num of the encoded_bits
 * Info bits are encoded with hash number hash_num
 * @param encoded_bits Holds the final encoded output
 * @param info_frame A single frame of info bits to be enocded and added
 * @param frame_num The frame of encoded_bits on which this info frame will
 * be encoded
 * @param hash_num The hash number to be used encoding these info bits
 */
void ConcurrentEncoder::add_message(Bitlist& encoded_bits, Bitlist info_frame,
                                    int frame_num, int hash_num) {
    int info_frame_size = info_frame.size();
    int frame_offset = frame_num * N;

    for (int i = 0; i < info_frame_size; i++) {
        // Get the index of the current prefix
        int index = prefix_map[info_frame];
        // Mark the prefix's associated bit
        encoded_bits[frame_offset + (hash_tables[hash_num])[index]] = 1;
        // Resize down to the next prefix
        info_frame.resize(info_frame.size() - 1);
    }
}

/**
 * Initializes multiple maps (1, N) -> (1, N)
 * These hashs are used to evenly distribute N prefixes
 * across our size N codeword. In the case where multiple
 * info frames are overlayed into a single codeword, a
 * different hash is used for each info frame
 */
void ConcurrentEncoder::init_hash_list() {
    for (int i = 0; i < overlays; i++) {
        LFSR l(i + 1);
        std::unordered_map<int, int> hash = l.build_map(N);
        hash_tables.push_back(hash);
    }
}

/**
 * Initializes the unordered map prefix_map
 * such that every binary sequence which could
 * appear in a K bit sequence maps to an integer
 * (1, N)
 */
void ConcurrentEncoder::init_prefix_map() {
    // Set starting bits to build on
    Bitlist b1;
    Bitlist b2;
    b1.push_back(0);
    b2.push_back(1);
    prefix_map[b1] = 0;
    prefix_map[b2] = 1;

    std::vector<Bitlist> prefixes{b1, b2};

    int index = 2;
    for (int i = 0; i < K - 1; i++) {
        int last_prefix_size = prefixes.size();
        for (int j = 0; j < last_prefix_size; j++) {
            Bitlist pre_zero(prefixes[j]);
            Bitlist pre_one(prefixes[j]);
            pre_zero.push_back(0);
            pre_one.push_back(1);
            prefixes[j] = pre_zero;
            prefixes.push_back(pre_one);
        }

        // Add the length i prefixes to the map
        for (Bitlist prefix : prefixes) {
            prefix_map[prefix] = index;
            index++;
        }
    }
}
