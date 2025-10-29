#include <iostream>

#include <string>

#include <vector>

#include <unordered_set>

#include <cctype>

using namespace std;



enum TokenType {

    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR, SYMBOL, BOOLEAN, END_OF_FILE

};



struct Token {

    TokenType type;

    string value;

};



unordered_set<string> keywords = {

    "ignite", "engine",

    "gear", "turbo", "exhaust", "flag",

    "announce", "listen",

    "track", "pitstop", "looplap", "overtake", "finishline",

    "key", "#oil", "#car"

};



unordered_set<string> booleans = { "true", "false" };

unordered_set<char> operators = { '+', '-', '*', '/', '=', '<', '>' };

unordered_set<char> symbols = { '{', '}', '(', ')', ';', '#' };



vector<Token> scan(const string& code) {

    vector<Token> tok;

    int i = 0;

    while (i < code.size()) {

        if (isspace(code[i])) { i++; continue; }



        // String

        if (code[i] == '"') {

            string str; i++;

            while (i < code.size() && code[i] != '"')

                str += code[i++];

            i++; // skip closing "

            tok.push_back({ STRING, str });

        }

        // Identifier or Keyword

        else if (isalpha(code[i]) || code[i] == '#') {

            string word;

            while (isalnum(code[i]) || code[i] == '#' || code[i] == '_')

                word += code[i++];

            if (keywords.count(word))

                tok.push_back({ KEYWORD, word });

            else if (booleans.count(word))

                tok.push_back({ BOOLEAN, word });

            else

                tok.push_back({ IDENTIFIER, word });

        }

        // Number

        else if (isdigit(code[i])) {

            string num;

            while (isdigit(code[i]) || code[i] == '.')

                num += code[i++];

            tok.push_back({ NUMBER, num });

        }

        // Operator

        else if (operators.count(code[i])) {

            string op(1, code[i++]);

            tok.push_back({ OPERATOR, op });

        }

        // Symbol

        else if (symbols.count(code[i])) {

            string sym(1, code[i++]);

            tok.push_back({ SYMBOL, sym });

        }

        else {

            cout << "Unknown character: " << code[i] << endl;

            i++;

        }

    }

    tok.push_back({ END_OF_FILE, "EOF" });

    return tok;

}



int main() {

    string code = R"(

        ignite() {

            announce " Welcome to Auto-Speed!";

            gear fuel = 95;

            looplap (fuel > 0) {

                announce "Lap!";

                fuel = fuel - 10;

            }

            finishline 0;

        }

    )";



    vector<Token> tokens = scan(code);

    for (auto& t : tokens) {

        cout << "Token(" << t.value << ", " << t.type << ")" << endl;

    }

    return 0;

}