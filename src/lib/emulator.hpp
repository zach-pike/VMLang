#pragma once

#include "memory/memory.hpp"

#include <cstdint>


struct EmulatorRegs {
    // General registers
    std::uint64_t a;
    std::uint64_t b;
    std::uint64_t c;
    std::uint64_t d;

    std::uint64_t ip;  // Instruction Pointer
    std::uint64_t sp;  // Stack Pointer
    std::uint64_t sbp; // Stack Base Pointer
};

enum class Instructions : std::uint8_t {
    NOP = 0,

    // Stack operations
    PushU8  = 1,
    PushS8  = 2,

    PushU16 = 3,
    PushS16 = 4,
    PushU32 = 5,
    PushS32 = 6,
    PushU64 = 7,
    PushS64 = 8,

    PopU8   = 9,
    PopS8   = 10,
    PopU16  = 11,
    PopS16  = 12,
    PopU32  = 13,
    PopS32  = 14,
    PopU64  = 15,
    PopS64  = 16,

    // Math operations
    AddU8    = 17,
    AddS8    = 18,
    AddU16   = 19,
    AddS16   = 20,
    AddU32   = 21,
    AddS32   = 22,
    AddU64   = 23,
    AddS64   = 24,

    SubU8    = 25,
    SubS8    = 26,
    SubU16   = 27,
    SubS16   = 28,
    SubU32   = 29,
    SubS32   = 30,
    SubU64   = 31,
    SubS64   = 32,
};

class Emulator {
private:
    // System Registers
    EmulatorRegs regs;

    // System RAM
    SystemMemory memory;
public:
    Emulator();
    ~Emulator();

    void initializeVM(std::uint64_t execStartAddr = 0x100, std::uint64_t stackBasePtr = 0x3000);
    void stepExecution();    
};