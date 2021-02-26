cmake_minimum_required(VERSION 3.18.1)

project(Main-Bridge)

set(PROJECT_APP "/home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak2/app")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(ANDROID_STL "c++_shared")

# -- Main Bridge ---------------------------------------
add_library(main-bridge SHARED main-bridge.cpp)
# --------------------------------------- Main Bridge --

# -- LibConfig ------------------------------------------
add_library(config++ SHARED IMPORTED)
set_target_properties(config++ PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libconfig++.so
        )
# ------------------------------------------ LibConfig --

# -- StdC++ ---------------------------------------------
add_library(stdc++ SHARED IMPORTED)
set_target_properties(stdc++ PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libstdc++.so
        )
# --------------------------------------------- StdC++ --

 -- Boost ----------------------------------------------
add_library(boost_context SHARED IMPORTED)
set_target_properties(boost_context PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libboost_context.so
        )

add_library(boost_fiber SHARED IMPORTED)
set_target_properties(boost_fiber PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libboost_fiber.so
        )

add_library(boost_filesystem SHARED IMPORTED)
set_target_properties(boost_filesystem PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libboost_filesystem.so
        )

add_library(boost_regex SHARED IMPORTED)
set_target_properties(boost_regex PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libboost_regex.so
        )

add_library(boost_system SHARED IMPORTED)
set_target_properties(boost_system PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libboost_system.so
        )
 ---------------------------------------------- Boost --

# -- Aff3ct ---------------------------------------------
add_library(aff3ct SHARED IMPORTED)
set_target_properties(aff3ct PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libaff3ct-2.3.5.so
        )
# --------------------------------------------- Aff3ct --

# -- Kademlia -------------------------------------------
add_library(kademlia SHARED IMPORTED)
set_target_properties(kademlia PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libkademlia.so
        )
# ------------------------------------------- Kademlia --

# -- Poco -----------------------------------------------
add_library(poco_foundation SHARED IMPORTED)
set_target_properties(poco_foundation PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libpoco_foundation.so
        )

add_library(poco_json SHARED IMPORTED)
set_target_properties(poco_json PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libpoco_json.so
        )

add_library(poco_net SHARED IMPORTED)
set_target_properties(poco_net PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libpoco_net.so
        )
#target_include_directories(poco_net INTERFACE ${PROJECT_APP}/include/Poco/Net)

add_library(poco_util SHARED IMPORTED)
set_target_properties(poco_util PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libpoco_util.so
        )

add_library(poco_xml SHARED IMPORTED)
set_target_properties(poco_xml PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libpoco_xml.so
        )
# ---------------------------------------------- Poco ---

# -- QToken ---------------------------------------------
add_library(qtoken SHARED IMPORTED)
set_target_properties(qtoken PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libqtoken.so
        )
# -------------------------------------------- QToken ---

# -- Log ------------------------------------------------
find_library (log-lib log)
# ------------------------------------------------ Log --

#include_directories(${PROJECT_APP}/libs/arm64-v8a)
#find_library(stdc++
#        HINTS ${PROJECT_APP}/libs/arm64-v8a)

# -- Target ---------------------------------------------
target_link_libraries(
    main-bridge INTERFACE
    config++
    stdc++
    boost_context
    boost_fiber
    boost_filesystem
    boost_regex
    boost_system
    aff3ct
    kademlia
    poco_foundation
    poco_json
    poco_net
    poco_util
    poco_xml
    qtoken
    ${log-lib}
)
# --------------------------------------------- Target --

