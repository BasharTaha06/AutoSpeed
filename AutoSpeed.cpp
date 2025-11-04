#include <iostream>
#include <vector>
#include "scanner.h"
#include "parser.h"
#include "ast_printer.h"

int main() {

    // ===== All Test Programs =====
    std::vector<std::string> tests = {

        // TEST 1
        R"(ignite() {
            announce "Start!";
            finishline 0;
        })",

        // TEST 2
        R"(ignite() {
            looplap (5 > 0) {
                announce "Lap!";
            }
            finishline 0;
        })",

        // TEST 3
        R"(ignite() {
            looplap (3 > 1) {
                announce "Lap 1!";
                announce "Lap 2!";
            }
            finishline 0;
        })",

        // TEST 4
        R"(ignite() {
            gear fuel = 100;
            fuel = fuel - 10;
            announce fuel;
            finishline 0;
        })",

        // TEST 5
        R"(ignite() {
            gear fuel = 20;
            track (fuel < 30) {
                announce "Low fuel!";
            }
            finishline 0;
        })",

        // TEST 6
        R"(ignite() {
            gear fuel = 20;
            track (fuel < 30) {
                announce "Low fuel!";
            }
            pitstop {
                announce "Refueling...";
            }
            finishline 0;
        })",

        // TEST 7
        R"(engine boost() {
            announce "Boosting!";
        }

        ignite() {
            announce "Race start!";
            finishline 0;
        })",

        // TEST 8
        R"(ignite() {
            announce "Start";
            {
                announce "Inside nested";
            }
            finishline 0;
        })",

        // TEST 9 — invalid (should FAIL)
        R"(ignite() {
            looplap (5 > ) {
                announce "Bad code";
            }
            finishline 0;
        })"
    };

    int test_number = 1;

    // ===== Run all tests =====
    for (auto& code : tests) {
        std::cout << "\n=====================================\n";
        std::cout << "TEST #" << test_number++ << "\n";
        std::cout << "=====================================\n";

        try {
            // ===== Scanner =====
            auto tokens = scan(code);
            std::cout << "TOKENS:\n";
            for (auto& t : tokens) {
                std::cout << "[" << t.line << "] "
                    << tokenTypeToString(t.type)
                    << " : " << t.value << "\n";
            }

            // ===== Parser =====
            Parser parser(tokens);
            auto stmts = parser.parse();
            std::cout << "\n PARSE SUCCESS — " << stmts.size() << " statement(s)\n";

            // ===== AST Printer =====
            AstPrinter printer;
            std::cout << "\nAST:\n" << printer.print(stmts) << "\n";
        }
        catch (std::exception& e) {
            std::cout << "\n PARSE FAILED: " << e.what() << "\n";
        }
    }

    return 0;
}
