#ifndef CRYPTO_RECEIPT_H
#define CRYPTO_RECEIPT_H

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "types.hpp"

namespace Qtoken {

// FIXME: Class template for key value pair types
class CryptoReceipt {
public:
    CryptoReceipt() = default;
    CryptoReceipt(CryptoReceipt& cr) = default;
    CryptoReceipt(CryptoReceipt&& cr) = default;
    explicit CryptoReceipt(const std::string& file_path);
    explicit CryptoReceipt(std::vector<unsigned char> receipt_bytes);
    explicit CryptoReceipt(const std::unordered_map<Key, Val>& data)
        : data(data){};
    Val get(Key k) { return (data.find(k) != data.end()) ? data[k] : ""; }
    void put(const Key& k, const Val& v) { data[k] = v; }
    std::ostringstream stream();
    void save(std::string dir_path);
    void load(std::string file_path);
    std::stringstream serialize();
    bool operator==(CryptoReceipt& rhs);

private:
    std::unordered_map<Key, Val> data;
};

}  // namespace Qtoken

#endif // CRYPTO_RECEIPT_H