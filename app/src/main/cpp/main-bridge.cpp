#include <string.h>
#include <jni.h>
#include <dlfcn.h>
#include <string>
#include <android/log.h>
#include <iostream>
#include <fstream>
#include "include/qtoken/globals/globals.hpp"
#include "include/qtoken/globals/logger.hpp"

#include "include/qtoken/nodes/node.hpp"

using namespace Qtoken;
using namespace std;

string test_file = "test.txt";

extern "C" {

#if defined(__ANDROID__)
void QLog(const char *str) {
    __android_log_print(ANDROID_LOG_DEBUG, "###", "%s", str);
}
#else
void QLog(const char *str) {
    cout << str;
}
#endif



JNIEXPORT void JNICALL
Java_com_atakmap_android_plugintemplate_Qtoken_run(JNIEnv *env, jclass clazz) {

    string node_port = "8082";
    string receipt_port = "9092";
    string addr = "0.0.0.0:8000";

    //init_logger("main_log", "GlobalLogger", 7);

    QLog("###QTOKEN 1 | Start");

    try {

        string cfg_file = "defaults.cfg";

        struct stat st = {0};
        if (stat(cfg_file.data(), &st) == -1) {
            QLog("###QTOKEN 2 | Cannot fond file");
            //std::cout << "Cannot find file: " << cfg_file << std::endl;
        }

        libconfig::Config *cfg = new libconfig::Config();
        cfg->readFile("defaults.cfg");

        const libconfig::Setting& settings = cfg->getRoot();
        const libconfig::Setting& keys = settings["file_system"]["keys"];

        std::string key_path;
        keys.lookupValue("keys_dir", key_path);

        string txt = "###QTOKEN 3 | " + key_path;

        QLog(txt.data());

        Node *node = new Node(node_port, receipt_port, addr, true,*cfg);

        //node->run();

    } catch (const libconfig::FileIOException& fioex) {
//        std::cerr << "###QTOKEN" << config_io_failure << "/sdcard/VIN/defaults.cfg"
//                  << "\"" << std::endl;
        QLog("###QTOKEN 4 | FileIOException");

    } catch (const libconfig::ParseException& pex) {
//        std::cerr << "###QTOKEN" << "### Parse error at " << pex.getFile() << ":" << pex.getLine()
//                  << "-" << pex.getError() << std::endl;
        QLog("###QTOKEN 5 | ParseException");

    } catch (...) {
        QLog("###QTOKEN 6 | ...");
    }

    QLog("###QTOKEN 7 | End");




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
//
//    string test_file_12 = R"(\home\carlos\virgil\4.1.1.17\atak-civ\plugins\vintak2\app\src\main\cpp\test2.txt)";
//    string test_file_2 = R"(src/main/cpp/test.txt)";
//    string test_file_3 = R"(main/cpp/test.txt)";
//    string test_file_4 = R"(cpp/test.txt)";
//    string test_file_5 = "test.txt";
//
//    string test_file_13 = "\\home\\carlos\\virgil\\4.1.1.17\\atak-civ\\plugins\\vintak2\\app\\src\\main\\cpp\\test2.txt";
//
//    read_file_5(test_file_11);
//    usleep(1000);
//    read_file_5(test_file_12);
//    usleep(1000);
//    read_file_5(test_file_13);

    //read_file_1();
    //usleep(1000);
    //read_file_2();
    //usleep(1000);
//    read_file_4(test_file_1);
//    usleep(1000);
//    read_file_4(test_file_2);
//    usleep(1000);
//    read_file_4(test_file_3);
//    usleep(1000);
//    read_file_4(test_file_4);
//    usleep(1000);
//    read_file_4(test_file_5);

    //__ndk1::string addr23 = "test23";

    //cout << input;

    __android_log_print(ANDROID_LOG_DEBUG, "###", "The value is 23");

    //Node *node = new Node();
    //Node *node = new Node(node_port, receipt_port, addr, input);

    //"/home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak2/app/libs/arm64-v8a/libqtoken.so"

//    void* dlh = dlopen("libqtoken.so", RTLD_NOW | RTLD_GLOBAL);
//    char *dl_error = dlerror();
//    if(dl_error != nullptr) {
//        std::cerr << "###QTOKEN - Caught an error while opening shared library: " << dl_error << std::endl;
//    }
//
//    dlsym(dlh, "run");

    //dlsym(dlh, "doShare");

}

//
//    JNIEXPORT jstring JNICALL Java_com_atakmap_android_plugintemplate_MainBridgeCPP_stringFromCPP(JNIEnv* env, jobject thiz )
//    {
//
//        return env->NewStringUTF("Hi from CPP");
//    }
//
//    JNIEXPORT void JNICALL Java_com_atakmap_android_plugintemplate_MainBridgeCPP_run(JNIEnv* env, jobject thiz )
//    {
//
////        void* libqtoken = dlopen("libqtoken.so", RTLD_LAZY | RTLD_GLOBAL);
////        char *err = dlerror();
////        if (err) {
////            printf("###QTOKEN - Could not resolve symbol: %s\n", err);
//
//
////        }
//
//        int num = 1;
//
//        __android_log_print(ANDROID_LOG_DEBUG, "###QTOKEN 2", "Could not resolve symbol %d\n", num);
//
//        void* dlh = dlopen("libqtoken.so", RTLD_NOW | RTLD_GLOBAL);
//        char *dl_error = dlerror();
//        if(dl_error != nullptr) {
//            std::cerr << "###QTOKEN - Caught an error while opening shared library: " << dl_error << std::endl;
//        }
//
////        if (!dlh)
////        { fprintf(stderr, "###QTOKEN - dlopen failed: %s\n", dlerror());
////            exit(EXIT_FAILURE); };
//
//        dlsym(dlh, "run");
//        dlsym(dlh, "doShare");
//
//
//    }

//    JNIEXPORT jobject JNICALL Java_com_atakmap_android_plugintemplate_MainBridgeCPP_doShare(JNIEnv* env, jobject thiz )
//    {
//        typedef void (*do_share_t)();
//
//        // std::string app_path = "/home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak2/app";
//        // const char *libqtoken = app_path + "/libs/arm64-v8a/libqtoken.so";
//
//        //Node node = Node.run();
//
//        //LOGD("###QTOKEN - 1");
//
//        void* libqtoken = dlopen("libqtoken.so", RTLD_LAZY | RTLD_GLOBAL);
//        char *dl_error = dlerror();
//        if(dl_error != nullptr) {
//            std::cerr << "###QTOKEN - Caught an error while opening shared library: " << dl_error << std::endl;
//        }
//
////        auto do_share = dlsym(libqtoken, "doShare");
////        char *err = dlerror();
////        if (err) {
////            printf("###QTOKEN - Could not resolve symbol: %s\n", err);
////        }
//
//        //printf("Plugin object returned: %c\n", do_share());
//
//    }


//
//
//    JNIEXPORT int JNICALL Java_com_atakmap_android_plugintemplate_MainBridgeCPP_main(JNIEnv *env, jobject thiz) {
//        return 23;
//    }


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
