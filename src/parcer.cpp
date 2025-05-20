#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "parcer.hpp"
#include "ast.hpp"

Parcer::Parcer(const std::vector<Token>& token_array) :
    token_array(token_array) {}

void Parcer::parce() {
    root = std::make_shared<ASTRootNode>();
    parcer_starter();
}

std::shared_ptr<ASTNode> Parcer::getASTRoot() const {
    return root;
}

void Parcer::parcer_starter() {
    while (!check(TokenType::END_OF_FILE)) {
        root->statements.push_back(declaration());
    }
}

bool Parcer::check(TokenType type) const {
    return index < token_array.size() && token_array[index].type == type;
}

void Parcer::advance() {
    if (index < token_array.size())
        index++;
}

bool Parcer::check_advance(TokenType type) {
    if(check(type)){
        advance();
        return true;
    }
    return false;
}

Token Parcer::peek() const {
    return token_array[index];
}

std::shared_ptr<DeclNode> Parcer::declaration() {
    if (
        check(TokenType::KW_INT) ||
        check(TokenType::KW_FLOAT) ||
        check(TokenType::KW_CHAR) ||
        check(TokenType::KW_BOOL) ||
        check(TokenType::ID) ||
        check(TokenType::KW_VOID) /*check(SEMICOLON)*/
    ) {
        if (token_array[index + 2].type == TokenType::LPAREN)
            return function_declaration();       // вызвать функцию которая парсит и у функции и у переменной
        else
            return variable_declaration();
    }
    else if (check_advance(TokenType::KW_STRUCT))
        return struct_declaration();
    else if (check_advance(TokenType::KW_ASSERT))
        return assert_declaration();
    else 
        throw std::runtime_error("Неверный токен в декларации: " + token_array[index].value);
}

std::shared_ptr<DeclNode> Parcer::variable_declaration() {
    if (check_advance(TokenType::SEMICOLON)) // вот это убрать
        return nullptr;

    auto type = peek().value;
    advance();

    std::vector<VariableNode> variables;

    do {
        auto name = peek().value;
        advance();
        std::shared_ptr<ExprNode> init = nullptr;
        std::shared_ptr<ExprNode> size = nullptr;

        if (check_advance(TokenType::LBRACKET)) {
            if (!check(TokenType::RBRACKET)){
                size = expression();
            }
            if (!check_advance(TokenType::RBRACKET))
                throw std::runtime_error("Не закрыта квадратная скобка после объявления размера массива");
        }
        if (check_advance(TokenType::ASSIGN) /*|| check(TokenType::LBRACE) убрать нахуй*/) {
            if (check(TokenType::LBRACE)) init = array_initialization_expression();
            else init = expression();
        }
        variables.emplace_back(name, init, size);
    } while (check_advance(TokenType::COMMA));

    if (!check_advance(TokenType::SEMICOLON))
        throw std::runtime_error("Пропущена точка с запятой [1]");

    return std::make_shared<VarDeclNode>(type, variables);

}

std::shared_ptr<FuncDeclNode> Parcer::function_declaration() {
    auto func_type = peek().value;
    advance();
    auto func_name = peek().value;
    advance();
    if (!check_advance(TokenType::LPAREN))
        throw std::runtime_error("Пропущена открывающаяся скобка для параметров ф-ции");
    
    std::vector<std::pair<std::string, std::string>> parameters;
    while (!check(TokenType::RPAREN)) {  // стоит перекинуть  в отдель ную функцию
        auto type = peek().value;
        advance();
        auto name = peek().value;
        advance();
        parameters.emplace_back(type, name);
        if (!check(TokenType::RPAREN)) 
            if (!check_advance(TokenType::COMMA))
                throw std::runtime_error("Пропущена запятая между параметрами");
    }
    if (!check_advance(TokenType::RPAREN))
        throw std::runtime_error("Пропущена закрывающая скобка для параметров ф-ции");
    
    if (check_advance(TokenType::SEMICOLON))
        return std::make_shared<FuncDeclNode>(func_type, func_name, parameters, nullptr);
    
    auto body = std::dynamic_pointer_cast<BlockStatmNode>(block_statement());
    return std::make_shared<FuncDeclNode>(func_type, func_name, parameters, body);
}

