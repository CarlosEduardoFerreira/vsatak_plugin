#include <gflags/gflags.h>

#include <iostream>

#include "bootstrap.hpp"
#include "configs.hpp"
#include "globals.hpp"
#include "node.hpp"
#include "utils.hpp"

using namespace Qtoken;
using namespace std;

static bool ValidatePort(const char* flagname, int32_t value) {
    if (value > 0 && value < 32768)
        return true;
    std::cout << "Invalid value for --" << flagname << ": " << (int)value
              << std::endl;
    return false;
}
DEFINE_string(a, "", "Address for the bootstrap node.");
DEFINE_string(
    t, "",
    "Path to input file for commands to be run by the node or bootstrap.");
DEFINE_string(config, "/etc/opt/VIN/defaults.cfg",
              "Path to config file for this program.");
DEFINE_bool(b, false, "A Flag that indicates we are to run a bootstrap node.");
DEFINE_int32(p, 8080, "The port that the node should be started on.");
DEFINE_validator(p, &ValidatePort);
DEFINE_int32(r, 9090,
             "The port that the node listens on for incoming receipts.");
DEFINE_validator(r, &ValidatePort);

int main(int argc, char* argv[]) {
    int testmode = 0;
    int port;
    int r_port;
    std::string bootstrap_addr = "";
    fstream fileinput;

    // Handle flags passed in
    gflags::SetUsageMessage(
        "Set up a qtoken node for secure file transmission across a Virgil "
        "systems network.");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    /********-Start config Management-**********/
    try {
        cfg->readFile(FLAGS_config.c_str());
    } catch (const libconfig::FileIOException& fioex) {
        std::cerr << "I/O error while reading config file \"" << FLAGS_config
                  << "\"" << std::endl;
        return (EXIT_FAILURE);
    } catch (const libconfig::ParseException& pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << "-" << pex.getError() << std::endl;
        return (EXIT_FAILURE);
    }
    // TODO: Change this so that it checks if the flag is used. Currently it is
    //      overwritting the user input with the config from the config file.
    FLAGS_a = (std::string)(cfg->lookup("default_bootstrap_address"));
    std::string version = cfg->lookup("vin_version");
    gflags::SetVersionString(
        version);  // Idk what version we are on, someone correct me
    /********-End config Management-**********/

    if (!FLAGS_t.empty()) {
        std::string fd = FLAGS_t;
        try {
            fileinput.open(fd, std::ifstream::in);
        } catch (const ifstream::failure& e) {
            std::cout << "Invalid test filename provided" << std::endl;
        }
        testmode = 1;
    }

    istream& input = (testmode == 0) ? std::cin : fileinput;

    std::string release_year = "2020";
    std::cout << std::endl
              << std::endl
              << "Virgil Integrity Network\u00AE " + release_year << std::endl;

    bootstrap_addr = FLAGS_a;

    if (FLAGS_b) {
        Bootstrap boot(bootstrap_addr, &input);
        boot.run();
        // boot.lvm_close();
    } else {
        int port;
        port = FLAGS_p;
        r_port = FLAGS_r;
        Node node(to_string(port), to_string(r_port), bootstrap_addr, &input);

        // make node globally accessible
        global_node = &node;

        node.run();
        // node.lvm_close();
    }

    gflags::ShutDownCommandLineFlags();
    delete cfg;
    return 0;
}
