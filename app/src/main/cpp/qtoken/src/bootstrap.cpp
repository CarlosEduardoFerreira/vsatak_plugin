#include "bootstrap.hpp"

using namespace Qtoken;

Bootstrap::Bootstrap(const std::string& add, std::istream* inp)
    : Host(add, inp) {}
void Bootstrap::run() {
    firstNode = std::make_shared<kademlia::first_session>(
        kademlia::endpoint{boot_address, boot_port},
        kademlia::endpoint{"::", boot_port});

    auto main_loop_result =
        async(std::launch::async, &kademlia::first_session::run, firstNode);

    std::cout << "Starting Bootstrap node DHT at port " << boot_port
              << std::endl;
    std::cout << std::endl << std::endl;
    printCommands();

    processInput();

    std::cout << "finishing..." << std::endl;
    firstNode->abort();
    main_loop_result.get();
}

void Bootstrap::processInput() {
    std::string line;
    while (true) {
        getline(*input, line);
        std::stringstream command(line);
        if (!handleCommands(command))
            break;
        (*input).clear();
    }
}
int Bootstrap::handleCommands(std::stringstream& command_line) {
    std::string command; /*, key, value, receipt_file_path, file_path;*/
    command_line >> command;

    transform(command.begin(), command.end(), command.begin(), ::tolower);

    try {
        if (command == "exit") {
            std::cout << "exiting..." << std::endl;
            return 0;
        }
        if (command == "") {
            std::cout << "\r";
            return 1;
        }
        if (command != "help") {
            std::cout << "unknown command '" << command << "'" << std::endl;
        }
        printCommands();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 1;
}
void Bootstrap::printCommands() {
    std::cout << "Type \"exit\" to finish" << std::endl;
}
