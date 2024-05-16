#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "vm/value/value.hpp"

enum class VMReg : std::uint8_t {
    A  = 0,
    B  = 1,
    C  = 2,
    D  = 3,
    IP = 4
};

inline std::vector<std::string> getRegNames() {
    return {
        "a",
        "b",
        "c",
        "d",
        "ip"
    };
}

struct VMRegs {
    // General registers
    VMValue a;
    VMValue b;
    VMValue c;
    VMValue d;

    std::uint64_t ip;  // Instruction Pointer

    VMValue getReg(VMReg) const;
    void               setReg(VMReg, VMValue);
};