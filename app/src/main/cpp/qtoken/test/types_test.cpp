#include <gtest/gtest.h>

#include "crypto_receipt.hpp"
#include "types.hpp"

using namespace Qtoken;

TEST(TypesTest, CryptoReceiptPutEmpty) {
    CryptoReceipt* cr = new CryptoReceipt;
    cr->put(Key(""), Val(""));
    delete cr;
}

TEST(TypesTest, CryptoReceiptPutVal) {
    CryptoReceipt* cr = new CryptoReceipt;
    cr->put(Key("A"), Val("B"));
    delete cr;
}

// FIXME: this works now but should return a collision
TEST(TypesTest, CryptoReceiptPutValTwice) {
    CryptoReceipt* cr = new CryptoReceipt;
    cr->put(Key("A"), Val("B"));
    cr->put(Key("A"), Val("A"));
    delete cr;
}

TEST(TypesTest, CryptoReceiptGetEmpty) {
    CryptoReceipt* cr = new CryptoReceipt;
    cr->put(Key(""), Val(""));
    auto val = cr->get(Key(""));

    ASSERT_EQ(val, "");

    delete cr;
}

TEST(TypesTest, CryptoReceiptGetVal) {
    CryptoReceipt* cr = new CryptoReceipt;
    cr->put(Key("A"), Val("B"));
    auto val = cr->get(Key("A"));

    ASSERT_EQ(val, "B");

    delete cr;
}

TEST(TypesTest, CryptoReceiptGetNull) {
    CryptoReceipt* cr = new CryptoReceipt;
    auto val = cr->get(Key("A"));

    ASSERT_EQ(val, "");

    delete cr;
}
