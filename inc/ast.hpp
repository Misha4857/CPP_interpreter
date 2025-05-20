#pragma once

#include <string>
#include <vector>
#include <memory>
#include <variant>

struct ASTVisitor;

// добавить DeclStatement - то же самое, что и ExpressionStatement только для decl

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

struct DeclNode : ASTNode {
    virtual ~DeclNode() = default;
    virtual void accept(ASTVisitor& visitor) override = 0;
};

struct ExprNode : ASTNode {
    virtual ~ExprNode() = default;
    virtual void accept(ASTVisitor& visitor) override = 0;
};

struct StatmNode : ASTNode {
    virtual ~StatmNode() = default;
    virtual void accept(ASTVisitor& visitor) override = 0;
};

struct TernaryExprNode : ExprNode{
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<ExprNode> true_expr;
    std::shared_ptr<ExprNode> false_expr;
    TernaryExprNode(std::shared_ptr<ExprNode> condition, std::shared_ptr<ExprNode> true_expr, std::shared_ptr<ExprNode> false_expr) :
        condition(std::move(condition)), true_expr(std::move(true_expr)), false_expr(std::move(false_expr)) {}
    void accept(ASTVisitor& visitor) override;
};

struct BinaryExprNode : ExprNode{
    std::string oper;
    std::shared_ptr<ExprNode> left;
    std::shared_ptr<ExprNode> right;
    BinaryExprNode(const std::string& oper, std::shared_ptr<ExprNode> left, std::shared_ptr<ExprNode> right) :
        oper(oper), left(std::move(left)), right(std::move(right)) {}
    void accept(ASTVisitor& visitor) override;
};

struct UnaryExprNode : ExprNode{
    std::string oper;
    std::shared_ptr<ExprNode> operand;
    UnaryExprNode(const std::string& oper, std::shared_ptr<ExprNode> operand) :
        oper(oper), operand(std::move(operand)) {}
    void accept(ASTVisitor& visitor) override;
};

struct AssignExprNode : ExprNode{
    std::shared_ptr<ExprNode> left;
    std::shared_ptr<ExprNode> right;
    AssignExprNode(std::shared_ptr<ExprNode> left, std::shared_ptr<ExprNode> right) :
        left(std::move(left)), right(std::move(right)) {}
    void accept(ASTVisitor& visitor) override;
};

struct PostfixExprNode : ExprNode{
    std::string oper;
    std::shared_ptr<ExprNode> operand;
    PostfixExprNode(const std::string& oper, std::shared_ptr<ExprNode> operand) :
        oper(oper), operand(std::move(operand)) {}
    void accept(ASTVisitor& visitor) override;
};

struct LiteralExprNode : ExprNode{
    std::variant<int, double, bool, char, std::string> value;
    explicit LiteralExprNode(std::variant<int, double, bool, char, std::string> value) :
        value(value) {}
    void accept(ASTVisitor& visitor) override;
};

struct IdExprNode : ExprNode{
    std::string name;
    explicit IdExprNode(const std::string& name) :
        name(name) {}
    void accept(ASTVisitor& visitor) override;
};

struct MemberAccessExprNode : ExprNode{
    std::shared_ptr<ExprNode> object;
    std::string member;
    MemberAccessExprNode(std::shared_ptr<ExprNode> object, const std::string& member) :
        object(std::move(object)), member(member) {}
    void accept(ASTVisitor& visitor) override;
};

struct CallExprNode : ExprNode {
    std::shared_ptr<ExprNode> called;
    std::vector<std::shared_ptr<ExprNode>> arguments;
    explicit CallExprNode(std::shared_ptr<ExprNode> called, std::vector<std::shared_ptr<ExprNode>> arguments) :
        called(called), arguments(arguments) {}
    void accept(ASTVisitor& visitor) override;
};

struct ArrayAccessExprNode : ExprNode{
    std::shared_ptr<ExprNode> array;
    std::shared_ptr<ExprNode> index;
    ArrayAccessExprNode(std::shared_ptr<ExprNode> array, std::shared_ptr<ExprNode> index) :
        array(std::move(array)), index(std::move(index)) {}
    void accept(ASTVisitor& visitor) override;
};

struct ArrayInitExprNode : ExprNode {
    std::vector<std::shared_ptr<ExprNode>> elements;
    ArrayInitExprNode(std::vector<std::shared_ptr<ExprNode>> elements) :
        elements(std::move(elements)) {}
    void accept(ASTVisitor& visitor) override;
};

struct ReturnStatmNode : StatmNode {
    std::shared_ptr<StatmNode> expr;
    ReturnStatmNode(std::shared_ptr<StatmNode> expr = nullptr) :
        expr(std::move(expr)) {}
    void accept(ASTVisitor& visitor) override;
};

struct BreakStatmNode : StatmNode {
    void accept(ASTVisitor& visitor) override;
};

struct ContinueStatmNode : StatmNode {
    void accept(ASTVisitor& visitor) override;
};

