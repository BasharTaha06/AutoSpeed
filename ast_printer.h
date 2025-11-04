#pragma once

#include "parser.h"
#include <string>
#include <vector>
#include <memory>

class AstPrinter : public ExprVisitor<std::string>, public StmtVisitor<std::string> {
public:
    std::string print(const std::vector<std::shared_ptr<Stmt>>& statements);

private:
    std::string parenthesize(const std::string& name, const std::vector<std::shared_ptr<Expr>>& exprs);

    // Expression visitors
    std::string visit(std::shared_ptr<BinaryExpr> expr) override;
    std::string visit(std::shared_ptr<LiteralExpr> expr) override;
    std::string visit(std::shared_ptr<VariableExpr> expr) override;
    std::string visit(std::shared_ptr<AssignExpr> expr) override;

    // Statement visitors
    std::string visit(std::shared_ptr<ExprStmt> stmt) override;
    std::string visit(std::shared_ptr<AnnounceStmt> stmt) override;
    std::string visit(std::shared_ptr<VarDeclStmt> stmt) override;
    std::string visit(std::shared_ptr<BlockStmt> stmt) override;
    std::string visit(std::shared_ptr<LoopStmt> stmt) override;
    std::string visit(std::shared_ptr<FinishlineStmt> stmt) override;
    std::string visit(std::shared_ptr<FuncDefStmt> stmt) override;
    std::string visit(std::shared_ptr<IfStmt> stmt) override;       // ✅ ADD
    std::string visit(std::shared_ptr<ListenStmt> stmt) override;   // ✅ ADD
};
