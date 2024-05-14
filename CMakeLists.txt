cmake_minimum_required(VERSION 3.22.1)

project(MyArch)

include(CTest)

include_directories(src/)

file(GLOB_RECURSE LIBEMUSRC src/lib/*.cpp)
file(GLOB_RECURSE EMUAPPSRC src/frontend/*.cpp)

add_library(emu SHARED ${LIBEMUSRC})
add_executable(frontend ${EMUAPPSRC})

target_link_libraries(frontend emu)


add_executable(test1 src/test/test1.cpp)
add_test(NAME Test1 COMMAND test1)