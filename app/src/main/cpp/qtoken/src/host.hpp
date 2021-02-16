#pragma once
#ifndef HOST_H
#define HOST_H

#include <arpa/inet.h>   //htons, inet_addr
#include <sys/socket.h>  //socket
#include <sys/un.h>      //sockaddr_un
#include <unistd.h>      //close
#include <boost/fiber/all.hpp>
#include <filesystem>
#include <fstream>  //ofstream
#include <iterator>
#include <kademlia/first_session.hpp>
#include <kademlia/session.hpp>
#include <thread>

#include "chunker.hpp"
#include "configs.hpp"
#include "types.hpp"

#define SERVER_PATH "/tmp/lvm_server.socket"
typedef boost::fibers::buffered_channel<std::vector<char>> channel_t;

namespace Qtoken {

/**
 * Qtoken base class
 *
 * Host constitutes the bare minumum functionality for a node on the VIN,
 * that is it stores the address and port of the bootstrap node, and defines and
 * input stream upon construction. Afterwards it connects to it's local LVM
 * database. Different types of VIN nodes will inherit from this class.
 *
 */
class Host {
protected:
    /*** START vars ***/
    /* data */
    std::string boot_address;
    std::string boot_port;
    std::istream* input;

    /* lvm socket data */
    int lvm_socket;
    std::string lvm_port;
    struct sockaddr_in server;
    /*** END  vars ***/

    /*** START Command line processing helpers ***/
    virtual void processInput(){};
    virtual int handleCommands() { return 0; };
    virtual void printCommands(){};
    /*** END  Command line processing helpers ***/

public:
    /*** START LVM connection helpers ***/
    bool lvm_socket_empty();
    bool lvm_connect(bool udp = false);
    bool lvm_close();
    /** Send data to LVM db.
     *
     * @param data component of interest
     */
    bool lvm_send_data(std::string data);
    /** Send data to LVM get data back from LVM db.
     *
     * @param size size of recv buffer for LVM.
     * @return outputs data received from LVM.
     */
    std::string lvm_receive_data(int size = 512);
    /*** END  LVM connection helpers ***/

    /** Base class constructor
     *
     * @param add Bootstrap address.
     * @param inp input stream for passing commands to the node.
     */
    Host(const std::string& add, std::istream* inp);

    virtual void run(){};
};

}  // namespace Qtoken

// Helper functions here
// Consider moving these into Qtoken namespace somehow?
// Was generating naming collisions before so they were moved out.
void discardBlanks(std::istream& input);
std::string readArg(std::stringstream& input);
const kademlia::session_base::data_type asBinary(const std::string& value);

#endif  // HOST_H
