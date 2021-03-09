# Include this file in a cmake project with include(cppcheck.cmake)
# Then run cppcheck with make cppcheck

# get all project files
file(GLOB_RECURSE ALL_SOURCE_ALL_TEST_FILES test/*.cpp test/*.hpp src/*.cpp src/*.hpp)

add_custom_target(
        cppcheck
        COMMAND cppcheck
        --enable=all 
        --language=c++ 
        --suppress=missingIncludeSystem
        --error-exitcode=1
        ${ALL_SOURCE_ALL_TEST_FILES}
)