// TYPES_H_ Definition in Aff3ct library
#ifndef TYPES_H
#define TYPES_H

#include <functional>

#include "chunker.hpp"

namespace Qtoken {

// NOTE: std::vector<bool> is not a container and does not support
// every vector operation in the ways we might expect
using Bitlist = std::vector<bool>;
class CryptoReceipt;
using Writer = std::pair<Chunker*, CryptoReceipt*>;
using ChunkOperator = std::function<void(Writer&)>;

using Key = std::string;
using Val = std::string;
using IP = std::string;
using Port = std::string;

// type for signed/unsigned bits default
using Byte_8 = int8_t;
using Byte = Byte_8;
using UByte_8 = uint8_t;
using UByte = UByte_8;

// struct Keypair {
//     std::vector<unsigned char> public_key;
//     std::vector<unsigned char> private_key;

//     Keypair(std::vector<unsigned char> public_key,
//             std::vector<unsigned char> private_key) {
//         if (public_key.size() != crypto_box_PUBLICKEYBYTES &&
//             private_key.size() != crypto_box_SECRETKEYBYTES)
//             throw("invalid length for keypair");
//         this->public_key = public_key;
//         this->private_key = private_key;
//     }
// };

struct Addr {
    IP ip;
    Port port;

    Addr(const IP& ip, const Port& port)
        : ip(ip)
        , port(port) {}
};

}  // namespace Qtoken

#endif // TYPES_H