#pragma once

#include "ast.hpp"

struct ASTVisitor {
    virtual void visit(TernaryExprNode& node) = 0;
    virtual void visit(BinaryExprNode& node) = 0;
    virtual void visit(UnaryExprNode& node) = 0;
    virtual void visit(AssignExprNode& node) = 0;
    virtual void visit(PostfixExprNode& node) = 0;
    virtual void visit(LiteralExprNode& node) = 0;
    virtual void visit(IdExprNode& node) = 0;
    virtual void visit(MemberAccessExprNode& node) = 0;
    virtual void visit(CallExprNode& node) = 0;
    virtual void visit(ArrayAccessExprNode& node) = 0;
    virtual void visit(ArrayInitExprNode& node) = 0;

    virtual void visit(ReturnStatmNode& node) = 0;
    virtual void visit(BreakStatmNode& node) = 0;
    virtual void visit(ContinueStatmNode& node) = 0;
    virtual void visit(ConditionStatmNode& node) = 0;
    virtual void visit(ExprStatmNode& node) = 0;
    virtual void visit(BlockStatmNode& node) = 0;
    virtual void visit(ForStatmNode& node) = 0;
    virtual void visit(WhileStatmNode& node) = 0;
    virtual void visit(InStatmNode& node) = 0;
    virtual void visit(OutStatmNode& node) = 0;
    virtual void visit(SZFStatmNode& node) = 0;
    virtual void visit(ExitStatmNode& node) = 0;

    virtual void visit(VarDeclNode& node) = 0;
    virtual void visit(FuncDeclNode& node) = 0;
    virtual void visit(StructDeclNode& node) = 0;
    virtual void visit(AssertDeclNode& node) = 0;

    virtual void visit(ASTRootNode& node) = 0;

    virtual ~ASTVisitor() = default;
};

struct PrintVisitor : ASTVisitor {
    void visit(TernaryExprNode& node) override;
    void visit(BinaryExprNode& node) override;
    void visit(UnaryExprNode& node) override;
    void visit(AssignExprNode& node) override;
    void visit(PostfixExprNode& node) override;
    void visit(LiteralExprNode& node) override;
    void visit(IdExprNode& node) override;
    void visit(MemberAccessExprNode& node) override;
    void visit(CallExprNode& node) override;
    void visit(ArrayAccessExprNode& node) override;
    void visit(ArrayInitExprNode& node) override;

    void visit(ReturnStatmNode& node) override;
    void visit(BreakStatmNode& node) override;
    void visit(ContinueStatmNode& node) override;
    void visit(ConditionStatmNode& node) override;
    void visit(ExprStatmNode& node) override;
    void visit(BlockStatmNode& node) override;
    void visit(ForStatmNode& node) override;
    void visit(WhileStatmNode& node) override;
    void visit(InStatmNode& node) override;
    void visit(OutStatmNode& node) override;
    void visit(SZFStatmNode& node) override;
    void visit(ExitStatmNode& node) override;

    void visit(VarDeclNode& node) override;
    void visit(FuncDeclNode& node) override;
    void visit(StructDeclNode& node) override;
    void visit(AssertDeclNode& node) override;

    void visit(ASTRootNode& node) override;
};
