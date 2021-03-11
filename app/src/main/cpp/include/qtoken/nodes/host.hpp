#ifndef HOST_H
#define HOST_H

#include <arpa/inet.h>   //htons, inet_addr
#include <sys/socket.h>  //socket
#include <sys/un.h>      //sockaddr_un
#include <unistd.h>      //close
#include <fstream>       //ofstream
#include <future>
#include <iterator>
#include "../../kademlia/first_session.hpp"
#include "../../kademlia/session.hpp"

#include "../globals/globals.hpp"
#include "../globals/logger.hpp"
#include "../globals/strings.hpp"
#include "../tools/chunker.hpp"
#include "../tools/types.hpp"

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
    std::string boot_address;
    std::string boot_port;
    std::istream* input;
    virtual void processInput(){};
    virtual int handleCommands() { return 0; };
    virtual void printCommands(){};

public:
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
