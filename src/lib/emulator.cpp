#include "emulator.hpp"

Emulator::Emulator() {}
Emulator::~Emulator() {}

void Emulator::initializeVM(std::uint64_t execStartAddr, std::uint64_t stackBasePtr) {
    // Initialize registers
    regs.a = 0;
    regs.b = 0;
    regs.c = 0;
    regs.d = 0;

    // Start executing here
    regs.ip = execStartAddr;

    // Set stack stuff
    regs.sp = regs.sbp = stackBasePtr;

    // Initialize system RAM
    memory.allocateMemory();
}

void Emulator::stepExecution() {
    // Fetch instruction at IP
    Instructions instr = (Instructions)memory.getU8(regs.ip);

    // Fetch arguments
    std::uint64_t arg1 = memory.getU8(regs.ip + 1);
    std::uint64_t arg2 = memory.getU8(regs.ip + 1 + 8);

    bool ipIncreases = true;

    switch(instr) {
        case Instructions::NOP: {
            // Do nothing
        } break;
    }

    if (ipIncreases) regs.ip += 1 + sizeof(std::uint64_t) * 2;
}