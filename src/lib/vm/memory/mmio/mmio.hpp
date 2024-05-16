#pragma once

#include <cstdint>

struct MMIORange {
    std::uint64_t startAddress;
    std::uint64_t endAddress;
};

class MMIODevice {
public:
    virtual MMIORange getMMIORange() = 0;

    virtual void onWrite(std::uint8_t value, std::uint64_t address) = 0;
    virtual std::uint8_t onRead(std::uint64_t address) = 0;
};