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