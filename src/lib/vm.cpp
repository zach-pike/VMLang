#include "vm.hpp"

const char* instrNames[] = {
    "NoOp",
    "Halt",
    "Push",
    "Pop",
    "Add",
    "AddStack",
    "Sub",
    "SubStack",
    "Dump",
};

char regNames[] = {
    'A',
    'B',
    'C',
    'D'
};

#include <iostream>
#include <sstream>

InstructionArg::InstructionArg() {
    type = InstructionArgType::IMM;
    size = 0;
    isSigned = false;
    value = 0;
}

std::string InstructionArg::toString() const {
    std::string s;

    s.append("InstructionArg {");

    switch(type) {
        case InstructionArgType::IMM: {
            if (size == 0) {
                s.push_back('}');
                return s;
            }

            s.append(" Immediate(");
            if (isSigned) s.push_back('S'); else s.push_back('U');
            s.append(std::to_string(size*8));
            s.append("): ");
            s.append(std::to_string(value));
        } break;
        case InstructionArgType::REG: {
            s.append(" Reg: ");
            s.push_back(regNames[value]);
        } break;
        case InstructionArgType::IMM_MEM_ADDR: {
            s.append("Immediate Address: ");
            s.append(std::to_string(value));
        } break;
    }

    s.append(" }");

    return s;
}

VM::VM() {}
VM::~VM() {}

SystemMemory& VM::getMemory() {
    return memory;
}

void VM::initializeVM(std::uint64_t execStartAddr) {
    // Initialize registers
    regs.a = 0;
    regs.b = 0;
    regs.c = 0;
    regs.d = 0;

    regs.ip = execStartAddr;

    // Initialize system RAM
    memory.allocateMemory();
}

void VM::stepExecution(bool debug) {
    // Fetch instruction at IP
    std::uint8_t instr = memory.getRaw()[regs.ip];

    // Fetch arguments
    InstructionArg arg1 = *(InstructionArg*)(memory.getRaw() +regs.ip + 1);
    InstructionArg arg2 = *(InstructionArg*)(memory.getRaw() + regs.ip + 1 + sizeof(InstructionArg));

    bool ipIncreases = true;

    std::cout
        << std::hex << "0x" << regs.ip
        << ": OPCODE " 
        << instrNames[instr] 
        << " ARGS "
        << arg1.toString()
        << ' '
        << arg2.toString()
        << '\n';

    switch((Instructions)instr) {
        case Instructions::NOP: {
            // Do nothing
        } break;

        case Instructions::Halt: {
            ipIncreases = false;
        } break;

        case Instructions::Push: {
            // 
        } break;
    }

    if (ipIncreases) regs.ip += 1 + sizeof(InstructionArg) * 2;
}

void VM::dumpEverything() const {
    // Dump regs
    std::cout
    << std::hex 
    << "------ REGS ------\n"
    << "A: " << regs.a << '\n'
    << "B: " << regs.b << '\n'
    << "C: " << regs.c << '\n'
    << "D: " << regs.d << '\n'
    << "IP: " << regs.ip << '\n';

    // Dump Stack
    std::cout << "------ Stack ------\n";
    stack.dump();
}