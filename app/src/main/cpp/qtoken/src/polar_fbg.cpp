#include "polar_fbg.hpp"

using namespace Qtoken;

/**
 * Creates a frozen big generator for polar encoding
 * @param N encoder/decoder codeword size in bits
 * @param K encoder/decoder info size in bits
 */
PolarFBG::PolarFBG(const int& N, const int& K)
    : N(N)
    , K(K) {
    fb_generator = nullptr;
    frozen_bit_params.N_cw = N;
    frozen_bit_params.K = K;
    frozen_bits.resize(N);
}

/**
 * Generates the frozen bits
 */
void PolarFBG::generateFrozenBits() {
    if (fb_generator != nullptr) {
        return;
    }

    fb_generator = frozen_bit_params.build();

    // Noise variations can be toggled here
    // Options include a 5G noise simulation
    const auto ebn0 = frozen_bit_params.noise;
    tools::Sigma<float> noise(tools::esn0_to_sigma(tools::ebn0_to_esn0(ebn0)));
    fb_generator->set_noise(noise);

    fb_generator->generate(frozen_bits);
}
