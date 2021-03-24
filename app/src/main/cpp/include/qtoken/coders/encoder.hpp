#ifndef ENCODER_H
#define ENCODER_H

#include <vector>

#include "receipt/crypto_receipt.hpp"
#include "tools/types.hpp"
#include "tools/utils.hpp"

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

#endif // ENCODER_H