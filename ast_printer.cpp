#include "ast_printer.h"

// Print whole program
std::string AstPrinter::print(const std::vector<std::shared_ptr<Stmt>>& statements) {
    std::string result = "(Program\n";
    for (const auto& stmt : statements) {
        if (stmt)
            result += "  " + stmt->accept(*this) + "\n";
    }
    result += ")";
    return result;
}

// Helper
std::string AstPrinter::parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs) {
    std::string result = "(" + name;
    for (const auto& expr : exprs) {
        if (expr)
            result += " " + expr->accept(*this);
        else
            result += " (null)";
    }
    return result + ")";
}

// ==== EXPRESSIONS ====

std::string AstPrinter::visit(std::shared_ptr<BinaryExpr> expr) {
    return parenthesize(expr->op.value, { expr->left, expr->right });
}

std::string AstPrinter::visit(std::shared_ptr<LiteralExpr> expr) {
    if (expr->value.type == STRING)   return "\"" + expr->value.value + "\"";
    if (expr->value.type == BOOLEAN) return expr->value.value == "true" ? "#true" : "#false";
    return expr->value.value;  // number
}

std::string AstPrinter::visit(std::shared_ptr<VariableExpr> expr) {
    return "(var " + expr->name.value + ")";
}

std::string AstPrinter::visit(std::shared_ptr<AssignExpr> expr) {
    return parenthesize("= " + expr->name.value, { expr->value });
}

// ==== STATEMENTS ====

std::string AstPrinter::visit(std::shared_ptr<ExprStmt> stmt) {
    return parenthesize("Expr", { stmt->expression });
}

std::string AstPrinter::visit(std::shared_ptr<AnnounceStmt> stmt) {
    return parenthesize("announce", { stmt->expression });
}

std::string AstPrinter::visit(std::shared_ptr<VarDeclStmt> stmt) {
    std::string name = stmt->typeToken.value + " " + stmt->name.value;
    if (stmt->initializer)
        return parenthesize(name, { stmt->initializer });
    return "(" + name + ")";
}

std::string AstPrinter::visit(std::shared_ptr<BlockStmt> stmt) {
    std::string result = "(block\n";
    for (const auto& s : stmt->statements) {
        if (s)
            result += "    " + s->accept(*this) + "\n";
    }
    result += "  )";
    return result;
}

std::string AstPrinter::visit(std::shared_ptr<LoopStmt> stmt) {
    return "(looplap " + stmt->condition->accept(*this) + "\n  " +
        stmt->body->accept(*this) + "\n)";
}

std::string AstPrinter::visit(std::shared_ptr<FinishlineStmt> stmt) {
    return parenthesize("finishline", { stmt->value });
}

std::string AstPrinter::visit(std::shared_ptr<FuncDefStmt> stmt) {
    return "(function " + stmt->name.value + " ()\n  " +
        stmt->body->accept(*this) + "\n)";
}

// ✅ NEW: if / pitstop
std::string AstPrinter::visit(std::shared_ptr<IfStmt> stmt) {
    std::string result = "(track " + stmt->condition->accept(*this) + "\n";
    result += "  " + stmt->thenBranch->accept(*this);
    if (stmt->elseBranch) {
        result += "\n  (pitstop " + stmt->elseBranch->accept(*this) + ")";
    }
    result += ")";
    return result;
}

// ✅ NEW: listen statement
std::string AstPrinter::visit(std::shared_ptr<ListenStmt> stmt) {
    return "(listen " + stmt->name.value + ")";
}
