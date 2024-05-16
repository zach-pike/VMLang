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
    VMValue a;
    VMValue b;
    VMValue c;
    VMValue d;

    std::uint64_t ip;  // Instruction Pointer

    VMValue getReg(VMReg) const;
    void               setReg(VMReg, VMValue);
};

// This allow the InstructionArg class to represent all datatypes along with
enum class InstructionArgType : std::uint8_t {
    NUMBER   = 0,
    REGISTER = 1
};

struct InstructionArg {
    VMValue var;
    InstructionArgType type;

    InstructionArg(VMValue var = VMValue(), InstructionArgType type = InstructionArgType::NUMBER);
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

    // Debug
    Dump     = 8,

    Move     = 9,
    Jump     = 10,
};

class VM {
private:
    // System Registers
    VMRegs regs;

    // System RAM
    SystemMemory memory;
    Stack stack;

    VMValue getVariableFromInstructionArg(InstructionArg arg);
public:
    VM();
    ~VM();

    void initializeVM(std::uint64_t execStartAddr = 0x100);
    bool stepExecution(bool debug);

    SystemMemory& getMemory();
    Stack&        getStack();

    void dumpEverything() const;
};