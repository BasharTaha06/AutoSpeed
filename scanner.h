#pragma once

#include <string>
#include <vector>

// --- Public Interface ---
// All the parts another file needs to know about.

/*
 * TokenType
 * Defines all the possible categories a token can belong to.
 */
enum TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    STRING,
    OPERATOR,
    SYMBOL,
    BOOLEAN,
    END_OF_FILE,
    UNKNOWN // Added for any character that doesn't match
};

/*
 * Token
 * A struct to hold the data for a single token.
 */
struct Token {
    TokenType type;
    std::string value;
    int line; // The line number where the token was found
};

/*
 * scan
 * The main scanner function.
 * Takes raw code as a string and returns a vector of Tokens.
 */
std::vector<Token> scan(const std::string& code);


/*
 * tokenTypeToString
 * A helper function to get a printable name for a TokenType.
 */
std::string tokenTypeToString(TokenType type);

