#include "host.hpp"
#include "strings.hpp"

using namespace Qtoken;
using namespace std;

Host::Host(const string& add, istream* inp)
    : lvm_socket(0) {
    // Consider making this address into a struct for ease of passing
    auto separatorIdx = add.find(':');
    boot_address = add.substr(0, separatorIdx);
    boot_port = add.substr(separatorIdx + 1);

    std::string port = cfg->lookup("default_lvm_port");
    lvm_port = port;

    input = inp;
    //lvm_connect();
}

bool Host::lvm_connect(bool udp /*= false*/) {
    // Open a socket if there isn't one yet

    if (lvm_socket_empty()) {
        if (udp) {
            lvm_socket = socket(AF_INET, SOCK_DGRAM, 0);
        } else {
            lvm_socket = socket(AF_INET, SOCK_STREAM, 0);
        }
        if (lvm_socket_empty()) {
            perror(create_socket_failed);
        }
        cout << "New lvm socket [" << lvm_socket << "] created" << endl;
    } else {
        cout << "Socket [" << lvm_socket << "] already open for this node"
             << endl;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(stoi(lvm_port));
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    cout << attmpt_socket_conn << endl;

    // if (connect(lvm_socket, (const struct sockaddr*)&server, sizeof(server))
    // <
    //     0) {
    //     perror("Server is down");
    //     return false;
    // }

    cout << lvm_conn_success << endl;
    return true;
}

bool Host::lvm_socket_empty() {
    return lvm_socket < 0;
}

bool Host::lvm_send_data(string data) {
    if (lvm_socket_empty()) {
        cout << socket_not_connected << endl;
        return false;
    }
    cout << sending_data << endl;
    cout << data << endl << endl;

    sendto(lvm_socket, data.c_str(), strlen(data.c_str()), 0,
           (const struct sockaddr*)&server, sizeof(server));

    // if (send(lvm_socket, data.c_str(), strlen(data.c_str()), 0) < 0) {
    //     perror("Sending failed.");
    //     return false;
    // }
    cout << data_sent << endl;

    return true;
}

bool Host::lvm_close() {
    cout << closing_lvm << endl;
    if (close(lvm_socket) < 0) {
        cout << closing_lvm_failed << endl;
        return false;
    }

    return true;
}

string Host::lvm_receive_data(int size /*= 512*/) {
    char buffer[size];
    struct sockaddr_storage remote_address;
    socklen_t address_length;
    ssize_t number_of_bytes{0};
    // string reply;

    // UDP INET recv
    if ((number_of_bytes = recvfrom(lvm_socket, buffer, sizeof(buffer), 0,
                                    (struct sockaddr*)&remote_address,
                                    &address_length)) == -1) {
        perror(recvfrom_failed);
    }

    // UNIX recv
    // if (recv(lvm_socket, buffer, sizeof(buffer), 0) < 0) {
    //     puts("receive failed");
    //     return NULL;
    // }

    std::string reply(buffer);

    return reply;
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