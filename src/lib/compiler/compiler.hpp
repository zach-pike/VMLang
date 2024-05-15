#pragma once

#include <cstdint>
#include <map>

#include "lib/vm.hpp"
#include "lexer/lexer.hpp"

class Compiler {
private:
    enum class PrecompiledInstructionArgumentType {
        IMM,
        REG,
        LABEL_ADDRESS
    };

    struct PrecompiledInstructionArgument {
        std::string strValue;

        std::uint64_t value;
        std::uint8_t size;
        bool isSigned;
        bool isUnused;

        PrecompiledInstructionArgumentType type;

        PrecompiledInstructionArgument(
            std::string _sv = "",
            std::uint64_t _v = 0,
            std::uint8_t sz = 0,
            bool is = false, 
            bool iu = true,
            PrecompiledInstructionArgumentType tp = PrecompiledInstructionArgumentType::IMM);
    
        PrecompiledInstructionArgument(
            const std::vector<LexerToken>& tokens
        );
    };

    struct PrecompiledInstruction {
        Instructions instr;
        PrecompiledInstructionArgument arg1;
        PrecompiledInstructionArgument arg2;

        PrecompiledInstruction(Instructions, PrecompiledInstructionArgument, PrecompiledInstructionArgument);
    };

    std::vector<LexerToken> tokens;
public:
    Compiler();
    ~Compiler();

    void parseFile(std::string filePath);
    void compileAndWriteBinary(std::string filePath);
};