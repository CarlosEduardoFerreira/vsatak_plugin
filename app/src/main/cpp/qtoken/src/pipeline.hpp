#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "types.hpp"

namespace Qtoken {

/**
 * Encoder pipeline inspired by monads. Takes different encoders and
 * composes them together to a functional pipeline. The composed
 * function takes a Chunker instance and sequentially encodes it.
 * All of them have access to a shared CryptoReceipt struct that
 * they can write to.
 */
template <typename Coder>
class Pipeline {
private:
    ChunkOperator base;

public:
    Pipeline()
        : base([](Writer& w) {}) {}
    Pipeline(Pipeline& pipeline) = default;
    Pipeline(Pipeline&& pipeline) = default;
    ~Pipeline() {}

    void add_coder(Coder& c);
    void add(int count, ...);
    void run(Writer& w);

private:
    ChunkOperator compose(ChunkOperator f, ChunkOperator g);
    CryptoReceipt* merge_receipts(CryptoReceipt* cr1, CryptoReceipt* cr2);
};

}  // namespace Qtoken

#endif
