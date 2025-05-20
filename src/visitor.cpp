#include <iostream>
#include <memory>
#include <variant>
#include "visitor.hpp"
#include "ast.hpp"

void PrintVisitor::visit(TernaryExprNode& expr)  { 
    std::cout << "Ternary(";
    expr.condition.get()->accept(*this); 
    std::cout << " ? ";
    expr.true_expr.get()->accept(*this); 
    std::cout << " : ";
    expr.false_expr.get()->accept(*this); 
    std::cout << ")";
}

void PrintVisitor::visit(LiteralExprNode& expr) {
    std::cout << "Literal(";
    if (std::holds_alternative<int>(expr.value)) {
        std::cout << std::get<int>(expr.value);
    } else if (std::holds_alternative<double>(expr.value)) {
        std::cout << std::get<double>(expr.value);
    } else if (std::holds_alternative<bool>(expr.value)) {
        std::cout << (std::get<bool>(expr.value) ? "true" : "false");
    } else if (std::holds_alternative<char>(expr.value)) {
        std::cout << "'" << std::get<char>(expr.value) << "'";
    } else if (std::holds_alternative<std::string>(expr.value)) {
        std::cout << "\"" << std::get<std::string>(expr.value) << "\"";
    } else {
        std::cout << "Unknown";
    }
    std::cout << ")";
}

void PrintVisitor::visit(IdExprNode& expr) {
    std::cout << "ID(" << expr.name << ")";
}

void PrintVisitor::visit(BinaryExprNode& expr) {
    std::cout << "Binary(" << expr.oper << ", ";
    expr.left->accept(*this);
    std::cout << ", ";
    expr.right->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(UnaryExprNode& expr) {
    std::cout << "Unary(" << expr.oper << ", ";
    expr.operand->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(AssignExprNode& expr) {
    std::cout << "Assign(";
    expr.left->accept(*this);
    std::cout << " = ";
    expr.right->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(PostfixExprNode& expr) {
    std::cout << "Postfix(";
    expr.operand->accept(*this);
    std::cout << expr.oper << ")";
}

void PrintVisitor::visit(MemberAccessExprNode& expr) {
    std::cout << "Access(";
    expr.object->accept(*this);
    std::cout << "." << expr.member << ")";
}

void PrintVisitor::visit(CallExprNode& expr) {
    std::cout << "Call(";
    expr.called->accept(*this);
    std::cout << ", [";
    for (size_t i = 0; i < expr.arguments.size(); ++i) {
        expr.arguments[i]->accept(*this);
        if (i + 1 < expr.arguments.size()) std::cout << ", ";
    }
    std::cout << "])";
}

void PrintVisitor::visit(ArrayAccessExprNode& expr) {
    std::cout << "Array(";
    expr.array->accept(*this);
    std::cout << "[";
    expr.index->accept(*this);
    std::cout << "])";
}

void PrintVisitor::visit(ArrayInitExprNode& expr) {
    std::cout << "ArrayInit([";
    for (size_t i = 0; i < expr.elements.size(); ++i) {
        expr.elements[i]->accept(*this);
        if (i + 1 < expr.elements.size()) std::cout << ", ";
    }
    std::cout << "])";
}

// === Statements ===

void PrintVisitor::visit(ExprStatmNode& stmt) {
    std::cout << "Expr(";
    stmt.expr->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(BlockStatmNode& stmt) {
    std::cout << "Block([";
    for (size_t i = 0; i < stmt.statements.size(); ++i) {
        stmt.statements[i]->accept(*this);
        if (i + 1 < stmt.statements.size()) std::cout << ", ";
    }
    std::cout << "])";
}

void PrintVisitor::visit(ConditionStatmNode& stmt) {
    std::cout << "If(";
    stmt.condition->accept(*this);
    std::cout << ", ";
    stmt.then_statm->accept(*this);
    if (stmt.else_statm) {
        std::cout << ", ";
        stmt.else_statm->accept(*this);
    }
    std::cout << ")";
}

void PrintVisitor::visit(WhileStatmNode& stmt) {
    std::cout << "While(";
    stmt.condition->accept(*this);
    std::cout << ", ";
    stmt.body->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(ForStatmNode& stmt) {
    std::cout << "For(";
    stmt.init->accept(*this);
    std::cout << "; ";
    stmt.condition->accept(*this);
    std::cout << "; ";
    stmt.incr->accept(*this);
    std::cout << ", ";
    stmt.body->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(ReturnStatmNode& stmt) {
    std::cout << "Return(";
    if (stmt.expr) stmt.expr->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(BreakStatmNode&) {
    std::cout << "Break";
}

void PrintVisitor::visit(ContinueStatmNode&) {
    std::cout << "Continue";
}

void PrintVisitor::visit(InStatmNode& stmt) {
    std::cout << "Read(";
    stmt.expr->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(OutStatmNode& stmt) {
    std::cout << "Print(";
    stmt.expr->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(SZFStatmNode& stmt) {
    std::cout << "Sizeof(";
    stmt.expr->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(ExitStatmNode& stmt) {
    std::cout << "Exit(";
    if (stmt.expr) stmt.expr->accept(*this);
    std::cout << ")";
}

// === Declarations ===

void PrintVisitor::visit(VarDeclNode& stmt) {
    std::cout << "VarDecl(" << stmt.type << ", [";
    for (size_t i = 0; i < stmt.variables.size(); ++i) {
        const auto& var = stmt.variables[i];
        std::cout << var.name;
        if (var.size) {
            std::cout << "[";
            var.size->accept(*this);
            std::cout << "]";
        }
        if (var.init) {
            std::cout << " = ";
            var.init->accept(*this);
        }
        if (i + 1 < stmt.variables.size()) std::cout << ", ";
    }
    std::cout << "])";
}

void PrintVisitor::visit(FuncDeclNode& decl) {
    std::cout << "Func(" << decl.func_type << " " << decl.func_name << ", [";
    for (size_t i = 0; i < decl.parameters.size(); ++i) {
        const auto& param = decl.parameters[i];
        std::cout << param.first << " " << param.second;
        if (i + 1 < decl.parameters.size()) std::cout << ", ";
    }
    std::cout << "], ";
    decl.body->accept(*this);
    std::cout << ")";
}

void PrintVisitor::visit(StructDeclNode& decl) {
    std::cout << "Struct(" << decl.name << ", [";
    for (size_t i = 0; i < decl.fields.size(); ++i) {
        visit(decl.fields[i]);
        if (i + 1 < decl.fields.size()) std::cout << ", ";
    }
    std::cout << "])";
}

void PrintVisitor::visit(AssertDeclNode& decl) {
    std::cout << "Assert(";
    decl.expr->accept(*this);
    if (!decl.message.empty()) {
        std::cout << ", \"" << decl.message << "\"";
    }
    std::cout << ")";
}

void PrintVisitor::visit(ASTRootNode& node) {
    std::cout << "Program([" << std::endl;
    for (const auto& stmt : node.statements) {
        stmt->accept(*this);
        std::cout << std::endl;
    }
    std::cout << "])" << std::endl;
}


void TernaryExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void LiteralExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void IdExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BinaryExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void UnaryExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void AssignExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void PostfixExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void MemberAccessExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void CallExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ArrayAccessExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ArrayInitExprNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }

void ExprStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BlockStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ConditionStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void WhileStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ForStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ReturnStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void BreakStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ContinueStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void InStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void OutStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void SZFStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void ExitStatmNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }

void VarDeclNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void FuncDeclNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void StructDeclNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
void AssertDeclNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }

void ASTRootNode::accept(ASTVisitor& visitor) { visitor.visit(*this); }