struct ConditionStatmNode : StatmNode {
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<StatmNode> then_statm;
    std::shared_ptr<StatmNode> else_statm;
    ConditionStatmNode(std::shared_ptr<ExprNode> condition, std::shared_ptr<StatmNode> then_statm, std::shared_ptr<StatmNode> else_statm = nullptr) :
        condition(std::move(condition)), then_statm(std::move(then_statm)), else_statm(std::move(else_statm)) {}
    void accept(ASTVisitor& visitor) override;
};

struct ExprStatmNode : StatmNode {
    std::shared_ptr<ASTNode> expr;
    explicit ExprStatmNode(std::shared_ptr<ASTNode> expr) :
        expr(std::move(expr)) {}
    void accept(ASTVisitor& visitor) override;
};

struct BlockStatmNode : StatmNode {
    std::vector<std::shared_ptr<StatmNode>> statements;
    void accept(ASTVisitor& visitor) override;
};

struct ForStatmNode : StatmNode {
    std::shared_ptr<DeclNode> init; // decl statement or expr
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<ExprNode> incr;
    std::shared_ptr<StatmNode> body;
    ForStatmNode(std::shared_ptr<DeclNode> init, std::shared_ptr<ExprNode> condition, std::shared_ptr<ExprNode> incr, std::shared_ptr<StatmNode> body) :
        init(std::move(init)), condition(std::move(condition)), incr(std::move(incr)), body(std::move(body)) {}
    void accept(ASTVisitor& visitor) override;
};

struct WhileStatmNode : StatmNode {
    std::shared_ptr<ExprNode> condition;
    std::shared_ptr<StatmNode> body;
    WhileStatmNode(std::shared_ptr<ExprNode> condition, std::shared_ptr<StatmNode> body) :
        condition(std::move(condition)), body(std::move(body)) {}
    void accept(ASTVisitor& visitor) override;
};

struct InputStatmNode : StatmNode {
    std::shared_ptr<StatmNode> expr;
    InputStatmNode(std::shared_ptr<StatmNode> expr) :
        expr(std::move(expr)) {}
    void accept(ASTVisitor& visitor) override;
};

struct OutStatmNode : StatmNode {
    std::shared_ptr<StatmNode> expr;
    OutStatmNode(std::shared_ptr<StatmNode> expr) :
        expr(std::move(expr)) {}
    void accept(ASTVisitor& visitor) override;
};

struct SZFStatmNode : StatmNode { // sizof это опреатор, сделать его оператором
    std::shared_ptr<ExprNode> expr;
    SZFStatmNode(std::shared_ptr<ExprNode> expr) :
        expr(std::move(expr)) {}
    void accept(ASTVisitor& visitor) override;
};

struct ExitStatmNode : StatmNode {
    std::shared_ptr<ExprNode> expr;
    ExitStatmNode(std::shared_ptr<ExprNode> expr) :
        expr(std::move(expr)) {}
    void accept(ASTVisitor& visitor) override;
};

struct VariableNode { 
    std::string name;
    std::shared_ptr<ExprNode> init;
    std::shared_ptr<ExprNode> size;
    VariableNode(const std::string& name, std::shared_ptr<ExprNode> init, std::shared_ptr<ExprNode> size) :
        name(name), init(std::move(init)), size(std::move(size)) {}
};

struct VarDeclNode : DeclNode {
    std::string type;
    std::vector<VariableNode> variables;
    VarDeclNode(const std::string& type, std::vector<VariableNode> variables) :
        type(type), variables(std::move(variables)) {}
    void accept(ASTVisitor& visitor) override;
};

struct FuncDeclNode : DeclNode {
    std::string func_type;
    std::string func_name;
    std::vector<std::pair<std::string, std::string>> parameters;
    std::shared_ptr<BlockStatmNode> body;
    FuncDeclNode(const std::string& func_type, const std::string& func_name, const std::vector<std::pair<std::string, std::string>>& parameters, std::shared_ptr<BlockStatmNode> body = nullptr) :
        func_type(func_type), func_name(func_name), parameters(parameters), body(std::move(body)) {}
    void accept(ASTVisitor& visitor) override;
};

struct StructDeclNode : DeclNode {
    std::string name;
    std::vector<VarDeclNode> fields;
    StructDeclNode(const std::string& name, const std::vector<VarDeclNode>& fields) :
        name(name), fields(fields) {}
    void accept(ASTVisitor& visitor) override;
};

struct AssertDeclNode : DeclNode {
    std::shared_ptr<ExprNode> expr;
    std::string message;
    AssertDeclNode(std::shared_ptr<ExprNode> expr, std::string message) :
        expr(std::move(expr)), message(message) {}
    void accept(ASTVisitor& visitor) override;
};

struct ASTRootNode : ASTNode{
    std::vector<std::shared_ptr<ASTNode>> statements;
    ASTRootNode() = default;
    ASTRootNode(std::vector<std::shared_ptr<ASTNode>> statements) :
        statements(std::move(statements)) {}
    void accept(ASTVisitor& visitor) override;
};
