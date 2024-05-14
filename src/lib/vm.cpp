#include "vm.hpp"

#include <iostream>
#include <sstream>

#include "lib/errors/errors.hpp"

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

std::uint64_t& VMRegs::getReg(VMReg reg) {
    switch(reg) {
        case VMReg::A: return a;
        case VMReg::B: return b;
        case VMReg::C: return c;
        case VMReg::D: return d;
        case VMReg::IP: return ip;
        default: throw ArgumentException(0);
    }
}

InstructionArg::InstructionArg(InstructionArgType _type, std::uint8_t _size, std::uint64_t _value, bool _isSigned) {
    type = _type;
    size = _size;
    isSigned = _isSigned;
    value = _value;
}

std::string InstructionArg::toString() const {
    std::string s;

    s.append("Arg {");

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
            s.append(" Immediate Address: ");
            s.append(std::to_string(value));
        } break;
    }

    s.append(" }");

    return s;
}

VM::VM() {}
VM::~VM() {}

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
        } break;

        case Instructions::Push: {
            switch(arg1.type) {
                case InstructionArgType::IMM: {
                    // Push a immediate value
                    StackItem item;
                    item.value = arg1.value;
                    item.isSigned = arg1.isSigned;
                    item.size = arg1.size;

                    stack.pushStackItem(item);
                } break;

                case InstructionArgType::IMM_MEM_ADDR: {
                    // Pulls a byte from the memory address specified
                    StackItem item;
                    item.value = memory.getU8(arg1.value);
                    item.isSigned = false;
                    item.size = 1;

                    stack.pushStackItem(item);
                } break;

                case InstructionArgType::REG: {
                    StackItem item;
                    switch(arg1.value) {
                        case 0: item.value = regs.a; break;
                        case 1: item.value = regs.b; break;
                        case 2: item.value = regs.c; break;
                        case 3: item.value = regs.d; break;
                    }
                    item.isSigned = false;
                    item.size = 1;

                    stack.pushStackItem(item);
                } break;
            }
        } break;

        case Instructions::Pop: {
            switch(arg1.type) {
                case InstructionArgType::IMM_MEM_ADDR: {
                    // try to pop u8 off stack and store it at memory address specified
                    StackItem item = stack.popStackItem();
                    if (item.size != 1) throw ArgumentException(regs.ip);
                    memory.setU8(arg1.value, item.value);
                } break;

                case InstructionArgType::REG: {
                    StackItem item = stack.popStackItem();
                    if (item.size != 8) throw ArgumentException(regs.ip);

                    regs.getReg((VMReg)arg1.value) = item.value;
                } break;
            }
        } break;

        case Instructions::Add: {
            std::uint64_t numA;
            std::uint64_t numB;

            switch (arg1.type) {
                case InstructionArgType::IMM: numA = arg1.value; break;
                case InstructionArgType::REG: numA = regs.getReg((VMReg)arg1.value); break;
            }
            switch (arg2.type) {
                case InstructionArgType::IMM: numB = arg2.value;break;
                case InstructionArgType::REG: numB = regs.getReg((VMReg)arg2.value); break;
            }

            std::uint64_t sum = numA + numB;

            stack.pushStackItem(StackItem{ .value = sum, .size = 8, .isSigned = false });
        } break;

        case Instructions::AddStack: {
            StackItem itm1 = stack.popStackItem();
            StackItem itm2 = stack.popStackItem();

            std::uint64_t sum = itm1.value + itm2.value;
            stack.pushStackItem(StackItem{ .value = sum, .size = 8, .isSigned = false });
        } break;

        case Instructions::Sub: {
            std::uint64_t numA;
            std::uint64_t numB;

            switch (arg1.type) {
                case InstructionArgType::IMM: numA = arg1.value; break;
                case InstructionArgType::REG: numA = regs.getReg((VMReg)arg1.value); break;
            }
            switch (arg2.type) {
                case InstructionArgType::IMM: numB = arg2.value;break;
                case InstructionArgType::REG: numB = regs.getReg((VMReg)arg2.value); break;
            }

            std::uint64_t diff = numA - numB;

            stack.pushStackItem(StackItem{ .value = diff, .size = 8, .isSigned = false });
        } break;

        case Instructions::SubStack: {
            StackItem itm1 = stack.popStackItem();
            StackItem itm2 = stack.popStackItem();

            std::uint64_t diff = itm1.value - itm2.value;
            stack.pushStackItem(StackItem{ .value = diff, .size = 8, .isSigned = false });
        } break;

        case Instructions::Dump: {
            dumpEverything();
        } break;
    }

    if (ipIncreases) regs.ip += 1 + sizeof(InstructionArg) * 2;
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