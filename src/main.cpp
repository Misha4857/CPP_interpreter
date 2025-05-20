#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "lexer.hpp"
#include "token.hpp"
#include "parcer.hpp"
#include "visitor.hpp"

std::string readfile (const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Не получилось получить доступ к файлу: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    try {
        std::string input = readfile("prg.txt");
        Lexer lexer(input);
        std::cout << "лексер нач" << std::endl;
        std::vector<Token> tokens = lexer.tokenize();
        std::cout << "лексер кон" << std::endl;

        for (const Token& token : tokens) {
            std::cout << "Токен: " << tokenTypeToString(token.type)
                      << ", Значение: \"" << token.value << "\"" << std::endl;
        }

        Parcer parcer(tokens);
        std::cout << "парсер нач" << std::endl;
        parcer.parce();
        std::cout << "парсер кон" << std::endl;
        auto ast = parcer.getASTRoot();

        std::cout << "__________________________" << std::endl;
        PrintVisitor visitor;
        ast->accept(visitor);

    } catch (const std::runtime_error& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
