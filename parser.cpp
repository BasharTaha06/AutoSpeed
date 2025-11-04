#include "parser.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Parser::Parser(const vector<Token>& tokens) : tokens(tokens) {}

vector<shared_ptr<Stmt>> Parser::parse() {
    vector<shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        try {
            statements.push_back(parseStatement());
        }
        catch (runtime_error& e) {
            cerr << e.what() << endl;
            synchronize();
        }
    }
    return statements;
}

shared_ptr<Stmt> Parser::parseStatement() {
    Token p = peek();

    if (p.type == KEYWORD && p.value == "engine") {
        advance();
        return parseFuncDef();
    }
    if (p.type == KEYWORD && p.value == "ignite") {
        advance();
        return parseIgniteFunc();
    }
    if (p.type == KEYWORD && (p.value == "gear" || p.value == "turbo" || p.value == "exhaust" || p.value == "flag")) {
        advance();
        return parseVarDecl();
    }
    if (p.type == KEYWORD && p.value == "looplap") {
        advance();
        return parseLoopStmt();
    }
    if (p.type == KEYWORD && p.value == "announce") {
        advance();
        return parseAnnounceStmt();
    }
    if (p.type == KEYWORD && p.value == "finishline") {
        advance();
        return parseFinishlineStmt();
    }
    if (p.type == KEYWORD && p.value == "track") {
        advance();
        return parseIfStmt();
    }
    if (p.type == KEYWORD && p.value == "listen") {
        advance();
        return parseListenStmt();
    }
    if (check(SYMBOL) && peek().value == "{") {
        return parseBlock();
    }
    return parseExprStatement();
}

shared_ptr<Stmt> Parser::parseFuncDef() {
    Token name = consume(IDENTIFIER, "Expect function name after 'engine'.");
    Token open = consume(SYMBOL, "Expect '(' after function name.");
    if (open.value != "(") throw runtime_error("Expect '(' after function name.");
    Token close = consume(SYMBOL, "Expect ')' after function name.");
    if (close.value != ")") throw runtime_error("Expect ')' after function name.");
    auto body = parseBlock();
    return make_shared<FuncDefStmt>(name, body);
}

shared_ptr<Stmt> Parser::parseIgniteFunc() {
    // ignite() has no arguments, and its name is literally "ignite"
    Token open = consume(SYMBOL, "Expect '(' after 'ignite'.");
    if (open.value != "(") throw runtime_error("Expect '(' after 'ignite'.");
    Token close = consume(SYMBOL, "Expect ')' after 'ignite'.");
    if (close.value != ")") throw runtime_error("Expect ')' after 'ignite'.");

    // Construct a fake identifier token named "ignite"
    Token igniteName;
    igniteName.type = IDENTIFIER;
    igniteName.value = "ignite";
    igniteName.line = open.line;

    auto body = parseBlock();
    return make_shared<FuncDefStmt>(igniteName, body);
}

shared_ptr<Stmt> Parser::parseVarDecl() {
    Token typeToken = previous();
    Token name = consume(IDENTIFIER, "Expect variable name.");
    shared_ptr<Expr> initializer = nullptr;

    if (check(OPERATOR) && peek().value == "=") {
        advance();
        initializer = parseExpression();
    }

    Token semi = consume(SYMBOL, "Expect ';' after variable.");
    if (semi.value != ";") throw runtime_error("Expect ';' after variable.");
    return make_shared<VarDeclStmt>(typeToken, name, initializer);
}

shared_ptr<Stmt> Parser::parseLoopStmt() {
    Token open = consume(SYMBOL, "Expect '(' after 'looplap'.");
    if (open.value != "(") throw runtime_error("Expect '(' after 'looplap'.");
    auto condition = parseExpression();
    Token close = consume(SYMBOL, "Expect ')' after condition.");
    if (close.value != ")") throw runtime_error("Expect ')' after condition.");
    auto body = parseStatement();
    return make_shared<LoopStmt>(condition, body);
}

shared_ptr<Stmt> Parser::parseAnnounceStmt() {
    auto value = parseExpression();
    Token semi = consume(SYMBOL, "Expect ';' after announce.");
    if (semi.value != ";") throw runtime_error("Expect ';' after announce.");
    return make_shared<AnnounceStmt>(value);
}

shared_ptr<Stmt> Parser::parseFinishlineStmt() {
    auto value = parseExpression();
    Token semi = consume(SYMBOL, "Expect ';' after finishline.");
    if (semi.value != ";") throw runtime_error("Expect ';' after finishline.");
    return make_shared<FinishlineStmt>(value);
}

