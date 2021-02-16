#include "concurrent_decoder.hpp"

using namespace Qtoken;

/**
 * Creates a decoder containing functions for decoding binary or bytestream
 * frames
 * @param overlays number of info frames overlayed into a single encoded frame
 * @param frames number of "blocks" size N to be decoded
 * @param N codeword size in bits
 * @param K info size in bits
 */
ConcurrentDecoder::ConcurrentDecoder(const int& overlays, const int& frames,
                                     const int& N, const int& K)
    : overlays(overlays)
    , frames(frames)
    , N(N)
    , K(K)
    /**
     * Lambda function for concurrent encoding an entire chunker object
     * Note: The lambda function will always be constructed with default N and K
     * Additionally the frame count will be adjusted to match the passed chunker
     * object
     * @param w writer containing chunker and crypto receipt
     */
    , concurrent_decoder([N, K](const Writer& w) {
        Chunker* ch = w.first;
        CryptoReceipt* cr = w.second;

        int file_size = ch->get_file_size();
        int chunk_size = ch->get_chunk_size();
        int frame_size = N / CHAR_BIT;  // Size of a frame in bytes

        int dec_chunk_size = 1;
        if (cr->get("conc_chunk_size") != "" &&
            cr->get("conc_file_size") != "") {
            dec_chunk_size = std::stoi(cr->get("conc_chunk_size"));
        } else {
            // FIXME: need a common error handling
            exit(EXIT_FAILURE);
        }

        std::vector<std::vector<char>>& chunks = ch->get_chunks();
        ConcurrentDecoder decoder(1, chunk_size / frame_size);

        for (int i = 0; i < chunks.size(); i++) {
            chunks[i] = std::move(decoder.decode(chunks[i]));
        }

        ch->resize_chunks(dec_chunk_size);
    }) {
    init_hash_list();
    init_prefix_map();
}

/**
 * Decodes a char* representation of a byte stream
 * @param bytes byte stream to be decoded - input is not changed
 * @return decoded byte stream is allocated on the returned char*
 */
std::vector<char> ConcurrentDecoder::decode(std::vector<char>& bytes) {
    Bitlist bits = bytes_to_bits<Bitlist>(bytes);
    bits = decode(bits);
    return bits_to_bytes<Bitlist>(bits);
}

/**
 * Concurrent decodes a bitlist with entries representing binary
 * @param bits input binary is not changed
 * @return bitlist containing decoded output binary
 */
Bitlist ConcurrentDecoder::decode(Bitlist bits) {
    Bitlist encoded_frame(N);
    Bitlist decoded;
    // For each encoded frame in the codeword
    for (int i = 0; i < bits.size() / N; i++) {
        // Copy the frame
        for (int j = 0; j < N; j++) {
            encoded_frame[j] = bits[(i * N) + j];
        }

        // Get all decoded frames overlaid into the cw frame
        std::vector<Bitlist> decoded_frames = decode_frame(encoded_frame);
        // Add the decoded messages to the output vec in hash order
        for (auto decoded_frame : decoded_frames) {
            for (int j = 0; j < decoded_frame.size(); j++) {
                decoded.push_back(decoded_frame[j]);
            }
        }
    }

    return decoded;
}

/**
 * Concurrent decodes a single N bit encoded bitlist frame with
 * entries representing binary. In the case where overlays > 1 multiple
 * info frames might be overlayed onto a single encoded frame so a list
 * of decoded frames must be returned.
 * @param cw the encoded N bit frame
 * @return std::vector<bitlist> list of decoded frames
 */
std::vector<Bitlist> ConcurrentDecoder::decode_frame(Bitlist cw) {
    std::vector<Bitlist> decoded_frames;

    // For each hash
    for (int i = 0; i < overlays; i++) {
        std::vector<Bitlist> pres;
        Bitlist pre;
        pres.push_back(pre);

        // For each prefix length
        for (int j = 0; j < K; j++) {
            int pres_size = pres.size();

            // For each live prefix path
            for (int k = 0; k < pres_size; k++) {
                pre = pres[k];
                pre.push_back(1);

                if (cw[(hash_tables[i])[prefix_map[pre]]]) {
                    // Add the valid prefix to the "tree"
                    pres.push_back(pre);
                }

                // Check if pre + 0 is marked
                pre[pre.size() - 1] = 0;

                if (cw[(hash_tables[i])[prefix_map[pre]]]) {
                    // Add the valid prefix to the "tree"
                    pres.push_back(pre);
                }
            }

            // Prune previous shorter paths
            pres.erase(pres.begin(), pres.begin() + pres_size);
        }

        // Decoded frames added in order of hash
        for (auto p : pres) {
            decoded_frames.push_back(p);
        }
    }

    return decoded_frames;
}

/**
 * Initializes multiple maps (1, N) -> (1, N)
 * These hashs are used to evenly distribute N prefixes
 * across our size N codeword. In the case where multiple
 * info frames are overlayed into a single codeword, a
 * different hash is used for each info frame
 */
void ConcurrentDecoder::init_hash_list() {
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
void ConcurrentDecoder::init_prefix_map() {
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
