#include "connection_handler.hpp"

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
    /* Logging */
    Poco::Logger::get("GlobalLogger")
        .information("Received new receipt connection from " +
                     socket.address().toString());
    /* Logging */
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
    _socket.receiveBytes(_pBuffer, MSG_BUFFER_SIZE);
    std::string s(_pBuffer);

    // Start receipt handshake
    if (s.find("pubkey") != std::string::npos) {
        std::cout << "Connection found..." << std::endl;

        std::string key_path, pub_key_fd, received_dir, output_file;

        // Recieve encrypted receipt
        // FIXME: Decide what/how we will know the encrypted receipt size
        // For now we will just use the arbitrarily large MSG_BUFFER_SIZE
        std::cout << "Recieving receipt..." << std::endl;
        std::vector<unsigned char> enc_receipt(MSG_BUFFER_SIZE, 0);
        _socket.receiveBytes(enc_receipt.data(), enc_receipt.size());

        Poco::Logger::get("GlobalLogger")
            .information("Receipt size:" + std::to_string(enc_receipt.size()));

        // Decrypt receipt
        // std::vector<unsigned char> receipt = decrypt_receipt(enc_receipt);

        Poco::Logger::get("GlobalLogger").information("Receipt decrypted.");

        Writer w;
        w.second = new CryptoReceipt(enc_receipt);

        std::string receipt_dir;
        cfg->lookupValue("file_system.receipts.receipts_received_dir",
                         receipt_dir);
        // Save receipt to disc with randomly generated name
        std::string save_path(receipt_dir + "CR" +
                              std::to_string(get_rand_seed_uint32_t()));
        w.second->save(save_path);
        std::cout << "Receipt path: " << save_path << std::endl;

        auto ch = global_node->doGather(*w.second);
        w.first = &ch;

        Poco::Logger::get("GlobalLogger").information("Shards gathered.");

        // Report error if shards not found
        if (w.first->get_file_size() != 0) {
            // // Decoding pipeline
            // std::cout << "Decoding shards..." << std::endl;
            // Pipeline<Decoder> p_decode;
            // p_decode.add(3, EntanglementDecoder(), ConcurrentDecoder(),
            //              PolarDecoder());
            // p_decode.run(w);

            std::string file_name = std::to_string(get_rand_seed_uint32_t());
            int shard_count = w.first->get_chunks().size();
            w.first->rebuild(output_file + file_name);
            Poco::Logger::get("GlobalLogger")
                .debug("Shard count:" + std::to_string(shard_count));
            Poco::Logger::get("GlobalLogger")
                .information("File rebuilt at " + output_file + file_name);
            std::cout << "File rebuilt at " << output_file + file_name
                      << std::endl;

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
