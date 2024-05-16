#pragma once

#include <cstdint>
#include <map>

#include "lib/vm.hpp"
#include "lib/variable/variable.hpp"
#include "lexer/lexer.hpp"

class Compiler {
private:
    enum class PrecompiledInstructionArgumentType {
        NUMBER,
        REGISTER,
        LABEL_ADDRESS
    };

    struct PrecompiledInstructionArgument {
        std::string strValue;
        VMValue var;
        PrecompiledInstructionArgumentType type;

        PrecompiledInstructionArgument(
            std::string _sv = "",
            VMValue _var = VMValue(),
            PrecompiledInstructionArgumentType tp = PrecompiledInstructionArgumentType::NUMBER);
    
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