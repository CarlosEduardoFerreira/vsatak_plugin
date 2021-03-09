#include "lfsr.hpp"

using namespace Qtoken;
/**
 * Creates an LFSR object which uses LFSR sequences
 * to build mappings of variable size.
 * Only a pseudo random map of size 32 is generated larger
 * maps are created by appending multiple 32 bit maps
 * @param seed used as initial lfsr state
 */
LFSR::LFSR(uint8_t seed) {
    // if seed is 0, init with clock time
    if (seed == 0)
        seed = get_rand_seed();
    this->seed = seed;
    this->state = seed;
}

/**
 * Returns the next randomly generated 5-bit number in the sequence.
 * Returns LFSR_RESET if this happens to be the seed
 * @return random 5 bit sequence (first three bits are always zeroed out)
 */
uint8_t LFSR::next5() {
    uint8_t curr = state & five_bit_cut;
    uint8_t next = (curr >> 0) ^ (curr >> 2);
    state = (curr >> 1) | (next << 4);
    state = state & five_bit_cut;

    if (state == uint8_t(seed & five_bit_cut))
        return LFSR_RESET;
    return state;
}

/**
 * Builds a pseudo random one to one map:(0, size - 1) -> (0, size - 1)
 * @param size must be a power of 2 greater than 2^5
 * @return a pseudo random map from (0, size - 1) -> (0, size - 1)
 */
std::unordered_map<int, int> LFSR::build_map(int size) {
    std::unordered_map<int, int> m;
    uint8_t curr_state = 0;
    std::unordered_map<int, int> full_size_hash;

    // Generate map m:(0, P-1) -> (1, P-1) where P is total
    // count of all 5 bit binary sequences
    int count = 1;
    m[int(state & five_bit_cut)] = 0;
    while (curr_state != LFSR_RESET || count == 1) {
        curr_state = next5();
        m[int(curr_state)] = count;
        count++;
    }

    // Concatenate m to create a map of size size
    for (int i = 0; i < size / m.size(); i++) {
        for (int j = 0; j < m.size(); j++) {
            full_size_hash[(i * m.size()) + j] = (i * m.size()) + m[j];
        }
    }

    return full_size_hash;
}
