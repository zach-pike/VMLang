#pragma once

#include "memory/memory.hpp"
#include "stack/stack.hpp"

#include <cstdint>
#include <vector>


enum class VMReg : std::uint8_t {
    A  = 0,
    B  = 1,
    C  = 2,
    D  = 3,
    IP = 4
};

struct VMRegs {
    // General registers
    std::uint64_t a;
    std::uint64_t b;
    std::uint64_t c;
    std::uint64_t d;

    std::uint64_t ip;  // Instruction Pointer

    std::uint64_t& getReg(VMReg);
};

enum class InstructionArgType : std::uint8_t {
    IMM          = 0,
    REG          = 1,
    IMM_MEM_ADDR = 2
};

struct InstructionArg {
    InstructionArgType type;
    std::uint8_t       size;      // in bytes 
    bool               isSigned;
    std::uint64_t      value;

    InstructionArg(InstructionArgType type = InstructionArgType::IMM, std::uint8_t size = 0, std::uint64_t value = 0, bool isSigned = false);
    std::string toString() const;
} __attribute__((packed));

enum class Instructions : std::uint8_t {
    NOP  = 0,
    Halt = 1,   // Just sits there, doesn't increment ip

    // Stack operations
    Push = 2,
    Pop  = 3,

    // Math operations
    Add      = 4,
    AddStack = 5,

    Sub      = 6,
    SubStack = 7,

    Dump     = 8
};

class VM {
private:
    // System Registers
    VMRegs regs;

    // System RAM
    SystemMemory memory;
    Stack stack;
public:
    VM();
    ~VM();

    void initializeVM(std::uint64_t execStartAddr = 0x100);
    void stepExecution(bool debug);

    SystemMemory& getMemory();
    Stack&        getStack();

    void dumpEverything() const;
};