std::shared_ptr<DeclNode> Parcer::struct_declaration() {
    auto name = peek().value;
    advance();
    if (!check_advance(TokenType::LBRACE))
        throw std::runtime_error("Не открыты фигурные скобки для структуры");
    
    std::vector<VarDeclNode> fields;
    while (!check(TokenType::RBRACE)) {
        fields.push_back(*std::dynamic_pointer_cast<VarDeclNode>(variable_declaration()));
    }

    if (!check_advance(TokenType::RBRACE))
        throw std::runtime_error("Не закрыты фигурные скобки структуры");

    if (!check_advance(TokenType::SEMICOLON)) // по идее не нужна
        throw std::runtime_error("Пропущена точка с запятой после объявления структуры [2]");
    return std::make_shared<StructDeclNode>(name, fields);
}

std::shared_ptr<DeclNode> Parcer::assert_declaration() {
    if (!check_advance(TokenType::LPAREN))
        throw std::runtime_error("нужны скобочки для ассерта");

    auto expr = expression();
    std::string message = "";

    if (check_advance(TokenType::COMMA)) {
        if (!check(TokenType::STR_LIT)) {
            throw std::runtime_error("после запятой в ассерте ожидается строка");
        }
        auto message = peek().value;
        advance();
    }
    if (!check_advance(TokenType::RPAREN))
        throw std::runtime_error("Ожидалось закрытие скобочек)))");
    
    if (!check_advance(TokenType::SEMICOLON))
        throw std::runtime_error("Ожидалась точка с запятой после ассерта [3]");

    return std::make_shared<AssertDeclNode>(expr, message);
}


std::shared_ptr<StatmNode> Parcer::statement() {
    if (check_advance(TokenType::KW_IF)) return conditional_statement();
    if (check_advance(TokenType::KW_WHILE)) return while_statement();
    if (check_advance(TokenType::KW_DO)) return dowhile_statement();
    if (check_advance(TokenType::KW_FOR)) return for_statement();
    if (check_advance(TokenType::KW_RETURN)) return return_statement();
    if (check_advance(TokenType::KW_BREAK)) return break_statement();
    if (check_advance(TokenType::KW_CONTINUE)) return continue_statement();
    if (check_advance(TokenType::KW_EXIT)) return exit_statement();
    if (check_advance(TokenType::KW_PRINT)) return out_statement();
    if (check_advance(TokenType::KW_READ)) return in_statement();
    if (check_advance(TokenType::KW_SIZEOF)) return sizeof_statement(); // его тогда не сюда
    if (check(TokenType::LBRACE)) return block_statement();
    if ((check(TokenType::KW_INT) || check(TokenType::KW_FLOAT) ||
        check(TokenType::KW_CHAR) || check(TokenType::KW_BOOL) ||
        check(TokenType::ID) && token_array[index + 1].type == TokenType::ID)) {
        return std::make_shared<ExprStatmNode>(variable_declaration());
    }
    return expression_statement();
}

std::shared_ptr<StatmNode> Parcer::expression_statement() {
    if (check_advance(TokenType::SEMICOLON)) {
        return nullptr;
    }
    auto expr = expression();

    // тут тоже добавить проверку на тточку с запятой

    return std::make_shared<ExprStatmNode>(expr);
}

// заменит на хэкспект выволфы ошибок

std::shared_ptr<StatmNode> Parcer::conditional_statement() {
    if (!check_advance(TokenType::LPAREN))
        throw std::runtime_error("Ожидалась скобка после условного оператора");
    auto condition = expression();
    if (!check_advance(TokenType::RPAREN))
        throw std::runtime_error("Ожидалось закрытие скобки после условия");
    auto then_statm = statement();
    std::shared_ptr<StatmNode> else_statm = nullptr;
    if (check_advance(TokenType::KW_ELSE)) else_statm = statement();
    return std::make_shared<ConditionStatmNode>(condition, then_statm, else_statm);
}

std::shared_ptr<StatmNode> Parcer::while_statement() {
    if (!check_advance(TokenType::LPAREN))
        throw std::runtime_error("Ожидалось открытие скобки для условия цикла");
    auto condition = expression();
    if (!check_advance(TokenType::RPAREN))
        throw std::runtime_error("Ожидалось закрытие скобки после условия");
    auto body = statement();
    return std::make_shared<WhileStatmNode>(condition, body);
}

