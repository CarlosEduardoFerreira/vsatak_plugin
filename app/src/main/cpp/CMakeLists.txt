cmake_minimum_required(VERSION 3.18.1)

project(Main-Bridge)

# -- Main Bridge ---------------------------------------
add_library(main-bridge SHARED main-bridge.cpp)
# --------------------------------------- Main Bridge --

# -- OpenSSL -------------------------------------------
#set(OPENSSL_ROOT_DIR /usr/bin/openssl)
#set(OPENSSL_INCLUDE_DIR /usr/include/openssl)
#set(OPENSSL_CRYPTO_LIBRARY /usr/lib/x86_64-linux-gnu)
# ------------------------------------------- OpenSSL --

# -- gFlags --------------------------------------------
add_subdirectory(gflags)
# -------------------------------------------- gFlags --

# -- Aff3ct --------------------------------------------
add_subdirectory(aff3ct)
# -------------------------------------------- Aff3ct --

# -- Boost ----------------------------------------------
# -- boost regex
add_library(boost_regex SHARED IMPORTED)
set_target_properties(boost_regex PROPERTIES IMPORTED_LOCATION
        /home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak/app/src/main/cpp/boost-for-android/build/install/libs/arm64-v8a/libboost_regex.so
        )

# -- boost system
add_library(boost_system SHARED IMPORTED)
set_target_properties(boost_system PROPERTIES IMPORTED_LOCATION
        /home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak/app/src/main/cpp/boost-for-android/build/install/libs/arm64-v8a/libboost_system.so
        )

# -- boost filesystem
add_library(boost_filesystem SHARED IMPORTED)
set_target_properties(boost_filesystem PROPERTIES IMPORTED_LOCATION
        /home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak/app/src/main/cpp/boost-for-android/build/install/libs/arm64-v8a/libboost_filesystem.so
        )

# -- boost fiber
add_library(boost_fiber SHARED IMPORTED)
set_target_properties(boost_fiber PROPERTIES IMPORTED_LOCATION
        /home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak/app/src/main/cpp/boost-for-android/build/install/libs/arm64-v8a/libboost_fiber.so
        )

# -- boost context
add_library(boost_context SHARED IMPORTED)
set_target_properties(boost_context PROPERTIES IMPORTED_LOCATION
        /home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak/app/src/main/cpp/boost-for-android/build/install/libs/arm64-v8a/libboost_context.so
        )
# ---------------------------------------------- Boost --

# -- Kademlia -------------------------------------------
add_library(kademlia SHARED IMPORTED)
set_target_properties(kademlia PROPERTIES IMPORTED_LOCATION
        /home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak/app/src/main/cpp/libs/libkademlia.so
        )
# ------------------------------------------- Kademlia --

# -- Include --------------------------------------------
include_directories( ${BOOST_INC_DIR} )
# -------------------------------------------- Include --

# -- Log ------------------------------------------------
find_library (log-lib log)
# ------------------------------------------------ Log --

# -- Target ---------------------------------------------
target_link_libraries( main-bridge ${log-lib} boost_regex boost_system boost_filesystem boost_fiber boost_context kademlia)
# --------------------------------------------- Target --

