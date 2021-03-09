#include "entanglement_encoder.hpp"

using namespace Qtoken;

EntanglementEncoder::EntanglementEncoder() {
    entanglement_encoder = [](const Writer& w) {
        Chunker* chs = w.first;
        int chunk_size = chs->get_chunk_size();
        CryptoReceipt* cr = w.second;
        std::vector<std::vector<char>>& chunks = chs->get_chunks();

        // write hash to crypto receipt
        Key k_cs("ent_ch_size");
        Val v_cs(std::to_string(chs->get_chunk_size()));
        cr->put(k_cs, v_cs);

        // iterate through chunks
        for (int i = 0; i < chunks.size(); i++) {
            auto& ch = chunks[i];
            std::string ch_s(ch.begin(), ch.end());

            // generate sha256 hash
            std::string hash = sha256(ch_s);
            std::vector<char> hash_vec(hash.begin(), hash.end());

            // XOR hash with chunk
            ch = XOR_bytes(ch, hash_vec);

            // write hash to crypto receipt
            Key k_h("hash_ch_" + std::to_string(i));
            Val v_h(hash);

            cr->put(k_h, v_h);
        }
    };
}
