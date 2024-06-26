#include "compiler.hpp"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <sstream>

#include "programAssembler/prgmAssembler.hpp"
#include "vm/registers/registers.hpp"

Compiler::PrecompiledInstructionArgument::PrecompiledInstructionArgument(
    std::string _strval,
    VMValue _var,
    PrecompiledInstructionArgumentType _type
):
    strValue(_strval),
    var(_var),
    type(_type) {}

Compiler::PrecompiledInstructionArgument::PrecompiledInstructionArgument(
    const std::vector<LexerToken>& tokens
) { 
    // Check what the first literal says
    assert((tokens.size() > 0));
    assert((tokens.at(0).type == TokenType::LITERAL));

    LexerToken firstToken = tokens.at(0);

    if (firstToken.value.rfind('%', 0) == 0) {
        // Get the char after the percent
        assert((firstToken.value.size() > 1));
        std::vector<std::string> regs = getRegNames();
        std::string s = firstToken.value.substr(1);

        std::uint64_t regNum = std::find(regs.begin(), regs.end(), s) - regs.begin();

        // initialize struct
        strValue = "";
        var = VMValue(regNum);
        type = PrecompiledInstructionArgumentType::REGISTER;
    } else if (firstToken.value.rfind('$', 0) == 0) {
        strValue = firstToken.value.substr(1);
        var = VMValue();
        type = PrecompiledInstructionArgumentType::LABEL_ADDRESS;
    } else if (firstToken.value.rfind('u', 0) == 0) {
        std::uint8_t sizeBytes = std::stoi(firstToken.value.substr(1)) / 8;

        strValue = "";
        type = PrecompiledInstructionArgumentType::NUMBER;

        // Initialize value
        var = VMValue();
        var.isUninitialized = false;
        var.vartype = VMValueType::UINT;
        var.size = sizeBytes;

        // Assert that there is parens around number
        assert((tokens.at(1).type == TokenType::LEFT_PAREN));
        assert((tokens.at(3).type == TokenType::RIGHT_PAREN));

        LexerToken valueToken = tokens.at(2);
        assert((valueToken.type == TokenType::LITERAL));

        var.value.uInt = std::stoull(valueToken.value);
    }  else if (firstToken.value.rfind('s', 0) == 0) {
        std::uint8_t sizeBytes = std::stoi(firstToken.value.substr(1)) / 8;

        strValue = "";
        type = PrecompiledInstructionArgumentType::NUMBER;

        // Initialize value
        var = VMValue();
        var.isUninitialized = false;
        var.vartype = VMValueType::SINT;
        var.size = sizeBytes;

        // Assert that there is parens around number
        assert((tokens.at(1).type == TokenType::LEFT_PAREN));
        assert((tokens.at(3).type == TokenType::RIGHT_PAREN));

        LexerToken valueToken = tokens.at(2);
        assert((valueToken.type == TokenType::LITERAL));

        var.value.sInt = std::stoll(valueToken.value);
    } else if (firstToken.value.rfind("double", 0) == 0) {
        strValue = "";
        type = PrecompiledInstructionArgumentType::NUMBER;

        // Initialize value
        var = VMValue();
        var.isUninitialized = false;
        var.vartype = VMValueType::DOUBLE;
        var.size = sizeof(double);

        // Assert that there is parens around number
        assert((tokens.at(1).type == TokenType::LEFT_PAREN));
        assert((tokens.at(3).type == TokenType::RIGHT_PAREN));

        LexerToken valueToken = tokens.at(2);
        assert((valueToken.type == TokenType::LITERAL));

        var.value.doubleVal = std::stod(valueToken.value);
    }
    else {
        // std::stringstream ss;

        // for (const auto& a : tokens) {
        //     ss << (int)a.type << ' ' << a.value << '\n';
        // }

        // std::cout << ss.rdbuf();

        throw std::runtime_error("Could not parse arguments!");
    }
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
    if (!file.is_open()) throw std::runtime_error("Couldn't open file");

    auto nTokens = tokenizeString(file);

    tokens.insert(tokens.end(), nTokens.begin(), nTokens.end());
}

