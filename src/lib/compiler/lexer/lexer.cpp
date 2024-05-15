#include "lexer.hpp"

#include <algorithm>

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

std::vector<LexerToken> tokenizeString(std::ifstream& content) {
    std::vector<LexerToken> tokens;

    std::string accumulator;

    auto appendLiterals = [&]() {
        if (accumulator.size() > 0) {
            tokens.push_back(
                LexerToken{ .value=accumulator, .type=TokenType::LITERAL}
            );
            accumulator.clear();
        }
    };

    std::string line;
    while(std::getline(content, line)) {
        // Trim the line of spaces and whitespace first
        ltrim(line);
        rtrim(line);

        if (line.size() == 0) continue;

        for (char c : line) {
            if (isspace(c)) {
                appendLiterals();
                continue;
            }

            switch(c) {
                case '(': {
                    appendLiterals();
                    tokens.push_back(LexerToken{.value="", .type=TokenType::LEFT_PAREN});
                    continue;
                } break;
                case ')': {
                    appendLiterals();
                    tokens.push_back(LexerToken{.value="", .type=TokenType::RIGHT_PAREN});
                    continue;
                } break;
                case ':': {
                    appendLiterals();
                    tokens.push_back(LexerToken{.value="",.type=TokenType::COLON});
                    continue;
                } break;
                case ',': {
                    appendLiterals();
                    tokens.push_back(LexerToken{.value="",.type=TokenType::COMMA});
                    continue;
                } break;
            }

            accumulator.push_back(c);
        }

        appendLiterals();
        tokens.push_back(LexerToken{.value="", .type=TokenType::EOL});
    }

    return tokens;
}