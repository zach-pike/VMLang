#include "compiler.hpp"

#include <string.h>

BuiltinCompiler::BuiltinCompiler() {
    data = new std::uint8_t[VM_MEMORY_SIZE];
    insertPointer = data;
}

BuiltinCompiler::~BuiltinCompiler() {
    delete[] data;
}

std::uint8_t* BuiltinCompiler::getRaw() {
    return data;
}

void BuiltinCompiler::setInsertOffset(std::uint64_t offset) {
    insertPointer = data + offset;
}

void BuiltinCompiler::insertInstruction(Instructions opcode) { insertInstruction(opcode, InstructionArg(), InstructionArg()); }
void BuiltinCompiler::insertInstruction(Instructions opcode, InstructionArg arg1) { insertInstruction(opcode, arg1, InstructionArg()); }
void BuiltinCompiler::insertInstruction(Instructions opcode, InstructionArg arg1, InstructionArg arg2) {
    // Write byte of opcode
    *insertPointer = (std::uint8_t)opcode;

    // Write the args
    memcpy(insertPointer + 1, &arg1, sizeof(InstructionArg));
    memcpy(insertPointer + 1 + sizeof(InstructionArg), &arg2, sizeof(InstructionArg));

    insertPointer += 1 + sizeof(InstructionArg)*2;
}
