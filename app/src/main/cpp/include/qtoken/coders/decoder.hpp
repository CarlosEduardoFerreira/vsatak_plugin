#ifndef DECODER_H
#define DECODER_H

#include <vector>

#include "../receipt/crypto_receipt.hpp"
#include "../tools/types.hpp"
#include "../tools/utils.hpp"

namespace Qtoken {

/**
 * Decoder performs decoding on chunker objects.
 */
class Decoder {
public:
    Decoder() {}
    virtual ~Decoder() {}
    Decoder(Decoder& dec) = default;
    Decoder(Decoder&& dec) = delete;

    virtual ChunkOperator get_coder() = 0;
};

}  // namespace Qtoken

#endif // DECODER_H