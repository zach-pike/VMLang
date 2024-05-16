#include "vm.hpp"

#include <iostream>
#include <sstream>

#include <assert.h>
#include "vm/errors/errors.hpp"

VMValue VM::getVariableFromInstructionArg(InstructionArg arg) {
    switch(arg.type) {
        case InstructionArgType::NUMBER: return arg.var;
        case InstructionArgType::REGISTER: return regs.getReg((VMReg)arg.var.value.uInt);
        default: throw std::runtime_error("Could not get value!");
    }
}

VM::VM() {}
VM::~VM() {}

void VM::initializeVM(std::uint64_t execStartAddr) {
    // Initialize registers
    regs.ip = execStartAddr;

    // Initialize system RAM
    memory.allocateMemory();
}

bool VM::stepExecution(bool debug) {
    // Fetch instruction at IP
    std::uint8_t instr = memory.getRaw()[regs.ip];

    // Fetch arguments
    InstructionArg arg1 = *(InstructionArg*)(memory.getRaw() +regs.ip + 1);
    InstructionArg arg2 = *(InstructionArg*)(memory.getRaw() + regs.ip + 1 + sizeof(InstructionArg));

    bool ipIncreases = true;
    bool machineHalted = false;

    auto instrNames = getInstructionNames();

    if (debug)
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
            machineHalted = true;
        } break;

        case Instructions::Push: {
            VMValue item = getVariableFromInstructionArg(arg1);
            stack.push(item);
        } break;

        case Instructions::Pop: {
            assert((arg1.type == InstructionArgType::REGISTER));
            VMValue item = stack.pop();

            regs.setReg((VMReg)arg1.var.value.uInt, item);
        } break;

        case Instructions::Add: {
            VMValue numA = getVariableFromInstructionArg(arg1);
            VMValue numB = getVariableFromInstructionArg(arg2);
            VMValue sum = numA + numB;
            stack.push(sum);
        } break;

        case Instructions::AddStack: {
            VMValue itm1 = stack.pop();
            VMValue itm2 = stack.pop();

            VMValue sum = itm1 + itm2;
            stack.push(sum);
        } break;

        case Instructions::Sub: {
            VMValue numA = getVariableFromInstructionArg(arg1);
            VMValue numB = getVariableFromInstructionArg(arg2);

            VMValue diff = numA - numB;

            stack.push(diff);
        } break;

        case Instructions::SubStack: {
            VMValue itm1 = stack.pop();
            VMValue itm2 = stack.pop();

            VMValue diff = itm1 - itm2;
            stack.push(diff);
        } break;

        case Instructions::Dump: {
            dumpEverything();
        } break;

        case Instructions::Move: {
            // std::uint64_t valToMove = 
        } break;
    }

    if (ipIncreases) regs.ip += 1 + sizeof(InstructionArg) * 2;

    return machineHalted;
}

SystemMemory& VM::getMemory() {
    return memory;
}

Stack& VM::getStack() {
    return stack;
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