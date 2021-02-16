cmake_minimum_required(VERSION 3.18.1)

project(Main-Bridge)

#-----------------------------------------
add_library(main-bridge SHARED main-bridge.cpp)


#-----------------------------------------
add_library (boost_regex SHARED IMPORTED)

set_target_properties(boost_regex PROPERTIES IMPORTED_LOCATION
 /home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak/app/src/main/cpp/boost-for-android/build/install/libs/libboost_regex.so
                    )

#set_target_properties(boost_regex PROPERTIES IMPORTED_LOCATION
#                    ${BOOST_LIBS_DIR}/${ANDROID_ABI}/libboost_regex.so
# /home/carlos/virgil/4.1.1.17/atak-civ/plugins/vintak/app/src/main/cpp/ndk_21_boost_1.72.0/libs/libboost_regex.so
#                    )
#-----------------------------------------
#add_library( boost_chrono SHARED IMPORTED)

#set_target_properties(boost_chrono PROPERTIES IMPORTED_LOCATION
#                    ${MY_BOOST_LIBS_DIR}/${ANDROID_ABI}/libboost_chrono.so
#                    )
#-----------------------------------------

include_directories( ${BOOST_INC_DIR} )

#-----------------------------------------
find_library (log-lib log)

#-----------------------------------------
target_link_libraries(main-bridge ${log-lib} )




