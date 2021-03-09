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
#add_library(config++ SHARED IMPORTED)
#set_target_properties(config++ PROPERTIES IMPORTED_LOCATION
#        ${PROJECT_APP}/libs/${ANDROID_ABI}/libconfig++.so
#        )
# ------------------------------------------ LibConfig --


# -- QToken ---------------------------------------------
add_library(qtoken SHARED IMPORTED)
set_target_properties(qtoken PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libqtoken.so
        )
# -------------------------------------------- QToken ---

# -- Log ------------------------------------------------
find_library (log-lib log)
# ------------------------------------------------ Log --

#add_subdirectory(qtoken)
#find_library(stdc++
#        HINTS ${PROJECT_APP}/libs/arm64-v8a)

# -- Target ---------------------------------------------
target_link_libraries(
    main-bridge INTERFACE
     #   qtoken/include/libconfig
#    stdc++
#    boost_context
#    boost_fiber
#    boost_filesystem
#    boost_regex
#    boost_system
#    aff3ct
#    kademlia
#        libPocoFoundation
#        libPocoJSON
#        libPocoNet
#        libPocoUtil
#        libPocoXML
    qtoken
    ${log-lib}
)
# --------------------------------------------- Target --

