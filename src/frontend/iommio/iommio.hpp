#pragma once

#include "vm/memory/mmio/mmio.hpp"

class IOmmio : public MMIODevice {
private:
    MMIORange range;
public:
    IOmmio(std::uint64_t baseAddr);
    ~IOmmio();

    MMIORange getMMIORange();
    void onWrite(std::uint8_t value, std::uint64_t address);
    std::uint8_t onRead(std::uint64_t address);
};