#ifndef RECEIPT_SESSION_H
#define RECEIPT_SESSION_H


#include <iostream>
#include <iterator>
#include <sstream>

#include "../../Poco/Net/StreamSocket.h"
#include "../../Poco/Logger.h"

#include "receipt/crypto_receipt.hpp"
#include "tools/types.hpp"

namespace P = Poco;
namespace P_N = P::Net;

namespace Qtoken {

/**
 * ReceiptSession establishes a TCP session with one or more other peers. It
 * queries the peers for their public keys and sends and encrypted receipt blob.
 */
class ReceiptSession {
public:
    ReceiptSession() {}
    explicit ReceiptSession(CryptoReceipt receipt);
    void send_receipt(std::vector<Addr> addresses);

private:
    std::vector<unsigned char> encrypt_receipt(
        const std::vector<unsigned char>& pubkey_bin);

    std::vector<unsigned char> receipt_bytes;
};

}  // namespace Qtoken

#endif // RECEIPT_SESSION_H