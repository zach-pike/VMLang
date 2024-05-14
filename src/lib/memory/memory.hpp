#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "mmio/mmio.hpp"

#define VM_MEMORY_SIZE 1000000

class SystemMemory {
private:
    bool initialized = false;
    std::uint8_t* memory;

    std::vector<std::shared_ptr<MMIODevice>> mmioDevices;

public:
    SystemMemory();
    ~SystemMemory();

    void addMMIODevice(std::shared_ptr<MMIODevice> mmioDev);

    // Called when program is started or is used when resetting state of machine
    void allocateMemory();
    void deallocateMemory();

    std::uint8_t* getRaw();

    std::uint8_t getU8(std::uint64_t address);
    std::uint16_t getU16(std::uint64_t address);
    std::uint32_t getU32(std::uint64_t address);
    std::uint64_t getU64(std::uint64_t address);

    void setU8(std::uint64_t address, std::uint8_t value);
    void setU16(std::uint64_t address, std::uint16_t value);
    void setU32(std::uint64_t address, std::uint32_t value);
    void setU64(std::uint64_t address, std::uint64_t value);
};


void memDump(const std::uint8_t* data, std::size_t start, std::size_t end);