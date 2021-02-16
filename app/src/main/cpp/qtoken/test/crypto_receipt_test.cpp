#include <gtest/gtest.h>

#include "crypto_receipt.hpp"

using namespace Qtoken;

TEST(ReceiptTest, BuildFromFileTest) {
    CryptoReceipt cr;
    cr.put("shard1", "shard1sha256val");
    cr.put("shard2", "shard1sha256val");
    cr.put("shard3", "shard1sha256val");
    cr.save("./cr.txt");

    CryptoReceipt cr_cp("./cr.txt");

    std::system("rm cr.txt");

    EXPECT_TRUE(cr == cr_cp);
}

TEST(ReceiptTest, LoadFromFileTest) {
    CryptoReceipt cr;
    cr.put("shard1", "shard1sha256val");
    cr.put("shard2", "shard1sha256val");
    cr.put("shard3", "shard1sha256val");
    cr.save("./cr.txt");

    CryptoReceipt cr_cp;
    cr_cp.load("./cr.txt");

    std::system("rm cr.txt");

    EXPECT_TRUE(cr == cr_cp);
}