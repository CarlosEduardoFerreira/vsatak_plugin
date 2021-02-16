#include "ConnectionHandler.hpp"
#include <libconfig.h++>
#include "globals.hpp"

using namespace Qtoken;

ReceiptConnectionHandler::ReceiptConnectionHandler(P_N::StreamSocket& socket,
                                                   P_N::SocketReactor& reactor)
    : _socket(socket)
    , _reactor(reactor) {
    _reactor.addEventHandler(
        _socket,
        P::NObserver<ReceiptConnectionHandler, P_N::ReadableNotification>(
            *this, &ReceiptConnectionHandler::onReadable));
    _reactor.addEventHandler(
        _socket,
        P::NObserver<ReceiptConnectionHandler, P_N::ShutdownNotification>(
            *this, &ReceiptConnectionHandler::onShutdown));
}

ReceiptConnectionHandler::~ReceiptConnectionHandler() noexcept(false) {
    _reactor.removeEventHandler(
        _socket,
        P::NObserver<ReceiptConnectionHandler, P_N::ReadableNotification>(
            *this, &ReceiptConnectionHandler::onReadable));
    _reactor.removeEventHandler(
        _socket,
        P::NObserver<ReceiptConnectionHandler, P_N::ShutdownNotification>(
            *this, &ReceiptConnectionHandler::onShutdown));
}

/**
 * Handles incoming bytes. Is capable of having a back-and-forth within the same
 * handler.
 */
void ReceiptConnectionHandler::onReadable(
    const P::AutoPtr<P_N::ReadableNotification>& pNf) {
    // create buffer to read request
    char* _pBuffer = new char[MSG_BUFFER_SIZE];

    const libconfig::Setting& settings = cfg->getRoot();
    const libconfig::Setting& receipts_cfgs =
        settings["file_system"]["receipts"];
    const libconfig::Setting& keys_cfgs = settings["file_system"]["keys"];

    // Read request
    std::cout << "Reading request..." << std::endl;
    _socket.receiveBytes(_pBuffer, MSG_BUFFER_SIZE);
    std::string s(_pBuffer);

    // Start receipt handshake
    if (s.find("pubkey") != std::string::npos) {
        std::cout << "Starting receipt handshake..." << std::endl;

        std::string key_path, pub_key_fd, priv_key_fd, received_dir,
            output_file;
        keys_cfgs.lookupValue("keys_dir", key_path);
        keys_cfgs.lookupValue("public_key_name", pub_key_fd);
        receipts_cfgs.lookupValue("receipts_received_dir", received_dir);
        cfg->lookupValue("file_system.general.rebuilt_fd", output_file);

        // Load public key from disc
        std::ifstream pub_key_in(pub_key_fd);
        std::istream_iterator<unsigned char> pub_start(pub_key_in), pub_end;
        std::vector<unsigned char> pub_key_bin(pub_start, pub_end);

        // Send public key to peer
        std::cout << "Sending public key..." << std::endl;
        _socket.sendBytes(pub_key_bin.data(), pub_key_bin.size());

        // Recieve encrypted receipt
        // FIXME: Decide what/how we will know the encrypted receipt size
        // For now we will just use the arbitrarily large MSG_BUFFER_SIZE
        std::cout << "Recieving encrypted receipt..." << std::endl;
        std::vector<unsigned char> enc_receipt(MSG_BUFFER_SIZE, 0);
        _socket.receiveBytes(enc_receipt.data(), enc_receipt.size());

        // Decrypt receipt
        std::cout << "Decrypting receipt..." << std::endl;
        std::vector<unsigned char> receipt = decrypt_receipt(enc_receipt);

        Writer w;
        std::cout << "Loading receipt..." << std::endl;
        w.second = new CryptoReceipt(receipt);
        std::cout << "Gathering shards..." << std::endl;
        auto ch = global_node->doGather(*w.second);
        w.first = &ch;

        // Report error if shards not found
        if (w.first->get_file_size() != 0) {
            // // Decoding pipeline
            // std::cout << "Decoding shards..." << std::endl;
            // Pipeline<Decoder> p_decode;
            // p_decode.add(3, EntanglementDecoder(), ConcurrentDecoder(),
            //              PolarDecoder());
            // p_decode.run(w);

            w.first->rebuild(output_file +
                             std::to_string(get_rand_seed_uint32_t()));
        } else {
            std::cout << "Error: Some shards were not found on the network."
                      << std::endl
                      << "Shards need more time to propogate or" << std::endl
                      << "the cryptographic receipt may be corrupted."
                      << std::endl;
        }

        delete w.second;
    } else {
        std::string err("Invalid request");
        _socket.sendBytes(err.data(), err.size());
    }

    delete[] _pBuffer;
    delete this;
}

std::vector<unsigned char> ReceiptConnectionHandler::decrypt_receipt(
    std::vector<unsigned char> enc_receipt) {
    std::ifstream priv_key_in("../.vin/self.priv");
    std::istream_iterator<unsigned char> priv_start(priv_key_in), priv_end;
    std::vector<unsigned char> priv_key_bin(priv_start, priv_end);

    // TODO: Decrypt the receipt with the private key
    return enc_receipt;
}

/**
 * Handles receipt server shutdown.
 */
void ReceiptConnectionHandler::onShutdown(
    const P::AutoPtr<P_N::ShutdownNotification>& pNf) {
    std::cout << "goodbye" << std::endl;
    delete this;
}
