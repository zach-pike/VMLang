#include "errors.hpp"

#include <sstream>

ArgumentException::ArgumentException(std::uint64_t errorAddr) {
    std::stringstream ss;
    ss << "Error at instruction at 0x" << std::hex << errorAddr;
    errStr = ss.str();
}

ArgumentException::~ArgumentException() {}

const char* ArgumentException::what() {
    return errStr.c_str();
}