#ifndef ENCODER_H
#define ENCODER_H

#include <vector>

#include "crypto_receipt.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace Qtoken {

/**
 * Encoder performs encoding operations VIN encoding
 * on chunker objects.
 */
class Encoder {
public:
    Encoder() {}
    virtual ~Encoder() {}
    Encoder(Encoder& enc) = default;
    Encoder(Encoder&& enc) = delete;

    virtual ChunkOperator get_coder() = 0;
};

}  // namespace Qtoken

#endif
