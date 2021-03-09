#ifndef ENTANGLEMENT_ENCODER_H
#define ENTANGLEMENT_ENCODER_H

#include "encoder.hpp"

namespace Qtoken {

class EntanglementEncoder : public Encoder {
public:
    EntanglementEncoder();
    virtual ~EntanglementEncoder() = default;
    ChunkOperator get_coder() override { return entanglement_encoder; };

private:
    ChunkOperator entanglement_encoder;
};

}  // namespace Qtoken

#endif // ENTANGLEMENT_ENCODER_H