std::shared_ptr<StatmNode> Parcer::dowhile_statement() {
    auto body = statement();
    if (!check_advance(TokenType::KW_WHILE))
        throw std::runtime_error("Ожидалось 'пока' после 'делай'");
    if (!check_advance(TokenType::LPAREN))
        throw std::runtime_error("Ожидалось открытие скобки для условия цикла");
    auto condition = expression();
    if (!check_advance(TokenType::RPAREN))
        throw std::runtime_error("Ожидалось закрытие скобки после условия");

    // проверка на ;
    return std::make_shared<WhileStatmNode>(condition, body);
}

std::shared_ptr<StatmNode> Parcer::for_statement() { // переделать потому что не только инит
    if (!check_advance(TokenType::LPAREN))
        throw std::runtime_error("Ожидалось открытие скобки для условия цикла");
    std::shared_ptr<DeclNode> init = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        if (check(TokenType::KW_INT) || check(TokenType::KW_FLOAT) ||
            check(TokenType::KW_CHAR) || check(TokenType::KW_BOOL)) {
            init = variable_declaration();
        } else {
            init = std::dynamic_pointer_cast<DeclNode>(expression());
            if (!check_advance(TokenType::SEMICOLON))
                throw std::runtime_error("Ожидалась точка с запятой после инициализации [5]");
        }
    }
    auto condition = check(TokenType::SEMICOLON) ? nullptr : expression();
    if (!check_advance(TokenType::SEMICOLON))
        throw std::runtime_error("Ожидалась точка с запятой после условия [6]");
    auto incr = check(TokenType::RPAREN) ? nullptr : expression();
    if (!check_advance(TokenType::RPAREN))
        throw std::runtime_error("Ожидалось закрытие скобки после цикла фор");
    auto body = statement();
    return std::make_shared<ForStatmNode>(init, condition, incr, body);
}

std::shared_ptr<StatmNode> Parcer::return_statement() {
    std::shared_ptr<StatmNode> expr = nullptr;
    if (!check(TokenType::SEMICOLON))
        expr = statement();
    return std::make_shared<ReturnStatmNode>(expr);
}

std::shared_ptr<StatmNode> Parcer::break_statement() {
    if (!check_advance(TokenType::SEMICOLON))
        throw std::runtime_error("Ожидалась точка с запятой [9]");
    return std::make_shared<BreakStatmNode>();
}
std::shared_ptr<StatmNode> Parcer::continue_statement() {
    if (!check_advance(TokenType::SEMICOLON))
        throw std::runtime_error("Ожидалась точка с запятой [10]");
    return std::make_shared<ContinueStatmNode>();
}

std::shared_ptr<StatmNode> Parcer::exit_statement() {
    if (!check_advance(TokenType::LPAREN))
        throw std::runtime_error("Ожидалось открытие скобки");
    auto expr = expression();
    if (!check_advance(TokenType::RPAREN))
        throw std::runtime_error("Ожидалось закрытие скобки");
    if (!check_advance(TokenType::SEMICOLON))
        throw std::runtime_error("Ожидалась точка с запятой [11]");
    return std::make_shared<ExitStatmNode>(expr);
}

std::shared_ptr<StatmNode> Parcer::out_statement() {
    auto type = token_array[index - 1].type;
    std::shared_ptr<StatmNode> expr = nullptr;
    if (!check_advance(TokenType::LPAREN))
        throw std::runtime_error("Ожидалось открытие скобки");
    if (type == TokenType::KW_PRINT)
        expr = statement();
    
    if (!check_advance(TokenType::RPAREN))
        throw std::runtime_error("ожидалось закрытие скобки1");
    if (!check_advance(TokenType::SEMICOLON))
        throw std::runtime_error("Ожидалась точка с запятой [11]");

    return std::make_shared<OutStatmNode>(expr);
}

std::shared_ptr<StatmNode> Parcer::in_statement() {
    auto type = token_array[index - 1].type;
    std::shared_ptr<StatmNode> expr = nullptr;
    if (!check_advance(TokenType::LPAREN))
        throw std::runtime_error("Ожидалось открытие скобки");
    if (type == TokenType::KW_READ){
        expr = statement();
    }
    if (!check_advance(TokenType::RPAREN))
        throw std::runtime_error("ожидалось закрытие скобки2");
    if (!check_advance(TokenType::SEMICOLON))
        throw std::runtime_error("Ожидалась точка с запятой [11]");

    return std::make_shared<InputStatmNode>(expr);
}

