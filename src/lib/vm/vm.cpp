#include "vm.hpp"

#include <iostream>
#include <sstream>

#include <string.h>
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
            VMValue item = stack.pop();

            if (arg1.var.isUninitialized) break;

            assert((arg1.type == InstructionArgType::REGISTER));
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
            VMValue val = getVariableFromInstructionArg(arg1);
            
            assert((arg2.type == InstructionArgType::REGISTER));
            regs.setReg((VMReg)arg2.var.value.uInt, val);
        } break;

        case Instructions::Jump: {
            VMValue val = getVariableFromInstructionArg(arg1);
            assert((val.vartype == VMValueType::UINT && val.size == 8));
            regs.ip = val.value.uInt;

            ipIncreases = false;
        } break;

        case Instructions::StoreByte: {
            assert((arg1.type == InstructionArgType::NUMBER));
            assert((arg1.var.vartype == VMValueType::UINT));
            assert((arg1.var.size == 8));

            std::uint64_t addr = arg1.var.value.uInt;

            VMValue val = getVariableFromInstructionArg(arg2);
            assert((val.size == 1));
            assert((val.vartype == VMValueType::UINT));

            std::uint8_t byteToWrite = (std::uint8_t)val.value.uInt;

            memory.setU8(addr, byteToWrite);
        } break;

        case Instructions::LoadByte: {
            assert((arg1.type == InstructionArgType::NUMBER));
            assert((arg1.var.vartype == VMValueType::UINT));
            assert((arg1.var.size == 8));

            std::uint64_t addr = arg1.var.value.uInt;

            assert((arg2.type == InstructionArgType::REGISTER));
            std::uint8_t value = memory.getU8(addr);

            regs.setReg((VMReg)arg2.var.value.uInt, VMValue(value));
        } break;

        case Instructions::CompareEq: {
            VMValue val1 = getVariableFromInstructionArg(arg1);
            VMValue val2 = getVariableFromInstructionArg(arg2);

            if (val1.vartype == val2.vartype &&
                  memcmp(&val1.value, &val2.value, sizeof(VMValue::ValueUnion)) == 0) {
                stack.push(VMValue((std::uint8_t)1));
            } else {
                stack.push(VMValue((std::uint8_t)0));
            }
        } break;

        case Instructions::CompareEqStack: {
            VMValue val1 = stack.pop();
            VMValue val2 = stack.pop();

            if (val1.vartype == val2.vartype &&
                  memcmp(&val1.value, &val2.value, sizeof(VMValue::ValueUnion)) == 0) {
                stack.push(VMValue((std::uint8_t)1));
            } else {
                stack.push(VMValue((std::uint8_t)0));
            }
        } break;

        case Instructions::JumpIfTrue: {
            VMValue v = stack.pop();
            assert((v.vartype == VMValueType::SINT) || (v.vartype == VMValueType::UINT));

            VMValue addr = getVariableFromInstructionArg(arg1);
            assert((addr.vartype == VMValueType::SINT) || (addr.vartype == VMValueType::UINT));
            assert((addr.size == 8));

            if (v.value.uInt != 0) {
                regs.ip = addr.value.uInt;
                ipIncreases = false;
            }
        } break;

        case Instructions::JumpIfFalse: {
            VMValue v = stack.pop();
            assert((v.vartype == VMValueType::SINT) || (v.vartype == VMValueType::UINT));

            VMValue addr = getVariableFromInstructionArg(arg1);
            assert((addr.vartype == VMValueType::SINT) || (addr.vartype == VMValueType::UINT));
            assert((addr.size == 8));

            if (v.value.uInt == 0) {
                regs.ip = addr.value.uInt;
                ipIncreases = false;
            }

            ipIncreases = false;
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