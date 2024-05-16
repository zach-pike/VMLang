#include "iommio.hpp"

#include <stdlib.h>
#include <stdio.h>

IOmmio::IOmmio(std::uint64_t baseAddr):
    range(MMIORange{ baseAddr, baseAddr + 1 }) {}

IOmmio::~IOmmio() {}

MMIORange IOmmio::getMMIORange() { return range; }

void IOmmio::onWrite(std::uint8_t value, std::uint64_t address) {
    putchar(value);
}

std::uint8_t IOmmio::onRead(std::uint64_t address) {
    return 0;
}