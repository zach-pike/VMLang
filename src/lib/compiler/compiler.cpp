#include "compiler.hpp"

#include <iostream>
#include <fstream>
#include <assert.h>


Compiler::PrecompiledInstructionArgument::PrecompiledInstructionArgument(
    std::string _strval,
    std::uint64_t _value,
    bool _isSigned,
    bool _isUnused,
    PrecompiledInstructionArgumentType _type
):
    strValue(_strval),
    value(_value),
    isSigned(_isSigned),
    isUnused(_isUnused),
    type(_type) {}

Compiler::PrecompiledInstructionArgument::PrecompiledInstructionArgument(
    const std::vector<LexerToken>& tokens
) { 
    std::cout << "Arg toks begin\n";
    for (const LexerToken& tok : tokens) {
        std::cout << (int)tok.type << ' ' << tok.value << '\n';
    }
    std::cout << "Arg toks end\n";
}

Compiler::PrecompiledInstruction::PrecompiledInstruction(
    Instructions _instr,
    PrecompiledInstructionArgument _arg1,
    PrecompiledInstructionArgument _arg2
):
    instr(_instr),
    arg1(_arg1),
    arg2(_arg2) {}

Compiler::Compiler() {}
Compiler::~Compiler() {}

void Compiler::parseFile(std::string fp) {
    std::ifstream file(fp);
    std::cout << fp << '\n';
    if (!file.is_open()) throw std::runtime_error("Couldn't open file");

    auto nTokens = tokenizeString(file);

    tokens.insert(tokens.end(), nTokens.begin(), nTokens.end());
}

void Compiler::compileAndWriteBinary(std::string filePath) {
    std::map<std::string, std::uint64_t> labelAddresses;
    std::map<std::string, std::vector<PrecompiledInstruction>> symbols;

    std::vector<TokenType> labelDefOrder = {
        TokenType::LITERAL,
        TokenType::LEFT_PAREN,
        TokenType::LITERAL,
        TokenType::RIGHT_PAREN,
        TokenType::COLON,
        TokenType::EOL
    };

    std::map<std::string, Instructions> instructionLookupTable = {
        { "NOP", Instructions::NOP },
        { "halt", Instructions::Halt },
        { "push", Instructions::Push },
        { "pop", Instructions::Pop },
        { "add", Instructions::Add },
        { "addStack", Instructions::AddStack },
        { "sub", Instructions::Sub },
        { "subStack", Instructions::SubStack },
        { "dump", Instructions::Dump }
    };

    // Start the loop of trying to find a label definition and reading the instructions in
    std::string currentLabel = "";

    for (int i=0; i<tokens.size(); i++) {
        // Check for label def if we wont overrun the tokens array
        if(i<(tokens.size()-labelDefOrder.size())) {
            bool foundLabel = true;

            // Check to see if each tokens matches
            for(int j=0; j<labelDefOrder.size(); j++) {
                if (tokens.at(i + j).type != labelDefOrder.at(j)) {
                    foundLabel = false;
                    break;
                }
            }

            if (foundLabel) {
                assert((tokens.at(i).type == TokenType::LITERAL));

                currentLabel = tokens.at(i).value;
                std::cout << "Switching to label: " << currentLabel << "...\n";

                // Parse offset
                assert((tokens.at(i + 2).type == TokenType::LITERAL));
                std::uint64_t address = std::stoull(tokens.at(i + 2).value);
                std::cout << "with a offset of " << address << '\n';

                labelAddresses.insert({ currentLabel, address });

                i += labelDefOrder.size() - 1;
                continue;
            }
        }

        // Read first token as instruction name
        LexerToken nameToken = tokens.at(i);

        // Parse instruction
        assert((nameToken.type == TokenType::LITERAL));
        Instructions instr = instructionLookupTable.at(nameToken.value);

        // Parse args if possible

        // If next token is a EOL then this instruction is done
        if (tokens.at(i + 1).type == TokenType::EOL) {
            symbols[currentLabel].push_back(
                PrecompiledInstruction(
                    instr,
                    PrecompiledInstructionArgument(),
                    PrecompiledInstructionArgument()));

            i += 2;
            continue;
        }

        // Now we look ahead to find the comma and pass thoes tokens in between to the arg parser
        // we only look ahead 15 tokens if we get past that somethings fucked

        int arg1EndOffset;
        bool hasArg2 = false;

        for (int j=1; j<16; j++) {
            if (tokens.at(i + j).type == TokenType::COMMA) {
                arg1EndOffset = j;
                hasArg2 = true;
                break;
            }
            if (tokens.at(i + j).type == TokenType::EOL) {
                arg1EndOffset = j;
                break;
            }
        }

        std::vector<LexerToken> arg1Tokens(tokens.begin() + i + 1, tokens.begin() + i + arg1EndOffset);

        PrecompiledInstructionArgument arg1(arg1Tokens);
        
        if (!hasArg2) {
            // We are done
            symbols[currentLabel].push_back(
                PrecompiledInstruction(
                    instr,
                    arg1,
                    PrecompiledInstructionArgument()
                )
            );

            i += 1 + arg1EndOffset - 1;
            continue;
        }

        int arg2EndOffset;

        for (int j=arg1EndOffset+1; j<(arg1EndOffset+1+16); j++) {
            if (tokens.at(i + j).type == TokenType::EOL) {
                arg2EndOffset = j;
                break;
            }
        }

        std::vector<LexerToken> arg2Tokens(tokens.begin() + i + 1 + arg1EndOffset, tokens.begin() + i + arg2EndOffset);
        PrecompiledInstructionArgument arg2(arg2Tokens);

        // Okay now were really done
        symbols[currentLabel].push_back(
            PrecompiledInstruction(
                instr,
                arg1,
                arg2
            )
        );

        i += 1 + arg2EndOffset - 1;
        continue;

    }

    return;
}