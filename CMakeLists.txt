cmake_minimum_required(VERSION 3.22.1)

project(VMLang)

include(CTest)

include_directories(src/)

file(GLOB_RECURSE LIBEMUSRC src/lib/*.cpp)
file(GLOB_RECURSE EMUAPPSRC src/frontend/*.cpp)

add_library(emu SHARED ${LIBEMUSRC})
add_executable(frontend ${EMUAPPSRC})

target_link_libraries(frontend emu)

add_executable(stacktest src/test/stacktest.cpp)
target_link_libraries(stacktest emu)
add_test(NAME StackTest COMMAND stacktest)

add_executable(fibtest src/test/fibtest.cpp)
target_link_libraries(fibtest emu)
add_test(NAME FibTest COMMAND fibtest)