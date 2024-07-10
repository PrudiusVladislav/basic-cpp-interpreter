#include <iostream>
#include <string>

#include "AbstractSyntaxTree.h"
#include "Lexer.h"
#include "Parser.h"
using namespace std;

int main() {
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

    string input = "def myfunc(a, b) { min(a, b) + max(a, b) }; myfunc(3, 4)";
    // TODO: user defined functions does not work
    Lexer lexer(input);
    Parser parser(lexer);
    ExpressionPtr tree = parser.parse();

    std::cout << "Result: " << tree->evaluate() << std::endl;
    // std::cout << "a: " << variables["a"] << std::endl;
}
