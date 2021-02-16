#include <gtest/gtest.h>

#include "node.hpp"
#include "test_utils.hpp"

// TEST(HostTest, HostReceiptEndDecWorks) {
//     auto test_file = get_test_file();
//     auto receipt_file = get_test_file();

//     const std::string p("3505");
//     const std::string rp("3505");
//     const std::string b_addr("0.0.0.0:8000");
//     Qtoken::Node n(p, rp, b_addr, &test_file);

//     unsigned char publicKey[crypto_box_PUBLICKEYBYTES];
//     unsigned char privateKey[crypto_box_SECRETKEYBYTES];
//     unsigned char cipher_text[1024];

//     int keygen = crypto_box_keypair(publicKey, privateKey);

//     ASSERT_EQ(keygen, 0);

//     n.encryptReceipt(receipt_file, publicKey, cipher_text);
// }
