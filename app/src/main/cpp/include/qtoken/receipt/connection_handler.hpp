#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include <functional>
#include <iostream>
#include <libconfig.h++>

#include "Poco/Exception.h"
#include "Poco/NObserver.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Thread.h"
#include "Poco/Util/ServerApplication.h"

#include "globals/globals.hpp"
#include "globals/logger.hpp"
#include "nodes/node.hpp"

namespace P = Poco;
namespace P_N = P::Net;
namespace P_U = P::Util;

namespace Qtoken {

class Node;
// extern Node* global_node;

/**
 * ConnectionHandler handles new connections into the TCP socket by attaching
 * event handlers.
 * @param socket the TCP socket
 * @param reactor Poco reactor for adding events
 */
class ReceiptConnectionHandler {
public:
    ReceiptConnectionHandler(P_N::StreamSocket& socket,
                             P_N::SocketReactor& reactor);
    ~ReceiptConnectionHandler() noexcept(false);
    void onReadable(const P::AutoPtr<P_N::ReadableNotification>& pNf);
    void onShutdown(const P::AutoPtr<P_N::ShutdownNotification>& pNf);

private:
    std::vector<unsigned char> decrypt_receipt(
        std::vector<unsigned char> enc_receipt);

    // read 1 byte for request message
    enum { MSG_BUFFER_SIZE = 35000 };
    int i;
    Node* node;
    P_N::StreamSocket _socket;
    P_N::SocketReactor& _reactor;
    int bytes_received;
};

}  // namespace Qtoken

#endif  // CONNECTION_HANDLER_H
