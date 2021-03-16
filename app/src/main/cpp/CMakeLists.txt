cmake_minimum_required(VERSION 3.18.1)

project(Main-Bridge)

set(PROJECT_APP "/home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak2/app")
set(INCLUDE_DIR ${PROJECT_APP}/src/main/cpp/include)
set(POCO_DIR ${PROJECT_APP}/src/main/cpp/poco)

# -- Log ------------------------------------------------
find_library (log-lib log)
# ------------------------------------------------ Log --

# -- QToken --------------------------------------------
add_library(qtoken SHARED IMPORTED)
set_target_properties(qtoken PROPERTIES IMPORTED_LOCATION
        ${PROJECT_APP}/libs/${ANDROID_ABI}/libqtoken.so
        )
# -------------------------------------------- QToken --

# -- Include --------------------------------------------
include_directories(${INCLUDE_DIR})
include_directories(${INCLUDE_DIR}/aff3ct-2.3.5/aff3ct)
include_directories(${INCLUDE_DIR}/aff3ct-2.3.5/MIPP)
include_directories(${INCLUDE_DIR}/aff3ct-2.3.5/rang)
include_directories(${INCLUDE_DIR}/libconfig)
include_directories(${INCLUDE_DIR}/qtoken)
# -------------------------------------------- Include --

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

add_subdirectory(${POCO_DIR})

find_package(Poco)
find_package(PocoFoundation)
find_package(PocoNet)
find_package(PocoJSON)
find_package(PocoXML)
find_package(PocoUtil)
find_package(PocoEncodings)
find_package(PocoData)
find_package(PocoMongoDB)
find_package(PocoRedis)
find_package(PocoZip)

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

