#include <iostream>
#include <string>
#include <cmath>

#include "AbstractSyntaxTree.h"
#include "Lexer.h"
#include "Parser.h"
using namespace std;

int main() {
    try {
        // built-ins
        functions["max"] = [](const std::vector<double> &args) {
            if (args.size() != 2) {
                throw std::runtime_error("max() function expects 2 arguments");
            }
            return std::max(args[0], args[1]);
        };

        functions["min"] = [](const std::vector<double> &args) {
            if (args.size() != 2) {
                throw std::runtime_error("min() function expects 2 arguments");
            }
            return std::min(args[0], args[1]);
        };

        functions["pow"] = [](const std::vector<double> &args) {
            if (args.size() != 2) {
                throw std::runtime_error("pow() function expects 2 arguments");
            }
            return std::pow(args[0], args[1]);
        };

        functions["abs"] = [](const std::vector<double> &args) {
            if (args.size() != 1) {
                throw std::runtime_error("abs() function expects 1 argument");
            }
            return std::abs(args[0]);
        };

        std::string input;
        while (std::getline(std::cin, input)) {
            Lexer lexer(input);
            Parser parser(lexer);
            ExpressionPtr tree = parser.parse();

            double result = tree->evaluate();
            std::cout << result << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
};
