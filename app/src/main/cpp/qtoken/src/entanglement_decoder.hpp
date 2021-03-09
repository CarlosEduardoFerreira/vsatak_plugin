#ifndef ENTANGLEMENT_DECODER_H
#define ENTANGLEMENT_DECODER_H

#include "decoder.hpp"

namespace Qtoken {

class EntanglementDecoder : public Decoder {
public:
    EntanglementDecoder();
    virtual ~EntanglementDecoder() = default;
    ChunkOperator get_coder() override { return entanglement_decoder; };

private:
    ChunkOperator entanglement_decoder;
};

}  // namespace Qtoken

#endif // ENTANGLEMENT_DECODER_H