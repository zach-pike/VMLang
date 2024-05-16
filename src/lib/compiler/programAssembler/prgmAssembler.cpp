#include "prgmAssembler.hpp"

#include <string.h>

#include <iostream>
#include <fstream>

ProgramAssembler::ProgramAssembler() {}

ProgramAssembler::~ProgramAssembler() {}

// void ProgramAssembler::setInsertOffset(std::uint64_t offset) {
//     insertPointer = data + offset;
// }

void ProgramAssembler::insertInstruction(Instructions opcode) { insertInstruction(opcode, InstructionArg(), InstructionArg()); }
void ProgramAssembler::insertInstruction(Instructions opcode, InstructionArg arg1) { insertInstruction(opcode, arg1, InstructionArg()); }
void ProgramAssembler::insertInstruction(Instructions opcode, InstructionArg arg1, InstructionArg arg2) {
    // Write byte of opcode
    std::uint8_t data[1 + sizeof(InstructionArg) * 2];

    data[0] = (std::uint8_t)opcode;

    // Write the args
    memcpy(data + 1, &arg1, sizeof(InstructionArg));
    memcpy(data + 1 + sizeof(InstructionArg), &arg2, sizeof(InstructionArg));

    programData.insert(programData.end(), data, data + sizeof(data));
}

void ProgramAssembler::dumpMemory(std::string fp) const {
    std::ofstream file(fp, std::ios::out | std::ios::binary);

    file.write((char*)programData.data(), programData.size());

    file.close();
}