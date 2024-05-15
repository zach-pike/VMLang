#include "prgmAssembler.hpp"

#include <string.h>

#include <iostream>
#include <fstream>

ProgramAssembler::ProgramAssembler() {
    data = new std::uint8_t[VM_MEMORY_SIZE];
    insertPointer = data;
}

ProgramAssembler::~ProgramAssembler() {
    delete[] data;
}

std::uint8_t* ProgramAssembler::getRaw() {
    return data;
}

void ProgramAssembler::setInsertOffset(std::uint64_t offset) {
    insertPointer = data + offset;
}

void ProgramAssembler::insertInstruction(Instructions opcode) { insertInstruction(opcode, InstructionArg(), InstructionArg()); }
void ProgramAssembler::insertInstruction(Instructions opcode, InstructionArg arg1) { insertInstruction(opcode, arg1, InstructionArg()); }
void ProgramAssembler::insertInstruction(Instructions opcode, InstructionArg arg1, InstructionArg arg2) {
    // Write byte of opcode
    *insertPointer = (std::uint8_t)opcode;

    // Write the args
    memcpy(insertPointer + 1, &arg1, sizeof(InstructionArg));
    memcpy(insertPointer + 1 + sizeof(InstructionArg), &arg2, sizeof(InstructionArg));

    insertPointer += 1 + sizeof(InstructionArg)*2;
}

void ProgramAssembler::dumpMemory(std::string fp) const {
    std::ofstream file(fp, std::ios::out | std::ios::binary);

    file.write((char*)data, VM_MEMORY_SIZE);

    file.close();
}