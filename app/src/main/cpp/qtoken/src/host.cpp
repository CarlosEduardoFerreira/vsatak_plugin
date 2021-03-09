#include "host.hpp"
#include "strings.hpp"

using namespace Qtoken;
using namespace std;

/** Base class constructor
 *
 * @param add Bootstrap address.
 * @param inp input stream for passing commands to the node.
 */
Host::Host(const string& add, istream* inp) {
    // Consider making this address into a struct for ease of passing
    auto separatorIdx = add.find(':');
    boot_address = add.substr(0, separatorIdx);
    boot_port = add.substr(separatorIdx + 1);
    input = inp;
}

// Helpers
void discardBlanks(std::istream& input) {
    while (input.good() && isblank(input.peek())) {
        input.get();
    }
}

std::string readArg(std::stringstream& input) {
    std::string quoted;

    discardBlanks(input);

    if (!input.good()) {
        return std::string();
    }

    if (input.peek() == '"') {
        input.ignore(1);
        getline(input, quoted, '"');
    } else {
        input >> quoted;
    }

    return quoted;
}

const kademlia::session_base::data_type asBinary(const std::string& value) {
    return kademlia::session::data_type{value.begin(), value.end()};
}
