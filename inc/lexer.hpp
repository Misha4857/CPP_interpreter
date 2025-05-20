#pragma once

#include "token.hpp"

#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
public:
    Lexer(const std::string&);
    std::vector<Token> tokenize();

private:
    std::string input;
    std::size_t index = 0;

    Token extract();
    Token extract_id();
    Token extract_num();
    Token extract_char();
    Token extract_str();
    Token extract_op();
    void skip_comment();

    char peek(std::size_t = 0) const noexcept;
    void advance(std::size_t = 1) noexcept;
    [[noreturn]] void report(const std::string&) const;

    static const std::unordered_map<std::string, TokenType> comm_sym;
    static const std::unordered_map<std::string, TokenType> esc_chars;
    static const std::unordered_map<std::string, TokenType> keywords;
    static const std::unordered_map<std::string, TokenType> operators;
};