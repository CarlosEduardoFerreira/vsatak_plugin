#ifndef DECODER_H
#define DECODER_H

#include <vector>

#include "crypto_receipt.hpp"
#include "types.hpp"
#include "utils.hpp"

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

#endif
