#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "vm/value/value.hpp"
#include "vm/instruction/instruction.hpp"

// This allow the InstructionArg class to represent all datatypes along with
enum class InstructionArgType : std::uint8_t {
    NUMBER   = 0,
    REGISTER = 1
};

struct InstructionArg {
    VMValue var;
    InstructionArgType type;

    InstructionArg(VMValue var = VMValue(), InstructionArgType type = InstructionArgType::NUMBER);
    std::string toString() const;
} __attribute__((packed));

enum class Instructions : std::uint8_t {
    NOP  = 0,
    Halt = 1,   // Just sits there, doesn't increment ip

    // Stack operations
    Push = 2,
    Pop  = 3,

    // Math operations
    Add      = 4,
    AddStack = 5,

    Sub      = 6,
    SubStack = 7,

    // Debug
    Dump     = 8,

    Move     = 9,
    Jump     = 10,

    StoreByte = 11,
    LoadByte  = 12,

    CompareEq = 13,
    CompareEqStack = 14,
    JumpIfTrue  = 15,
    JumpIfFalse = 16
};

inline std::vector<std::string> getInstructionNames() {
    return {
        "nop",
        "halt",
        "push",
        "pop",
        "add",
        "addStack",
        "sub",
        "subStack",
        "dump",
        "move",
        "jump",

        "storeByte",   // storeByte u64 var
        "loadByte",

        "compareEq",
        "compareEqStack",
        "jumpIfEq",
        "jumpIfNEq"
    };
}