void Compiler::compileAndWriteBinary(std::string filePath) {
    std::map<std::string, std::vector<PrecompiledInstruction>> sections;
    std::vector<std::string> sectionOrder;

    std::vector<TokenType> labelDefOrder = {
        TokenType::LITERAL,
        // TokenType::LEFT_PAREN,
        // TokenType::LITERAL,
        // TokenType::RIGHT_PAREN,
        TokenType::COLON,
        TokenType::EOL
    };

    // Start the loop of trying to find a label definition and reading the instructions in
    std::string currentLabel = "";

    auto instructionNames = getInstructionNames();

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
                sectionOrder.push_back(currentLabel);
                sections[currentLabel] = std::vector<PrecompiledInstruction>();

                // Parse offset
                // assert((tokens.at(i + 2).type == TokenType::LITERAL));
                // std::uint64_t address = std::stoull(tokens.at(i + 2).value);

                // // Store label address
                // labelAddresses.insert({ currentLabel, address });

                // Jump over rest of label tokens
                i += labelDefOrder.size() - 1;
                continue;
            }
        }

        

        // Read first token as instruction name
        LexerToken nameToken = tokens.at(i);

        // Parse instruction
        assert((nameToken.type == TokenType::LITERAL));

        auto it = std::find(
            instructionNames.begin(),
            instructionNames.end(),
            nameToken.value
        );

        if (it == instructionNames.end()) {
            throw std::runtime_error("No such instruction " + nameToken.value);
        }

        Instructions instr = (Instructions)(it - instructionNames.begin());

        // Parse args if possible

        // If next token is a EOL then this instruction is done
        if (tokens.at(i + 1).type == TokenType::EOL) {
            sections[currentLabel].push_back(
                PrecompiledInstruction(
                    instr,
                    PrecompiledInstructionArgument(),
                    PrecompiledInstructionArgument()));

            i += 1;
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
            sections[currentLabel].push_back(
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
        sections[currentLabel].push_back(
            PrecompiledInstruction(
                instr,
                arg1,
                arg2
            )
        );

        i += 1 + arg2EndOffset - 1;
        continue;

    }

    // At this point we have 2 maps that contain the instructions and now we just need to actually
    // convert to bytecode

    ProgramAssembler assem;

    assert((sections.size() > 0));

    std::map<std::string, std::uint64_t> labelAddresses;

    std::uint64_t currentAddress = 0;
    for (const auto& sectName : sectionOrder) {
        const auto& symbol = sections.at(sectName);

        std::uint64_t sectSize = symbol.size() * ( 1 + sizeof(InstructionArg) * 2);
        labelAddresses[sectName] = currentAddress;
        currentAddress += sectSize;
    }

    for (const auto& sectName : sectionOrder) {
        const auto& currentSection = sections.at(sectName);

        for (const auto& instr : currentSection) {
            InstructionArg arg1;
            InstructionArg arg2;

            if (instr.arg1.type == PrecompiledInstructionArgumentType::LABEL_ADDRESS) {
                arg1 = InstructionArg(labelAddresses.at(instr.arg1.strValue));
            } else {

                if (instr.arg1.type == PrecompiledInstructionArgumentType::REGISTER)
                    arg1.type = InstructionArgType::REGISTER;

                arg1.var = instr.arg1.var;
            }

            if (instr.arg2.type == PrecompiledInstructionArgumentType::LABEL_ADDRESS) {
                arg2 = InstructionArg(labelAddresses.at(instr.arg2.strValue));
            } else {

                if (instr.arg2.type == PrecompiledInstructionArgumentType::REGISTER)
                    arg2.type = InstructionArgType::REGISTER;

                arg2.var = instr.arg2.var;
            }

            assem.insertInstruction(instr.instr, arg1, arg2);
        }
    }

    assem.dumpMemory(filePath);

    return;
}