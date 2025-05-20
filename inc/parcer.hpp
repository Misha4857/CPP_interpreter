#pragma once

#include "token.hpp"
#include "ast.hpp"

#include <string>
#include <vector>
#include <memory>

class Parcer {
    public:
        explicit Parcer(const std::vector<Token>& token_array);
        void parce();
        std::shared_ptr<ASTNode> getASTRoot() const;

    private:
        const std::vector<Token>& token_array;
        size_t index = 0;
        std::shared_ptr<ASTRootNode> root;

        bool check(TokenType type) const;
        bool check_advance(TokenType type);
        void advance();
        Token peek() const;

        void parcer_starter();
        std::shared_ptr<DeclNode> declaration();
        std::shared_ptr<DeclNode> variable_declaration();
        std::shared_ptr<FuncDeclNode> function_declaration();
        std::shared_ptr<DeclNode> struct_declaration();
        std::shared_ptr<DeclNode> assert_declaration();

        std::shared_ptr<StatmNode> statement();
        std::shared_ptr<StatmNode> expression_statement();
        std::shared_ptr<StatmNode> return_statement();
        std::shared_ptr<StatmNode> break_statement();
        std::shared_ptr<StatmNode> continue_statement();
        std::shared_ptr<StatmNode> conditional_statement();
        std::shared_ptr<StatmNode> block_statement();
        std::shared_ptr<StatmNode> for_statement();
        std::shared_ptr<StatmNode> dowhile_statement();
        std::shared_ptr<StatmNode> while_statement();
        std::shared_ptr<StatmNode> in_statement();
        std::shared_ptr<StatmNode> out_statement();
        std::shared_ptr<StatmNode> sizeof_statement();
        std::shared_ptr<StatmNode> exit_statement();

        std::shared_ptr<ExprNode> expression();
        std::shared_ptr<ExprNode> ternary_expression();
        std::shared_ptr<ExprNode> or_expression();
        std::shared_ptr<ExprNode> and_expression();
        std::shared_ptr<ExprNode> equality_expression();
        std::shared_ptr<ExprNode> comparison_expression();
        std::shared_ptr<ExprNode> term_expression();
        std::shared_ptr<ExprNode> factor_expression();
        std::shared_ptr<ExprNode> unary_expression();
        std::shared_ptr<ExprNode> assign_expression();
        std::shared_ptr<ExprNode> postfix_expression();
        std::shared_ptr<ExprNode> literal_expression();
        std::shared_ptr<ExprNode> id_expression();
        std::shared_ptr<ExprNode> member_access_expression();
        std::shared_ptr<ExprNode> array_access_expression();
        std::shared_ptr<ExprNode> array_initialization_expression();
};