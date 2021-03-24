cmake_minimum_required(VERSION 3.18.1)

project(Main-Bridge)

set(PROJECT_APP ${CMAKE_SOURCE_DIR})
set(INCLUDE_DIR ${PROJECT_APP}/include)
set(POCO_DIR ${PROJECT_APP}/poco)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -- Log ------------------------------------------------
find_library (log-lib log)
# ------------------------------------------------ Log --

# -- Include --------------------------------------------
include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_DIR}/aff3ct-2.3.5/aff3ct)
include_directories(${INCLUDE_DIR}/aff3ct-2.3.5/MIPP)
include_directories(${INCLUDE_DIR}/aff3ct-2.3.5/rang)
include_directories(${INCLUDE_DIR}/libconfig)
include_directories(${INCLUDE_DIR}/qtoken)
include_directories(${INCLUDE_DIR}/Poco)
include_directories(${INCLUDE_DIR}/Poco/Poco)
include_directories(${INCLUDE_DIR}/kademlia)

# -------------------------------------------- Include --

add_library(aff3ct SHARED IMPORTED)
set_target_properties(aff3ct PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libaff3ct-2.3.5.so
        )

add_library(config++ SHARED IMPORTED)
set_target_properties(config++ PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libconfig++.so
        )

add_library(crypto SHARED IMPORTED)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libcrypto.so
        )
add_library(ssl SHARED IMPORTED)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libssl.so
        )

add_library(c++_shared SHARED IMPORTED)
set_target_properties(c++_shared PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libc++_shared.so
        )

add_library(gflags_nothreads SHARED IMPORTED)
set_target_properties(gflags_nothreads PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libgflags_nothreads.so
        )

add_library(kademlia SHARED IMPORTED)
set_target_properties(kademlia PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libkademlia.so
        )


add_library(PocoFoundation SHARED IMPORTED)
set_target_properties(PocoFoundation PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libPocoFoundation.so
        )
add_library(PocoNet SHARED IMPORTED)
set_target_properties(PocoNet PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libPocoNet.so
        )
add_library(PocoJSON SHARED IMPORTED)
set_target_properties(PocoJSON PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libPocoJSON.so
        )
add_library(PocoXML SHARED IMPORTED)
set_target_properties(PocoXML PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libPocoXML.so
        )
add_library(PocoUtil SHARED IMPORTED)
set_target_properties(PocoUtil PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libPocoUtil.so
        )
add_library(PocoEncodings SHARED IMPORTED)
set_target_properties(PocoEncodings PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libPocoEncodings.so
        )
add_library(PocoData SHARED IMPORTED)
set_target_properties(PocoData PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libPocoData.so
        )
add_library(PocoMongoDB SHARED IMPORTED)
set_target_properties(PocoMongoDB PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libPocoMongoDB.so
        )
add_library(PocoRedis SHARED IMPORTED)
set_target_properties(PocoRedis PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libPocoRedis.so
        )
add_library(PocoZip SHARED IMPORTED)
set_target_properties(PocoZip PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libPocoZip.so
        )

include_directories(${POCO_DIR}/Foundation/include)
include_directories(${POCO_DIR}/Net/include)
include_directories(${POCO_DIR}/JSON/include)
include_directories(${POCO_DIR}/XML/include)
include_directories(${POCO_DIR}/Util/include)
include_directories(${POCO_DIR}/Encodings/include)
include_directories(${POCO_DIR}/Data/include)
include_directories(${POCO_DIR}/MongoDB/include)
include_directories(${POCO_DIR}/Redis/include)
include_directories(${POCO_DIR}/Zip/include)

#find_package(${POCO_DIR}/build)

#add_subdirectory(${POCO_DIR})
#
#find_package(PocoFoundation)
#find_package(PocoNet)
#find_package(PocoJSON)
#find_package(PocoXML)
#find_package(PocoUtil)
#find_package(PocoEncodings)
#find_package(PocoData)
#find_package(PocoMongoDB)
#find_package(PocoRedis)
#find_package(PocoZip)


# -- QToken --------------------------------------------
add_library(qtoken SHARED IMPORTED)
set_target_properties(qtoken PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libqtoken.so
        )
# -------------------------------------------- QToken --

# -- Main Bridge ---------------------------------------
add_library(main-bridge SHARED main-bridge.cpp)
# --------------------------------------- Main Bridge --

target_include_directories(main-bridge PUBLIC ${INCLUDE_DIR}/Poco)

# -- Target ---------------------------------------------

#target_link_libraries(main-bridge PocoNet PocoFoundation)
#target_link_libraries(main-bridge PocoNetSSL PocoCrypto PocoUtil PocoNet)
#target_link_libraries(main-bridge PocoCrypto PocoFoundation Ssl Crypto)
#target_link_libraries(main-bridge PocoUtil PocoFoundation)
#target_link_libraries(main-bridge PocoJSON PocoFoundation)
#target_link_libraries(main-bridge PocoXML PocoFoundation)

target_link_libraries(
        main-bridge

        config++
        crypto
        ssl
        c++_shared
        gflags_nothreads
        aff3ct
        kademlia

        PocoFoundation
        PocoNet
        PocoJSON
        PocoXML
        PocoUtil
        PocoEncodings
        PocoData
        PocoMongoDB
        PocoRedis
        PocoZip

        qtoken
        ${log-lib}
)
# --------------------------------------------- Target --

