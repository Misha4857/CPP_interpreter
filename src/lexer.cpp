#include "lexer.hpp"

#include <iostream>
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& input) : input(input) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        Token token = extract();
        tokens.push_back(token);
        if (token.type == TokenType::END_OF_FILE) break;
    }

    return tokens;
}

Token Lexer::extract() {
    while (std::isspace(peek())) advance();

    if (index >= input.size()) {
        return {TokenType::END_OF_FILE, ""};
    }

    if (std::isalpha(peek()) || peek() == '_') {
        return extract_id();
    }
    if (std::isdigit(peek())) {
        return extract_num();
    }
    if (peek() == '\'') {
        return extract_char();
    }
    if (peek() == '"') {     
        return extract_str();
    }
    if (peek() == '/') {
        skip_comment();
    }
    return extract_op();
}

Token Lexer::extract_id() {
    std::size_t start = index;
    while (std::isalnum(peek()) || peek() == '_') advance();
    std::string value = input.substr(start, index - start);
    if (keywords.contains(value)) {
        return {keywords.at(value), value};
    }
    if (value == "true" || value == "false") return {TokenType::BOOL_LIT, value};
    return {TokenType::ID, value};
}

Token Lexer::extract_num() {
    std::size_t start = index;
    while (std::isdigit(peek())) advance();
    if (peek() == '.') {
        advance();
        while (std::isdigit(peek())) advance();
        return {TokenType::FLOAT_LIT, input.substr(start, index - start)};
    }
    return {TokenType::INT_LIT, input.substr(start, index - start)};
}

Token Lexer::extract_char() {
    advance();
    char c;
    if (peek() == '\\') {
        advance();
        if (std::isalpha(peek())) {
            std::string value = "\\" + peek();
            if (esc_chars.contains(value)) return {esc_chars.at(value), value};
        }
        switch (peek()) {
            case '\'' : c = '\'';
            case '\"' : c = '\"';
            case '\\' : c = '\\';
            case '\?' : c = '\?';
            default : report("неизвестный символ \'" + peek() + '\'');
        }
    }

    c = peek();
    advance();
    if (peek() != '\'') report("не закрыта кавычка");
    advance();
    return {TokenType::CHAR_LIT, std::string(1, c)};
}

Token Lexer::extract_str() {
    advance();
    std::size_t start = index;
    while (peek() != '"' && index <= input.size()) {
        advance();
    }

    if (index > input.size() || peek() != '"') report("не закрыта кавычка");
    std::string value = input.substr(start, index - start);
    advance();
    return {TokenType::STR_LIT, value};
}

Token Lexer::extract_op() {
    std::size_t size = 0;
    std::string op;
    while (operators.contains(op + peek(size))) {
        op += peek(size);
        ++size;
    }
    advance(size);
    if (operators.contains(op)) {
        return {operators.at(op), op};
    }
    report("Неизвестный символ: " + op);
}

void Lexer::skip_comment() {
    advance();
    std::string value  = "/" + peek();
    if(!comm_sym.contains(value)) report("Неизвестный символ: \'/" + peek() + '\'');
    std::size_t start = index;
    if (comm_sym.at(value) == TokenType::COMLIT) {
        while(peek() != '\n') advance();
    }
    if (comm_sym.at(value) == TokenType::LCOMLIT) {
        while(!(peek(1) == '/' && peek() == '*')) advance();
    }
    advance();
    extract();
}

char Lexer::peek(std::size_t offset) const noexcept {
    return index + offset < input.size() ? input[index + offset] : '\0';
}

void Lexer::advance(std::size_t offset) noexcept {
    index += offset;
}

void Lexer::report(const std::string& message) const {
    throw std::runtime_error(message);
}

const std::unordered_map<std::string, TokenType> Lexer::comm_sym = {
    {"//", TokenType::COMLIT},
    {"/*", TokenType::LCOMLIT},
    {"*/", TokenType::RCOMLIT},
};

const std::unordered_map<std::string, TokenType> Lexer::esc_chars = {
    {"\\a", TokenType::ESCLIT},
    {"\\b", TokenType::ESCLIT},
    {"\\f", TokenType::ESCLIT},
    {"\\n", TokenType::ESCLIT},
    {"\\r", TokenType::ESCLIT},
    {"\\t", TokenType::ESCLIT},
    {"\\v", TokenType::ESCLIT},
};

const std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"int", TokenType::KW_INT},
    {"float", TokenType::KW_FLOAT},
    {"char", TokenType::KW_CHAR},
    {"bool", TokenType::KW_BOOL},
    {"void", TokenType::KW_VOID},
    {"struct", TokenType::KW_STRUCT},
    {"if", TokenType::KW_IF},
    {"else", TokenType::KW_ELSE},
    {"while", TokenType::KW_WHILE},
    {"do", TokenType::KW_DO},
    {"for", TokenType::KW_FOR},
    {"return", TokenType::KW_RETURN},
    {"break", TokenType::KW_BREAK},
    {"continue", TokenType::KW_CONTINUE},
    {"assert", TokenType::KW_ASSERT},
    {"const", TokenType::KW_CONST},
    {"exit", TokenType::KW_EXIT},
    {"print", TokenType::KW_PRINT},
    {"read", TokenType::KW_READ},
    {"sizeof", TokenType::KW_SIZEOF},
};

const std::unordered_map<std::string, TokenType> Lexer::operators = {
    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::STAR},
    {"/", TokenType::SLASH},
    {"=", TokenType::ASSIGN},
    {"+=", TokenType::PLUS_ASSIGN},
    {"-=", TokenType::MINUS_ASSIGN},
    {"*=", TokenType::STAR_ASSIGN},
    {"/=", TokenType::SLASH_ASSIGN},
    {"==", TokenType::EQ},
    {"!=", TokenType::NEQ},
    {"<", TokenType::LT},
    {">", TokenType::GT},
    {"<=", TokenType::LEQ},
    {">=", TokenType::GEQ},
    {"&&", TokenType::AND},
    {"||", TokenType::OR},
    {"!", TokenType::NOT},
    {"&", TokenType::BIT_AND},
    {"|", TokenType::BIT_OR},
    {"^", TokenType::BIT_XOR},
    {"~", TokenType::BIT_NOT},
    {"<<", TokenType::BIT_SHL},
    {">>", TokenType::BIT_SHR},
    {"++", TokenType::INCREMENT},
    {"--", TokenType::DECREMENT},
    {"?", TokenType::QMARK},
    {":", TokenType::COLON},
    {",", TokenType::COMMA},
    {";", TokenType::SEMICOLON},
    {".", TokenType::DOT},
    {"->", TokenType::ARROW},
    {"(", TokenType::LPAREN},
    {")", TokenType::RPAREN},
    {"{", TokenType::LBRACE},
    {"}", TokenType::RBRACE},
    {"[", TokenType::LBRACKET},
    {"]", TokenType::RBRACKET}
};
