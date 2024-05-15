#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

enum class TokenType {
    LITERAL     = 0,
    LEFT_PAREN  = 1,
    RIGHT_PAREN = 2,
    COLON       = 3,
    COMMA       = 4,
    EOL         = 5
};

struct LexerToken {
    std::string value;
    TokenType type;
};

std::vector<LexerToken> tokenizeString(std::ifstream& content);