#ifndef POLAR_FBG_H
#define POLAR_FBG_H

#include "../../aff3ct-2.3.5/aff3ct/aff3ct.hpp"

using namespace aff3ct;

namespace Qtoken {

// Generates and exposes frozen bits for polar encoders/decoders
class PolarFBG {
public:
    // N and K bits
    PolarFBG(const int& N, const int& K);
    virtual ~PolarFBG() { delete fb_generator; }
    void generateFrozenBits();
    std::vector<bool> getFrozenBits() { return frozen_bits; }

protected:
    const int N;  // Number of codeword bits
    const int K;  // Number of info bits
    std::vector<bool> frozen_bits;

private:
    factory::Frozenbits_generator::parameters frozen_bit_params;
    tools::Frozenbits_generator* fb_generator;
};

}  // namespace Qtoken

#endif // POLAR_FBG_H