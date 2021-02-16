#include "crypto_receipt.hpp"

using namespace Qtoken;

CryptoReceipt::CryptoReceipt(std::string file_path) {
    load(file_path);
}

CryptoReceipt::CryptoReceipt(std::vector<unsigned char> receipt_bytes) {
    std::string receipt_str(receipt_bytes.begin(), receipt_bytes.end());
    std::stringstream ss;
    ss.str(receipt_str);

    std::string k, v;
    while (ss >> k >> v) {
        data[Key(k)] = Val(v);
    }
}

void CryptoReceipt::save(std::string dir_path) {
    std::ofstream output(dir_path);
    auto ss = this->serialize();
    output << ss.str();
}

void CryptoReceipt::load(std::string file_path) {
    std::ifstream input(file_path);
    std::string k, v;
    while (input >> k >> v) {
        data[Key(k)] = Val(v);
    }
}

bool CryptoReceipt::operator==(CryptoReceipt& rhs) {
    for (const auto& [key, value] : data) {
        if (rhs.get(key) != value)
            return false;
    }
    return true;
}

std::stringstream CryptoReceipt::serialize() {
    std::stringstream ss;
    for (const auto& [key, value] : data) {
        ss << key << " " << value << '\n';
    }
    return ss;
}
