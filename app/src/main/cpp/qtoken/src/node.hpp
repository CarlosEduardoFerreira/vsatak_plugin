#ifndef NODE_H
#define NODE_H

#include "Poco/Condition.h"
#include "Poco/NamedMutex.h"
#include "Poco/Net/ParallelSocketAcceptor.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/ServerApplication.h"

#include "ConnectionHandler.hpp"
#include "concurrent_decoder.hpp"
#include "concurrent_encoder.hpp"
#include "crypto_receipt.hpp"
#include "decoder.hpp"
#include "encoder.hpp"
#include "entanglement_decoder.hpp"
#include "entanglement_encoder.hpp"
#include "host.hpp"
#include "pipeline.cpp"
#include "pipeline.hpp"
#include "polar_decoder.hpp"
#include "polar_encoder.hpp"
#include "receipt_session.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace P = Poco;
namespace P_N = P::Net;
namespace P_U = P::Util;

namespace Qtoken {

class ReceiptServer;
class ReceiptConnectionHandler;

struct PocoServer {
    P_N::SocketReactor* reactor;
    P_N::ServerSocket* socket;
    P_N::ParallelSocketAcceptor<ReceiptConnectionHandler, SocketReactor>*
        acceptor;
};

class Node : public Host {
    // TCP Connection Handler needs access to keys, reassess?
    friend class ReceiptConnectionHandler;

private:
    Keypair keypair;
    std::shared_ptr<kademlia::session> node;
    int node_port;
    int node_receipt_port;
    P_N::SocketReactor reactor;
    P_N::ServerSocket svs;
    P_N::ParallelSocketAcceptor<ReceiptConnectionHandler, SocketReactor>
        acceptor;
    P::Thread receipt_thread;

public:
    // Command parsing
    void processInput() override;
    void printCommands() override;
    int handleCommands(std::stringstream& input);

    // Network functions
    std::vector<char> doGet(const std::string& key);
    void doPut(const std::string& key, const std::vector<char>& value);
    CryptoReceipt doSpread(const std::string& file_path);
    CryptoReceipt doShare(const std::string& file_path,
                          const std::string& peer_ip,
                          const std::string& peer_port);

    void rebuild(std::vector<std::string> keys, std::string output_file);
    void runReceiptEndpoint();
    void encryptReceipt(std::ifstream& receipt_path, unsigned char* public_key,
                        unsigned char* ciphertext);
    const std::string decryptReceipt(std::ifstream& enc_receipt,
                                     unsigned char* public_key,
                                     unsigned char* private_key);

public:
    // these are made public to avoid having access errors on out of scope
    // functions NEEDS Revisiting
    unsigned char cipher_text[1024];
    int cipher_text_size;
    unsigned char recipient_public_key[crypto_box_PUBLICKEYBYTES];
    unsigned char recipient_message[1024];
    // overload Host constructor
    Node(const std::string& node_port, const std::string& node_receipt_port,
         const std::string& bootstrap_addr, std::istream* input_file);
    ~Node() {}
    Chunker doGather(const std::string& receipt_file_path);
    Chunker doGather(CryptoReceipt cr);
    // Override run
    void run() override;
};

}  // namespace Qtoken

#endif
