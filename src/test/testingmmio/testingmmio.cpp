#include "testingmmio.hpp"

#include <assert.h>

TestingMMIO::TestingMMIO(std::uint64_t address):
    range(MMIORange{address, address + 1}) {}
TestingMMIO::~TestingMMIO() {}

MMIORange TestingMMIO::getMMIORange() { return range; }

void TestingMMIO::onWrite(std::uint8_t value, std::uint64_t address) {
    assert((value == 255));
}

std::uint8_t TestingMMIO::onRead(std::uint64_t address) {
    return 0;
}