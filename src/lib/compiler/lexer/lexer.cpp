#include "lexer.hpp"

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