cmake_minimum_required(VERSION 3.18.1)

project(Main-Bridge)

set(PROJECT_APP "/home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak2/app")

# -- Log ------------------------------------------------
find_library (log-lib log)
# ------------------------------------------------ Log --

# -- QToken --------------------------------------------
add_library(qtoken SHARED IMPORTED)
set_target_properties(qtoken PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libqtoken.so
        )
target_include_directories(qtoken INTERFACE
        ${PROJECT_APP}/src/main/cpp/include/qtoken
        ${PROJECT_APP}/src/main/cpp/include/aff3ct-2.3.5
        ${PROJECT_APP}/src/main/cpp/include/kademlia
        ${PROJECT_APP}/src/main/cpp/include/libconfig
        ${PROJECT_APP}/src/main/cpp/include/openssl
        ${PROJECT_APP}/src/main/cpp/include/Poco
        ${PROJECT_APP}/src/main/cpp/include/Poco/Net
        )
target_link_libraries(
        qtoken INTERFACE
        aff3ct-2.3.5
        c++_shared
        crypto
        gflags_nothreads
        kademlia
        PocoData
        PocoDataSQLite
        PocoEncondings
        PocoFoundation
        PocoJSON
        PocoMongoDB
        PocoNet
        PocoRedis
        PocoUtil
        PocoXML
        PocoZip
        ssl
        ${log-lib}
)
# -------------------------------------------- QToken --

# -- Include --------------------------------------------
include_directories(${PROJECT_APP}/src/main/cpp/include/aff3ct-2.3.5/aff3ct)
include_directories(${PROJECT_APP}/src/main/cpp/include/aff3ct-2.3.5/MIPP)
include_directories(${PROJECT_APP}/src/main/cpp/include/aff3ct-2.3.5/rang)
include_directories(${PROJECT_APP}/src/main/cpp/include/Poco)
include_directories(${PROJECT_APP}/src/main/cpp/include/Poco/Net)
include_directories(${PROJECT_APP}/src/main/cpp/include/libconfig)
include_directories(${PROJECT_APP}/src/main/cpp/include/qtoken)
# -------------------------------------------- Include --

# -- Main Bridge ---------------------------------------
add_library(main-bridge SHARED main-bridge.cpp)
# --------------------------------------- Main Bridge --

# -- Target ---------------------------------------------
target_link_libraries(
        main-bridge INTERFACE
        qtoken
        ${log-lib}
)
# --------------------------------------------- Target --

