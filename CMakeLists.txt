cmake_minimum_required(VERSION 3.22.1)

project(VMLang)

include(CTest)

include_directories(argparse/include)
include_directories(src/lib/)

file(GLOB_RECURSE LIBVMSRC src/lib/*.cpp)
file(GLOB_RECURSE VMFRONTENDSRC src/frontend/*.cpp)

add_library(vm SHARED ${LIBVMSRC})
add_executable(vmcl ${VMFRONTENDSRC})

target_link_libraries(vmcl vm)

add_executable(stacktest src/test/stacktest.cpp)
target_link_libraries(stacktest vm)
add_test(NAME StackTest COMMAND stacktest)

add_executable(fibtest src/test/fibtest.cpp)
target_link_libraries(fibtest vm)
add_test(NAME FibTest COMMAND fibtest)