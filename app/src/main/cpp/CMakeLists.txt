cmake_minimum_required(VERSION 3.18.1)

project(Main-Bridge)

set(PROJECT_CPP "/home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak/app/src/main/cpp")



#add_library(qtoken SHARED IMPORTED)
#set_target_properties(qtoken PROPERTIES IMPORTED_LOCATION
#        ${PROJECT_CPP}/libs/libQtoken.so
#        )

# -- Main Bridge ---------------------------------------
add_library(main-bridge SHARED main-bridge.cpp)
# --------------------------------------- Main Bridge --

# -- Log ------------------------------------------------
find_library (log-lib log)
# ------------------------------------------------ Log --


# -- Target ---------------------------------------------
target_link_libraries(
    main-bridge
    ${log-lib}

)
# --------------------------------------------- Target --

