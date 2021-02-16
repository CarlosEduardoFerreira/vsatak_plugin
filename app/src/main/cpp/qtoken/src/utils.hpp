#ifndef UTILS_H
#define UTILS_H

#include <climits>
#include <cstring>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

// OpenSSL Library
#include <openssl/hmac.h>
#include <openssl/sha.h>

#include "types.hpp"

using namespace Qtoken;

// TODO: Both input and output of this function should be
// templated
/**
 * Convert char* bytes to templated Container
 * Container must implement push_back()
 * @param bytes bytes to be converted
 * @param num_bytes length of byte stream
 * @return bit vector
 */
template <class Container>
inline Container bytes_to_bits(std::vector<char> bytes) {
    Container bits;
    for (int i = 0; i < bytes.size(); i++) {
        char c = bytes[i];
        for (int j = CHAR_BIT - 1; j >= 0; j--) {
            bits.push_back(int((c >> j) & 1));
        }
    }
    return bits;
}

// TODO: Both input and output of this function should be
// templated
/**
 * Convert templated container bits to char* bytes
 * Container must implement size()
 * @param bits binary to be converted
 * @return byte stream is allocated on the return char*
 */
template <class Container>
inline std::vector<char> bits_to_bytes(Container bits) {
    int num_bytes = bits.size() / 8;
    std::vector<char> bytes;
    for (int i = 0; i < num_bytes; i++) {
        char c = 0x00;  // Zeroed out char
        for (int j = CHAR_BIT - 1; j >= 0; j--) {
            c |= bits[(i * CHAR_BIT) + ((CHAR_BIT - 1) - j)] << j;
        }
        bytes.push_back(c);
    }
    return bytes;
}

inline std::string sha256(const std::string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

inline Keypair create_keys() {
    std::vector<unsigned char> public_key(crypto_box_PUBLICKEYBYTES);
    std::vector<unsigned char> private_key(crypto_box_SECRETKEYBYTES);

    int keygen = crypto_box_keypair(public_key.data(), private_key.data());
    if (keygen == 0) {
        std::cout << "Crypto Key-Pair Successfully Generated!" << std::endl;
        std::cout << "Node's Public Key:" << std::endl;

    } else {
        throw("ERROR: Crypto Key-Pair Generation Failed!");
    }
    return Keypair(public_key, private_key);
}

// overload std::begin and std::end for char*
inline char* begin(char* s) {
    return s;
}
inline char* end(char* s) {
    return s + strlen(s);
}

/**
 * Variable length XOR for two char arrays.
 *
 * @param a first operand
 * @param b second param
 *
 * @return output char* (this contains heap memory!!)
 */
inline std::vector<char> XOR_bytes(std::vector<char> a, std::vector<char> b) {
    int a_size = a.size();
    int b_size = b.size();

    std::vector<char> greater = (a_size >= b_size) ? a : b;
    std::vector<char> lesser = (a_size < b_size) ? a : b;
    std::vector<char> out;

    for (int i = lesser.size(); i < greater.size(); i++) lesser.push_back(0);
    for (int i = 0; i < greater.size(); i++) {
        out.push_back(0);
        if (i >= lesser.size())
            lesser.push_back(0);
    }

    std::transform(begin(greater), end(greater), begin(lesser), begin(out),
                   std::bit_xor<char>());

    return out;
}

/**
 * Returns a random uint8_t
 * RNG is seeded with clock time
 * @return psuedo random uint8_t
 */
inline uint8_t get_rand_seed() {
    return (uint8_t)std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::time_point_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now())
                   .time_since_epoch())
        .count();
}

/**
 * Returns a random uint32_t
 * RNG is seeded with clock time
 * @return psuedo random uint32_t
 */
inline uint32_t get_rand_seed_uint32_t() {
    return (uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::time_point_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now())
                   .time_since_epoch())
        .count();
}

inline bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) {
                             return !std::isdigit(c);
                         }) == s.end();
}

#endif
