#ifndef LFSR_H
#define LFSR_H

#include <unordered_map>

#include "types.hpp"
#include "utils.hpp"

const int LFSR_RESET = 0;

namespace Qtoken {
class LFSR {
public:
    explicit LFSR(uint8_t seed = 0);
    ~LFSR() = default;
    uint8_t next5();
    // Note: Must be a power of 2 greater than 2^5
    std::unordered_map<int, int> build_map(int size);

private:
    uint8_t state;
    uint8_t seed;
    const uint8_t five_bit_cut = 31;
};

}  // namespace Qtoken

#endif
