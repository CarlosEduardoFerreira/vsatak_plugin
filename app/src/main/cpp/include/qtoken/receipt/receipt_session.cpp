#include "receipt_session.hpp"

using namespace Qtoken;

ReceiptSession::ReceiptSession(CryptoReceipt receipt) {
    // Read the receipt into a vector of strings
    std::stringstream ss = receipt.serialize();
    std::string rec_str = ss.str();
    receipt_bytes = std::vector<unsigned char>(
        rec_str.c_str(), rec_str.c_str() + rec_str.size() + 1);
}

std::vector<unsigned char> ReceiptSession::encrypt_receipt(
    const std::vector<unsigned char> &pubkey_bin) {
    // TODO: Encrypt the receipt with the provided
    // public key
    return receipt_bytes;
}

void ReceiptSession::send_receipt(std::vector<Addr> addresses) {
    P_N::StreamSocket ss;
    std::string pubkey_req = "pubkey";  // FIXME: Move to config constant
    std::vector<unsigned char> enc_receipt;

    for (Addr addr : addresses) {
        // Establish connection to recipient receipt server
        std::cout << "Connecting to peer..." << std::endl;
        ss.connect(P_N::SocketAddress(addr.ip, addr.port));

        auto receipt_bytes_ = receipt_bytes;

        // Request public key from node
        std::cout << "Requesting public key..." << std::endl;
        ss.sendBytes(pubkey_req.data(), pubkey_req.size());

        sleep(1);
        // Recieve public key from node
        // std::cout << "Recieving public key..." << std::endl;
        // int n = ss.receiveBytes(pubkey_bin.data(),
        // crypto_box_PUBLICKEYBYTES);

        // Encrypt receipt with public key
        std::cout << "Encrypting receipt..." << std::endl;
        // enc_receipt = encrypt_receipt(receipt_bytes);

        // Send encypted receipt
        std::cout << "Sending receipt..." << std::endl;
        ss.sendBytes(receipt_bytes_.data(), receipt_bytes_.size());
    }

    ss.shutdown();
}
