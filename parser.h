#pragma once

#include "scanner.h"
#include <vector>
#include <string>
#include <memory>

using std::vector;
using std::string;
using std::shared_ptr;

// Forward declare AST node types
struct BinaryExpr;
struct LiteralExpr;
struct VariableExpr;
struct AssignExpr;

struct AnnounceStmt;
struct VarDeclStmt;
struct BlockStmt;
struct LoopStmt;
struct FinishlineStmt;
struct FuncDefStmt;
struct ExprStmt;
struct IfStmt;
struct ListenStmt;

// ----------------------
// Visitor Interfaces
// ----------------------
template <typename R>
struct ExprVisitor {
    virtual ~ExprVisitor() = default;
    virtual R visit(shared_ptr<BinaryExpr> expr) = 0;
    virtual R visit(shared_ptr<LiteralExpr> expr) = 0;
    virtual R visit(shared_ptr<VariableExpr> expr) = 0;
    virtual R visit(shared_ptr<AssignExpr> expr) = 0;
};

template <typename R>
struct StmtVisitor {
    virtual ~StmtVisitor() = default;
    virtual R visit(shared_ptr<AnnounceStmt> stmt) = 0;
    virtual R visit(shared_ptr<VarDeclStmt> stmt) = 0;
    virtual R visit(shared_ptr<BlockStmt> stmt) = 0;
    virtual R visit(shared_ptr<LoopStmt> stmt) = 0;
    virtual R visit(shared_ptr<FinishlineStmt> stmt) = 0;
    virtual R visit(shared_ptr<FuncDefStmt> stmt) = 0;
    virtual R visit(shared_ptr<ExprStmt> stmt) = 0;
    virtual R visit(shared_ptr<IfStmt> stmt) = 0;
    virtual R visit(shared_ptr<ListenStmt> stmt) = 0;
};

// ----------------------
// Base AST classes
// ----------------------
struct Expr {
    virtual ~Expr() = default;
    virtual string accept(ExprVisitor<string>& visitor) = 0;
};

struct Stmt {
    virtual ~Stmt() = default;
    virtual string accept(StmtVisitor<string>& visitor) = 0;
};

// ----------------------
// Expression Nodes
// ----------------------
struct BinaryExpr : Expr, public std::enable_shared_from_this<BinaryExpr> {
    shared_ptr<Expr> left;
    Token op;
    shared_ptr<Expr> right;

    BinaryExpr(shared_ptr<Expr> l, Token o, shared_ptr<Expr> r)
        : left(l), op(o), right(r) {
    }
    string accept(ExprVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct LiteralExpr : Expr, public std::enable_shared_from_this<LiteralExpr> {
    Token value;
    LiteralExpr(Token v) : value(v) {}
    string accept(ExprVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct VariableExpr : Expr, public std::enable_shared_from_this<VariableExpr> {
    Token name;
    VariableExpr(Token n) : name(n) {}
    string accept(ExprVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct AssignExpr : Expr, public std::enable_shared_from_this<AssignExpr> {
    Token name;
    shared_ptr<Expr> value;
    AssignExpr(Token n, shared_ptr<Expr> v) : name(n), value(v) {}
    string accept(ExprVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

// ----------------------
// Statement Nodes
// ----------------------
struct ExprStmt : Stmt, public std::enable_shared_from_this<ExprStmt> {
    shared_ptr<Expr> expression;
    ExprStmt(shared_ptr<Expr> e) : expression(e) {}
    string accept(StmtVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct AnnounceStmt : Stmt, public std::enable_shared_from_this<AnnounceStmt> {
    shared_ptr<Expr> expression;
    AnnounceStmt(shared_ptr<Expr> e) : expression(e) {}
    string accept(StmtVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct VarDeclStmt : Stmt, public std::enable_shared_from_this<VarDeclStmt> {
    Token typeToken;
    Token name;
    shared_ptr<Expr> initializer;
    VarDeclStmt(Token t, Token n, shared_ptr<Expr> init)
        : typeToken(t), name(n), initializer(init) {
    }
    string accept(StmtVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct BlockStmt : Stmt, public std::enable_shared_from_this<BlockStmt> {
    vector<shared_ptr<Stmt>> statements;
    BlockStmt(vector<shared_ptr<Stmt>> s) : statements(s) {}
    string accept(StmtVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct LoopStmt : Stmt, public std::enable_shared_from_this<LoopStmt> {
    shared_ptr<Expr> condition;
    shared_ptr<Stmt> body;
    LoopStmt(shared_ptr<Expr> c, shared_ptr<Stmt> b) : condition(c), body(b) {}
    string accept(StmtVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct FinishlineStmt : Stmt, public std::enable_shared_from_this<FinishlineStmt> {
    shared_ptr<Expr> value;
    FinishlineStmt(shared_ptr<Expr> v) : value(v) {}
    string accept(StmtVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct FuncDefStmt : Stmt, public std::enable_shared_from_this<FuncDefStmt> {
    Token name;
    shared_ptr<Stmt> body;
    FuncDefStmt(Token n, shared_ptr<Stmt> b) : name(n), body(b) {}
    string accept(StmtVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct IfStmt : Stmt, public std::enable_shared_from_this<IfStmt> {
    shared_ptr<Expr> condition;
    shared_ptr<Stmt> thenBranch;
    shared_ptr<Stmt> elseBranch;
    IfStmt(shared_ptr<Expr> c, shared_ptr<Stmt> t, shared_ptr<Stmt> e)
        : condition(c), thenBranch(t), elseBranch(e) {
    }
    string accept(StmtVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

struct ListenStmt : Stmt, public std::enable_shared_from_this<ListenStmt> {
    Token name;
    ListenStmt(Token n) : name(n) {}
    string accept(StmtVisitor<string>& visitor) override {
        return visitor.visit(shared_from_this());
    }
};

// ----------------------
// Parser Class
// ----------------------
class Parser {
public:
    Parser(const vector<Token>& tokens);
    vector<shared_ptr<Stmt>> parse();

private:
    vector<Token> tokens;
    int current = 0;

    shared_ptr<Stmt> parseStatement();
    shared_ptr<Stmt> parseFuncDef();
    shared_ptr<Stmt> parseIgniteFunc(); // add if your .cpp defines it
    shared_ptr<Stmt> parseVarDecl();
    shared_ptr<Stmt> parseLoopStmt();
    shared_ptr<Stmt> parseAnnounceStmt();
    shared_ptr<Stmt> parseFinishlineStmt();
    shared_ptr<Stmt> parseExprStatement();
    shared_ptr<Stmt> parseBlock();
    shared_ptr<Stmt> parseIfStmt();
    shared_ptr<Stmt> parseListenStmt();

    shared_ptr<Expr> parseExpression();
    shared_ptr<Expr> parseAssignment();
    shared_ptr<Expr> parseComparison();
    shared_ptr<Expr> parseTerm();
    shared_ptr<Expr> parseFactor();
    shared_ptr<Expr> parsePrimary();

    bool isAtEnd();
    Token peek();    // Implementations should guard against out-of-range
    Token previous(); // Implementations should guard against current == 0
    Token advance();
    bool check(TokenType type);
    bool match(const vector<TokenType>& types);
    Token consume(TokenType type, const string& message);
    void synchronize();
};
