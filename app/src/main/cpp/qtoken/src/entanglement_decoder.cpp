#include "entanglement_decoder.hpp"

using namespace Qtoken;

EntanglementDecoder::EntanglementDecoder()
    : entanglement_decoder([](Writer& w) {
        Chunker* chs = w.first;
        int chunk_size = chs->get_chunk_size();
        CryptoReceipt* cr = w.second;
        std::vector<std::vector<char>>& chunks = chs->get_chunks();
        auto ch_size = cr->get("ent_ch_size");
        int ch_size_i = std::atoi(ch_size.c_str());

        // iterate through chunks
        for (int i = 0; i < chunks.size(); i++) {
            auto& ch = chunks[i];
            char xor_output[chunk_size];
            Key k = "hash_ch_" + std::to_string(i);

            // get hash from crypto receipt
            std::string hash = cr->get(k);
            std::vector<char> hash_vec(hash.c_str(),
                                       hash.c_str() + hash.size());

            // XOR hash with encoded chunk to get back original
            ch = std::move(XOR_bytes(ch, hash_vec));
        }
        chs->resize_chunks(ch_size_i);
    }) {}
