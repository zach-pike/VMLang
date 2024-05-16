#include "memory.hpp"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

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
    for (const auto& dev : mmioDevices) {
        auto range = dev->getMMIORange();

        if (address >= range.startAddress && address < range.endAddress) {
            // Address handled by MMIO device
            return dev->onRead(address);
        }
    }

    return memory[address];
}

void SystemMemory::setU8(std::uint64_t address, std::uint8_t value) {
    // Check if address space is handled by a MMIO device
    for (const auto& dev : mmioDevices) {
        auto range = dev->getMMIORange();

        if (address >= range.startAddress && address < range.endAddress) {
            // Address handled by MMIO device
            return dev->onWrite(address, value);
        }
    }

    memory[address] = value;
}

void SystemMemory::loadFromFile(std::string fp) {
    std::ifstream file(fp, std::ios::in | std::ios::binary);

    if (!file.is_open()) throw std::runtime_error("File doesn't exist!");

    file.read((char*)memory, VM_MEMORY_SIZE);

    file.close();
}

void memDump(const std::uint8_t* data, std::size_t start, std::size_t end) {
    printf("\t0x0\t0x1\t0x2\t0x3\t0x4\t0x5\t0x6\t0x7\t0x8\t0x9\t0xA\t0xB\t0xC\t0xD\t0xE\t0xF\n");

    for(std::size_t i=start; i<end; i += 16) {
        std::cout << std::hex 
            << "0x" << i
            << "0x" << data[i] << '\t'
            << "0x" << data[i+1] << '\t'
            << "0x" << data[i+2] << '\t'
            << "0x" << data[i+3] << '\t'
            << "0x" << data[i+4] << '\t'
            << "0x" << data[i+5] << '\t'
            << "0x" << data[i+6] << '\t'
            << "0x" << data[i+7] << '\t'
            << "0x" << data[i+8] << '\t'
            << "0x" << data[i+9] << '\t'
            << "0x" << data[i+10] << '\t'
            << "0x" << data[i+11] << '\t'
            << "0x" << data[i+12] << '\t'
            << "0x" << data[i+13] << '\t'
            << "0x" << data[i+14] << '\t'
            << "0x" << data[i+15] << '\n';
        // printf(
        //     "0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\t0x%X\n",
        //     i,
        //     data[i],
        //     data[i + 1],
        //     data[i + 2],
        //     data[i + 3],
        //     data[i + 4],
        //     data[i + 5],
        //     data[i + 6],
        //     data[i + 7],
        //     data[i + 8],
        //     data[i + 9],
        //     data[i + 10],
        //     data[i + 11],
        //     data[i + 12],
        //     data[i + 13],
        //     data[i + 14],
        //     data[i + 15]
        // );
    }
}