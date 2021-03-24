#include <string.h>
#include <jni.h>
#include <dlfcn.h>
#include <string>
#include <android/log.h>
#include <iostream>
#include <fstream>

#include "include/qtoken/nodes/node.hpp"

using namespace Qtoken;
using namespace std;

string test_file = "test.txt";

extern "C" {

void aprint(const char *str) {
    __android_log_print(ANDROID_LOG_DEBUG, "###", "%s", str);
}

void aprint2(basic_string<char, char_traits<char>, allocator<char>> str) {
    __android_log_print(ANDROID_LOG_DEBUG, "###", "%s", str.c_str());
}

int read_file_1() {
    ifstream is(test_file, ifstream::binary);
    aprint("get_file_1 1");
    if (is) {
        // get length of file:
        is.seekg(0, is.end);
        int length = is.tellg();
        is.seekg(0, is.beg);

        char *buffer = new char[length];

//        cout << "Reading " << length << " characters... ";
        aprint("get_file_1 2");
        // read data as a block:
        is.read(buffer, length);

        if (is) {// <== this is really odd
            aprint("get_file_1 3");
//            cout << "all characters read successfully.";
        } else {
            aprint("get_file_1 4");
//            cout << "error: only " << is.gcount() << " could be read";
        }
        is.close();

        // ...buffer contains the entire file...

        delete[] buffer;
    }
    aprint("get_file_1 5");
}

void read_file_2(const string& file_name) {
    std::ifstream file(file_name);
    aprint("read_file_2 1");
//    if (file.is_open()) {
        aprint("read_file_2 2");
        std::string line;
        while (std::getline(file, line)) {
            aprint("read_file_2 3");
            // using printf() in all tests for consistency
            //printf("%s", line.c_str());
            aprint(line.c_str());
        }
        file.close();
//    }
    aprint("read_file_2 4");
}

void read_file_3(const string& file_name) {
    filebuf fb;
    aprint("read_file_3 1");
    if (fb.open (file_name,ios::in))
    {
        aprint("read_file_3 2");
        istream is(&fb);
        while (is) {
            aprint("read_file_3 3");
            aprint(reinterpret_cast<const char *>(char(is.get())));
//            cout << char(is.get());
        }
        fb.close();
    }
    aprint("read_file_3 4");
}

void read_file_4(string file) {
    try {
        std::ifstream f(file);

        //const char *txt = "read_file_4 1 (" + num + ")";

        aprint("read_file_4 1");

        //f.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

        if (f.is_open()) {
            aprint("read_file_4 2");
            aprint(reinterpret_cast<const char *>(f.rdbuf()));
//        std::cout << f.rdbuf();
        }


        ifstream myfile;
        myfile.open(file.c_str());
        if (myfile.fail()) {
            aprint("read_file_4 3");
//        cerr << "Could not open file: " << file << endl;
        } else {
            aprint("read_file_4 4");
        }

    } catch (std::ios_base::failure& f) {
        aprint(f.what());
        //std::cerr << "Exception opening file: " << std::strerror(errno) << "\n";
    }

    aprint("read_file_4 5");
}

void read_file_5(string file) {
    std::ifstream f(file);

    //const char *txt = "read_file_4 1 (" + num + ")";

    aprint("read_file_4 1");

    //f.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    if (f.is_open()) {
        aprint("read_file_4 2");
        aprint(reinterpret_cast<const char *>(f.rdbuf()));
//        std::cout << f.rdbuf();
    }

    aprint("read_file_4 3");
}


JNIEXPORT void JNICALL
Java_com_atakmap_android_plugintemplate_Qtoken_run(JNIEnv *env, jclass clazz) {


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

    string node_port = "8082";
    string receipt_port = "9092";
    string addr;
    //istream* input = get_file_2();

    string test_file_11 = "/home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak2/app/src/main/cpp/test2.txt";
//    string test_file_2 = "src/main/cpp/test.txt";
//    string test_file_3 = "main/cpp/test.txt";
//    string test_file_4 = "cpp/test.txt";
//    string test_file_5 = "test.txt";

    string test_file_12 = R"(\home\carlos\virgil\4.1.1.17\atak-civ\plugins\vintak2\app\src\main\cpp\test2.txt)";
    string test_file_2 = R"(src/main/cpp/test.txt)";
    string test_file_3 = R"(main/cpp/test.txt)";
    string test_file_4 = R"(cpp/test.txt)";
    string test_file_5 = "test.txt";

    string test_file_13 = "\\home\\carlos\\virgil\\4.1.1.17\\atak-civ\\plugins\\vintak2\\app\\src\\main\\cpp\\test2.txt";

    read_file_5(test_file_11);
    usleep(1000);
    read_file_5(test_file_12);
    usleep(1000);
    read_file_5(test_file_13);

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