shared_ptr<Stmt> Parser::parseListenStmt() {
    Token name = consume(IDENTIFIER, "Expect variable name after 'listen'.");
    Token semi = consume(SYMBOL, "Expect ';' after listen.");
    if (semi.value != ";") throw runtime_error("Expect ';' after listen.");
    return make_shared<ListenStmt>(name);
}

shared_ptr<Stmt> Parser::parseIfStmt() {
    Token open = consume(SYMBOL, "Expect '(' after 'track'.");
    if (open.value != "(") throw runtime_error("Expect '(' after 'track'.");
    auto condition = parseExpression();
    Token close = consume(SYMBOL, "Expect ')' after condition.");
    if (close.value != ")") throw runtime_error("Expect ')' after condition.");
    auto thenBranch = parseStatement();

    shared_ptr<Stmt> elseBranch = nullptr;
    if (peek().type == KEYWORD && peek().value == "pitstop") {
        advance();
        elseBranch = parseStatement();
    }
    return make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

shared_ptr<Stmt> Parser::parseBlock() {
    Token open = consume(SYMBOL, "Expect '{' to start block.");
    if (open.value != "{") throw runtime_error("Expect '{' to start block.");

    vector<shared_ptr<Stmt>> statements;
    while (!(check(SYMBOL) && peek().value == "}")) {
        if (isAtEnd()) throw runtime_error("Unterminated block. Missing '}'.");
        statements.push_back(parseStatement());
    }

    Token close = consume(SYMBOL, "Expect '}' after block.");
    if (close.value != "}") throw runtime_error("Expect '}' after block.");
    return make_shared<BlockStmt>(statements);
}

shared_ptr<Stmt> Parser::parseExprStatement() {
    auto expr = parseExpression();
    Token semi = consume(SYMBOL, "Expect ';' after expression.");
    if (semi.value != ";") throw runtime_error("Expect ';' after expression.");
    return make_shared<ExprStmt>(expr);
}

/////////////////////// EXPRESSIONS ///////////////////////

shared_ptr<Expr> Parser::parseExpression() {
    return parseAssignment();
}

shared_ptr<Expr> Parser::parseAssignment() {
    auto expr = parseComparison();

    if (check(OPERATOR) && peek().value == "=") {
        advance(); // consume '='
        auto value = parseAssignment();
        auto var = dynamic_pointer_cast<VariableExpr>(expr);
        if (var) return make_shared<AssignExpr>(var->name, value);
        throw runtime_error("Invalid assignment target.");
    }
    return expr;
}

shared_ptr<Expr> Parser::parseComparison() {
    auto expr = parseTerm();

    while (check(OPERATOR)) {
        string v = peek().value;
        if (v == "<" || v == ">" || v == "<=" || v == ">=") {
            Token op = advance();
            auto right = parseTerm();
            expr = make_shared<BinaryExpr>(expr, op, right);
        }
        else break;
    }
    return expr;
}

shared_ptr<Expr> Parser::parseTerm() {
    auto expr = parseFactor();

    while (check(OPERATOR) && (peek().value == "+" || peek().value == "-")) {
        Token op = advance();
        auto right = parseFactor();
        expr = make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::parseFactor() {
    auto expr = parsePrimary();

    while (check(OPERATOR) && (peek().value == "*" || peek().value == "/")) {
        Token op = advance();
        auto right = parsePrimary();
        expr = make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

shared_ptr<Expr> Parser::parsePrimary() {
    if (match({ NUMBER, STRING, BOOLEAN })) {
        return make_shared<LiteralExpr>(previous());
    }
    if (match({ IDENTIFIER })) {
        return make_shared<VariableExpr>(previous());
    }
    if (match({ SYMBOL }) && previous().value == "(") {
        auto expr = parseExpression();
        Token close = consume(SYMBOL, "Expect ')'.");
        if (close.value != ")") throw runtime_error("Expect ')'.");
        return expr;
    }
    throw runtime_error("Expect expression.");
}

/////////////////// HELPERS ///////////////////

bool Parser::isAtEnd() {
    return peek().type == END_OF_FILE;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(const vector<TokenType>& types) {
    for (auto t : types) {
        if (check(t)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, const string& message) {
    if (check(type)) return advance();
    throw runtime_error(message + " At line: " + to_string(peek().line));
}

void Parser::synchronize() {
    advance();
    while (!isAtEnd()) {
        if (previous().type == SYMBOL && previous().value == ";") return;

        if (peek().type == KEYWORD) {
            if (peek().value == "engine" || peek().value == "ignite" || peek().value == "gear" ||
                peek().value == "looplap" || peek().value == "finishline")
                return;
        }
        advance();
    }
}
