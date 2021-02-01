cmake_minimum_required(VERSION 3.4.1)

add_library(hello-jni SHARED hello-jni.cpp)

target_link_libraries(hello-jni android log)

#target_include_directories(hello-jni PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#target_include_directories(android-aarch64 PUBLIC src/main/libs/arm64-v8a)
#target_include_directories(android-armv7 PUBLIC src/main/libs/armeabi-v7a)

