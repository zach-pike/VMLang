#pragma once

#include <cstdint>
#include <stdio.h>

#include "lib/vm.hpp"

class ProgramAssembler {
private:
    std::uint8_t* data;
    std::uint8_t* insertPointer;
public:
    ProgramAssembler();
    ~ProgramAssembler();

    std::uint8_t* getRaw();

    void setInsertOffset(std::uint64_t offset);

    void insertInstruction(Instructions opcode);
    void insertInstruction(Instructions opcode, InstructionArg arg1);
    void insertInstruction(Instructions opcode, InstructionArg arg1, InstructionArg arg2);

    void dumpMemory(std::string fp) const;
};