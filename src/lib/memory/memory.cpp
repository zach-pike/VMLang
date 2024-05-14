#include "memory.hpp"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

SystemMemory::SystemMemory() {}
SystemMemory::~SystemMemory() {
    if (initialized) deallocateMemory();
}

void SystemMemory::addMMIODevice(std::shared_ptr<MMIODevice> mmioDev) {
    mmioDevices.push_back(mmioDev);
}

void SystemMemory::allocateMemory() {
    if (!initialized) {
        memory = new std::uint8_t[VM_MEMORY_SIZE]; // 1MB of ram
        initialized = true;
    }

    memset(memory, 0, VM_MEMORY_SIZE); // Fill zeros

}
void SystemMemory::deallocateMemory() {
    if (!initialized) return;
    delete[] memory;
    initialized = false;
}

std::uint8_t* SystemMemory::getRaw() {
    return memory;
}

std::uint8_t SystemMemory::getU8(std::uint64_t address) {
    // Check if address space is handled by a MMIO device

    return memory[address];
}

std::uint64_t SystemMemory::getU64(std::uint64_t address) {
    // Check if address space is handled by a MMIO device
    return *(std::uint64_t*)memory[address];
}


void memDump(const std::uint8_t* data, std::size_t start, std::size_t end) {
    printf("\t0x0\t0x1\t0x2\t0x3\t0x4\t0x5\t0x6\t0x7\t0x8\t0x9\t0xA\t0xB\t0xC\t0xD\t0xE\t0xF\n");

    for(std::size_t i=start; i<end; i += 16) {
        printf(
            "0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X    \n",
            i,
            data[i],
            data[i + 1],
            data[i + 2],
            data[i + 3],
            data[i + 4],
            data[i + 5],
            data[i + 6],
            data[i + 7],
            data[i + 8],
            data[i + 9],
            data[i + 10],
            data[i + 11],
            data[i + 12],
            data[i + 13],
            data[i + 14],
            data[i + 15]
        );
    }
}