std::shared_ptr<StatmNode> Parcer::sizeof_statement() {
    auto type = token_array[index - 1].type;
    std::shared_ptr<ExprNode> expr = nullptr;
    if (!check_advance(TokenType::LPAREN))
        throw std::runtime_error("Ожидалось открытие скобки");
    if (type == TokenType::KW_SIZEOF)
        expr = expression();
    if (!check_advance(TokenType::RPAREN))
        throw std::runtime_error("ожидалось закрытие скобки3");
    if (!check_advance(TokenType::SEMICOLON))
        throw std::runtime_error("Ожидалась точка с запятой [12.5]");

    return std::make_shared<SZFStatmNode>(expr);
}

std::shared_ptr<StatmNode> Parcer::block_statement() {
    if (!check_advance(TokenType::LBRACE))
        throw std::runtime_error("Ожидалась фигурная скобка");
    auto body = std::make_shared<BlockStatmNode>();
    while (!check(TokenType::RBRACE) && !check(TokenType::END_OF_FILE)) {
        body -> statements.push_back(statement());
    }
    if (!check_advance(TokenType::RBRACE))
        throw std::runtime_error("ожидалось закрытие фигурной скобки");
    return body;
}


std::shared_ptr<ExprNode> Parcer::expression() {
    auto expr = assign_expression();
    while (check_advance(TokenType::COMMA)) {
        auto next = assign_expression();
        expr = std::make_shared<BinaryExprNode>(",", expr, next);
    }
    return expr;
}

std::shared_ptr<ExprNode> Parcer::assign_expression() {
    auto expr = ternary_expression();
    if (check_advance(TokenType::ASSIGN)) {
        auto value = assign_expression();
        return std::make_shared<AssignExprNode>(expr, value);
    }
    return expr;
}

std::shared_ptr<ExprNode> Parcer::ternary_expression() {
    auto condition = or_expression();
    if (check_advance(TokenType::QMARK)) {
        auto true_expr = expression();
        if (!check_advance(TokenType::COLON))
            throw std::runtime_error("Ожидалось двоеточие");
        auto false_expr = expression();
        return std::make_shared<TernaryExprNode>(condition, true_expr, false_expr);
    }
    return condition;
}

std::shared_ptr<ExprNode> Parcer::or_expression() {
    auto expr = and_expression();
    while (check_advance(TokenType::OR)) {
        auto oper2 = and_expression();
        expr = std::make_shared<BinaryExprNode>("||", expr, oper2);
    }
    return expr;
}

std::shared_ptr<ExprNode> Parcer::and_expression() {
    auto expr = equality_expression();
    while (check_advance(TokenType::AND)) {
        auto oper2 = equality_expression();
        expr = std::make_shared<BinaryExprNode>("&&", expr, oper2);
    }
    return expr;
}

std::shared_ptr<ExprNode> Parcer::equality_expression() {
    auto expr = comparison_expression();
    while (check_advance(TokenType::EQ) || check_advance(TokenType::NEQ)) {
        auto oper = token_array[index - 1].value;
        auto operand2 = comparison_expression();
        expr = std::make_shared<BinaryExprNode>(oper, expr, operand2);
    }
    return expr;
}

std::shared_ptr<ExprNode> Parcer::comparison_expression() {
    auto expr = term_expression();
    while (check_advance(TokenType::LT) || check_advance(TokenType::GT) || check_advance(TokenType::LEQ) || check_advance(TokenType::GEQ)) {
        auto oper = token_array[index - 1].value;
        auto operand2 = term_expression();
        expr = std::make_shared<BinaryExprNode>(oper, expr, operand2);
    }
    return expr;
}

std::shared_ptr<ExprNode> Parcer::term_expression() {
    auto expr = factor_expression();
    while (check_advance(TokenType::PLUS) || check_advance(TokenType::MINUS)) {
        auto oper = token_array[index - 1].value;
        auto operand2 = factor_expression();
        expr = std::make_shared<BinaryExprNode>(oper, expr, operand2);
    }
    return expr;
}

