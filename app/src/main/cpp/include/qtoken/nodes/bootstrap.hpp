#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include "../nodes/host.hpp"

namespace Qtoken {

/**
 * Implentation of Qtoken Host class : Bootstrap Node
 *
 * Bootstrap for the VIN network. Starts a kademlia first session.
 *
 */
class Bootstrap : public Host {
private:
    std::shared_ptr<kademlia::first_session> firstNode;

protected:
    void processInput() override;
    void printCommands() override;
    int handleCommands(std::stringstream& input);

public:
    Bootstrap(const std::string& add, std::istream* inp);
    void run() override;
};

}  // namespace Qtoken

#endif // BOOTSTRAP_H