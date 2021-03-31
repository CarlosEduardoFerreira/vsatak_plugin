#include <string.h>
#include <jni.h>
#include <dlfcn.h>
#include <string>
#include <android/log.h>
#include <iostream>
#include <fstream>

#include "Poco/AutoPtr.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/SimpleFileChannel.h"

#include "Poco/Foundation.h"
#include "Poco/Channel.h"

#include "include/qtoken/globals/globals.hpp"
#include "include/qtoken/globals/logger.hpp"
#include "include/qtoken/nodes/node.hpp"
#include "include/qtoken/receipt/crypto_receipt.hpp"

using Poco::AutoPtr;
using Poco::FormattingChannel;
using Poco::Logger;
using Poco::PatternFormatter;
using Poco::SimpleFileChannel;
using Poco::Channel;
using Poco::Formatter;

using namespace Qtoken;
using namespace std;

string test_file = "test.txt";

Node *node;

extern "C" {

JNIEXPORT void JNICALL
Java_com_atakmap_android_plugintemplate_Qtoken_run(JNIEnv *env, jclass clazz) {

    string node_port = "8082";
    string receipt_port = "9092";
    string addr = "192.168.1.10:8000";

    try {

        string cfg_file = "/sdcard/VIN/defaults.cfg";

        struct stat st = {0};
        if (stat(cfg_file.data(), &st) == -1) {
            log_message("###QTOKEN 2 | Cannot find the file");
            //std::cout << "Cannot find file: " << cfg_file << std::endl;
        }
        log_message("###QTOKEN 1.1 | libconfig");
//        libconfig::Config *cfg = new libconfig::Config();

        log_message("###QTOKEN 1.2 | readFile before");
        cfg->readFile(cfg_file.c_str());
        log_message("###QTOKEN 1.3 | readFile after");

        const libconfig::Setting& settings = cfg->getRoot();
        const libconfig::Setting& keys = settings["file_system"]["keys"];

        std::string key_path;
        keys.lookupValue("keys_dir", key_path);

        string txt = "###QTOKEN 3 | " + key_path;

        log_message(txt.data());

        node = new Node(node_port, receipt_port, addr, true, *cfg);

        log_message("###QTOKEN  | run before");
        node->run();
        log_message("###QTOKEN  | run after");

    } catch (...) {
        log_message("###QTOKEN 6 | ...");
    }
}





JNIEXPORT void JNICALL
Java_com_atakmap_android_plugintemplate_Qtoken_put(JNIEnv *env, jclass clazz) {


    log_message("###QTOKEN 1 | Start");

    try {

        //init_logger("main_log", "GlobalLogger", 7);

        std::string k = "Hello";
        std::string v = "World";
        std::vector vec(v.begin(), v.end());

        log_message("###QTOKEN  | put before");
        node->doPut(k, vec);
        log_message("###QTOKEN  | put after");

    } catch (const libconfig::FileIOException &fioex) {
//        std::cerr << "###QTOKEN" << config_io_failure << "/sdcard/VIN/defaults.cfg"
//                  << "\"" << std::endl;
        log_message("###QTOKEN 4 | FileIOException");

    } catch (const libconfig::ParseException &pex) {
//        std::cerr << "###QTOKEN" << "### Parse error at " << pex.getFile() << ":" << pex.getLine()
//                  << "-" << pex.getError() << std::endl;
        log_message("###QTOKEN 5 | ParseException");

    } catch (...) {
        log_message("###QTOKEN 6 | ...");
    }

    log_message("###QTOKEN 7 | End");


}


JNIEXPORT void JNICALL
Java_com_atakmap_android_plugintemplate_Qtoken_get(JNIEnv *env, jclass clazz) {


    log_message("###QTOKEN 1 | Start");

    try {

        log_message("###QTOKEN  | get before");
        node->doGet("Hello");
        log_message("###QTOKEN  | get after");

    } catch (const libconfig::FileIOException &fioex) {
//        std::cerr << "###QTOKEN" << config_io_failure << "/sdcard/VIN/defaults.cfg"
//                  << "\"" << std::endl;
        log_message("###QTOKEN 4 | FileIOException");

    } catch (const libconfig::ParseException &pex) {
//        std::cerr << "###QTOKEN" << "### Parse error at " << pex.getFile() << ":" << pex.getLine()
//                  << "-" << pex.getError() << std::endl;
        log_message("###QTOKEN 5 | ParseException");

    } catch (...) {
        log_message("###QTOKEN 6 | ...");
    }

    log_message("###QTOKEN 7 | End");


}


JNIEXPORT void JNICALL // spread and gather
Java_com_atakmap_android_plugintemplate_Qtoken_share(JNIEnv *env, jclass clazz) {

    log_message("###QTOKEN | Share Start");

    string test_file_5 = "/sdcard/test.txt";

    log_message("###QTOKEN | Share before");
    node->doShare(test_file_5, "192.168.1.10", "9091");
    log_message("###QTOKEN | Share after");


}


JNIEXPORT void JNICALL // calls put
Java_com_atakmap_android_plugintemplate_Qtoken_spread(JNIEnv *env, jclass clazz) {

    log_message("###QTOKEN | Spread Start");

    string test_file = "/sdcard/test.txt";

    log_message("###QTOKEN | Spread before");
    auto receipt = node->doSpread(test_file);
    log_message("###QTOKEN | " + receipt.serialize().str());
    receipt.save("/sdcard/VIN/receipts/sent/test.txt");
    log_message("###QTOKEN | Spread after");

}


JNIEXPORT void JNICALL // calls get
Java_com_atakmap_android_plugintemplate_Qtoken_gather(JNIEnv *env, jclass clazz) {

    log_message("###QTOKEN | Gather Start");

    string test_file = "/sdcard/VIN/receipts/sent/test.txt";

    log_message("###QTOKEN | Gather before");
    auto ch = node->doGather(test_file);
    ch.rebuild("/sdcard/VIN/outputs/test.txt");

    log_message("###QTOKEN | Gather after");

}

//    const std::string& node_port, const std::string& node_receipt_port,
//    const std::string& bootstrap_addr, std::istream* input_file

//    filebuf fb;
//    if (fb.open ("test.txt",ios::in)) {
//        istream is(&fb);
//        if (!is.empty()) {
//            std::string fd = FLAGS_t;
//            try {
//                fileinput.open(fd, std::ifstream::in);
//            } catch (const ifstream::failure& e) {
//                std::cout << "Invalid test filename provided" << std::endl;
//            }
//            testmode = 1;
//        }
//    }
//    istream& input = (testmode == 0) ? std::cin : fileinput;

    //aprint2(std::filesystem::current_path().string());


//
//    string test_file_11 = "/home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak2/app/src/main/cpp/test2.txt";
////    string test_file_2 = "src/main/cpp/test.txt";
////    string test_file_3 = "main/cpp/test.txt";
////    string test_file_4 = "cpp/test.txt";
////    string test_file_5 = "test.txt";



/*
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(0);
    }
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    //send(new_socket , qtoken , strlen(qtoken) , 0 );
    printf("message sent\n");
    */

















}