std::shared_ptr<ExprNode> Parcer::factor_expression() {
    auto expr = unary_expression();
    while (check_advance(TokenType::STAR) || check_advance(TokenType::SLASH) || check_advance(TokenType::PERCENT)) {
        auto oper = token_array[index - 1].value;
        auto operand2 = unary_expression();
        expr = std::make_shared<BinaryExprNode>(oper, expr, operand2);
    }
    return expr;
}

std::shared_ptr<ExprNode> Parcer::unary_expression() {  // проверить префиквсы тоже
    if (check_advance(TokenType::PLUS) || check_advance(TokenType::MINUS) || check_advance(TokenType::NOT)) {
        auto oper = token_array[index - 1].value;
        auto operand = assign_expression();
        return std::make_shared<UnaryExprNode>(oper, operand);
    }
    return postfix_expression();
}

std::shared_ptr<ExprNode> Parcer::postfix_expression() {
    auto expr = literal_expression();
    while (true) {
        if (check_advance(TokenType::INCREMENT) || check_advance(TokenType::DECREMENT)) {
            auto oper = token_array[index - 1].value;
            expr = std::make_shared<PostfixExprNode>(oper, expr);
        } else if (check_advance(TokenType::LBRACKET)) {
            auto ind = expression();
            if (!check_advance(TokenType::RBRACKET))
                throw std::runtime_error("Ожидалось закрытие квадратной скобки после индекса");
            expr = std::make_shared<ArrayAccessExprNode>(expr, ind);
        } else if (check_advance(TokenType::LPAREN)) { // отдельные функции лучше
            std::vector<std::shared_ptr<ExprNode>> arguments;
            if (!check(TokenType::RPAREN)) {
                do {
                    arguments.push_back(expression());
                } while (check_advance(TokenType::COMMA));
            }
            if (!check_advance(TokenType::RPAREN))
                throw std::runtime_error("ожидалось закрытие скобки после аргументов ф-ции");
            expr = std::make_shared<CallExprNode>(expr, arguments);
        } else if (check_advance(TokenType::DOT)) {
            advance();
            auto member = peek().value;
            expr = std::make_shared<MemberAccessExprNode>(expr, member);
        } else 
            break;
    }

    return expr;
}

std::shared_ptr<ExprNode> Parcer::literal_expression() {
    if (check_advance(TokenType::INT_LIT)) {
        return std::make_shared<LiteralExprNode>(std::stoi(token_array[index - 1].value));
    } else if (check_advance(TokenType::FLOAT_LIT)) {
        return std::make_shared<LiteralExprNode>(std::stod(token_array[index - 1].value));
    } else if (check_advance(TokenType::CHAR_LIT)) {
        return std::make_shared<LiteralExprNode>(token_array[index - 1].value[0]);
    } else if (check_advance(TokenType::STR_LIT)) {
        return std::make_shared<LiteralExprNode>(token_array[index - 1].value);
    } else if (check_advance(TokenType::BOOL_LIT)) {
        if (token_array[index - 1].value == "true") {
            return std::make_shared<LiteralExprNode>(true);    
        } else if (token_array[index - 1].value == "false") {
            return std::make_shared<LiteralExprNode>(false);
        }
    } else if (check_advance(TokenType::ID)) {
        return std::make_shared<LiteralExprNode>(token_array[index - 1].value);
    } else if (check_advance(TokenType::LPAREN)) {
        auto expr = expression();
        if (!check_advance(TokenType::RPAREN)) 
            throw std::runtime_error("ожидалось закрытие скобки");
        return expr;
    }

    throw std::runtime_error("Неизвестный токен: " + peek().value);
}

std::shared_ptr<ExprNode> Parcer::array_initialization_expression() {
    if (!check_advance(TokenType::LBRACE))
        throw std::runtime_error("Ожидалось открытие квадратной скобки для инициализации массива");
    std::vector<std::shared_ptr<ExprNode>> elements;
    while (!check(TokenType::RBRACE)) {
        elements.push_back(expression());
    }
    if (!check_advance(TokenType::RBRACE))
        throw std::runtime_error("Ожидалось закрытие квадратной скобки после конца инициализации массива");
    return std::make_shared<ArrayInitExprNode>(elements);
}