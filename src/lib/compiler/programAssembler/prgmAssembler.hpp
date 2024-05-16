#pragma once

#include <cstdint>
#include <stdio.h>

#include "vm/instruction/instruction.hpp"

class ProgramAssembler {
private:
    std::vector<std::uint8_t> programData;
public:
    ProgramAssembler();
    ~ProgramAssembler();

    // void setInsertOffset(std::uint64_t offset);

    void insertInstruction(Instructions opcode);
    void insertInstruction(Instructions opcode, InstructionArg arg1);
    void insertInstruction(Instructions opcode, InstructionArg arg1, InstructionArg arg2);

    void dumpMemory(std::string fp) const;
};