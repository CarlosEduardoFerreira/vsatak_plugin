#include <string.h>
#include <jni.h>
#include <dlfcn.h>
#include <string>
#include <android/log.h>
#include <iostream>

#include "include/qtoken/nodes/node.hpp"

using namespace Qtoken;


extern "C" {

JNIEXPORT void JNICALL
Java_com_atakmap_android_plugintemplate_Qtoken_run(JNIEnv *env, jclass clazz) {


//    const std::string& node_port, const std::string& node_receipt_port,
//    const std::string& bootstrap_addr, std::istream* input_file

    std::string node_port;
    std::string receipt_port;
    std::string addr;
    std::istream* input;


    Node *node = new Node(node_port, receipt_port, addr, input);


    //__android_log_print(ANDROID_LOG_DEBUG, "###QTOKEN 2", "Could not resolve symbol %d\n", num);

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
