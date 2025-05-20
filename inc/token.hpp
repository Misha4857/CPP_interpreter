#pragma once

#include <string>

enum class TokenType {

    //идентификаторы и литералы
    ID,             //идентификатор
    INT_LIT,        //литерал целых
    FLOAT_LIT,      //литерал плав.тчк
    CHAR_LIT,       //литерал символов
    STR_LIT,        //литерал строк
    BOOL_LIT,       //литерал boolean

    KW_INT, KW_FLOAT, KW_CHAR, KW_BOOL, KW_VOID,

    //ключевые слова
    KW_STRUCT, KW_IF, KW_ELSE, KW_WHILE, KW_DO, KW_FOR, KW_RETURN, KW_BREAK, KW_CONTINUE,
    KW_ASSERT, KW_CONST, KW_EXIT, KW_PRINT, KW_READ, KW_SIZEOF,

    //операторы
    PLUS, MINUS, SLASH, STAR, PERCENT,
    ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN, STAR_ASSIGN, SLASH_ASSIGN, PERCENT_ASSIGN,
    EQ, NEQ, LT, GT, LEQ, GEQ,
    AND, OR, NOT,
    BIT_AND, BIT_OR, BIT_XOR, BIT_NOT, BIT_SHL, BIT_SHR,
    INCREMENT, DECREMENT,
    QMARK, COLON,

    //пунктуаторы
    COMMA, SEMICOLON, DOT, ARROW,
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,

    //комментарии
    COMLIT, LCOMLIT, RCOMLIT,
    COMMENT,

    //спец. токены
    END_OF_FILE, ERROR, ESCLIT,

};

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::ID: return "ID";
        case TokenType::INT_LIT: return "INT_LIT";
        case TokenType::FLOAT_LIT: return "FLOAT_LIT";
        case TokenType::CHAR_LIT: return "CHAR_LIT";
        case TokenType::STR_LIT: return "STR_LIT";
        case TokenType::KW_STRUCT: return "KW_STRUCT";
        case TokenType::KW_INT: return "KW_INT";
        case TokenType::KW_FLOAT: return "KW_FLOAT";
        case TokenType::KW_CHAR: return "KW_CHAR";
        case TokenType::KW_BOOL: return "KW_BOOL";
        case TokenType::KW_VOID: return "KW_VOID";
        case TokenType::KW_IF: return "KW_IF";
        case TokenType::KW_ELSE: return "KW_ELSE";
        case TokenType::KW_WHILE: return "KW_WHILE";
        case TokenType::KW_DO: return "KW_DO";
        case TokenType::KW_FOR: return "KW_FOR";
        case TokenType::KW_RETURN: return "KW_RETURN";
        case TokenType::KW_BREAK: return "KW_BREAK";
        case TokenType::KW_CONTINUE: return "KW_CONTINUE";
        case TokenType::KW_PRINT: return "KW_PRINT";
        case TokenType::KW_READ: return "KW_READ";
        case TokenType::KW_SIZEOF: return "KW_SIZEOF";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::SLASH: return "SLASH";
        case TokenType::STAR: return "STAR";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::PLUS_ASSIGN: return "PLUS_ASSIGN";
        case TokenType::MINUS_ASSIGN: return "MINUS_ASSIGN";
        case TokenType::STAR_ASSIGN: return "STAR_ASSIGN";
        case TokenType::SLASH_ASSIGN: return "SLASH_ASSIGN";
        case TokenType::EQ: return "EQ";
        case TokenType::NEQ: return "NEQ";
        case TokenType::LT: return "LT";
        case TokenType::GT: return "GT";
        case TokenType::LEQ: return "LEQ";
        case TokenType::GEQ: return "GEQ";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        case TokenType::BIT_AND: return "BIT_AND";
        case TokenType::BIT_OR: return "BIT_OR";
        case TokenType::BIT_XOR: return "BIT_XOR";
        case TokenType::BIT_NOT: return "BIT_NOT";
        case TokenType::BIT_SHL: return "BIT_SHL";
        case TokenType::BIT_SHR: return "BIT_SHR";
        case TokenType::INCREMENT: return "INCREMENT";
        case TokenType::DECREMENT: return "DECREMENT";
        case TokenType::COLON: return "COLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::DOT: return "DOT";
        case TokenType::ARROW: return "ARROW";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::LBRACKET: return "LBRACKET";
        case TokenType::RBRACKET: return "RBRACKET";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        default: return "UNKNOWN";
    }
}

struct Token {
    TokenType type;
    std::string value;

    bool operator==(TokenType type) const {
        return this->type == type;
    }
};