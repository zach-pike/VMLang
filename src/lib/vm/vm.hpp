#pragma once

#include "memory/memory.hpp"
#include "stack/stack.hpp"
#include "registers/registers.hpp"
#include "instruction/instruction.hpp"

#include <cstdint>
#include <vector>

class VM {
private:
    // System Registers
    VMRegs regs;

    // System RAM
    SystemMemory memory;

    // Stack
    Stack stack;

    // Takes a InstructionArg and converts it to a value
    // If arg is a NUMBER then return value is arg.var
    // If arg is a REGISTER then return value is the value
    // of the register index supplied
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