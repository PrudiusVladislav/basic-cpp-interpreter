//
// Created by User on 7/10/2024.
//

#ifndef PARSER_H
#define PARSER_H
#include <memory>
#include <vector>

#include "AbstractSyntaxTree.h"
#include "Lexer.h"


class Parser {
public:
    explicit Parser(Lexer lexer) : lexer(std::move(lexer)) {
        advance();
    }
    ExpressionPtr parse();
private:
    Lexer lexer;
    Token currentToken;

    void advance() {
        currentToken = lexer.getNextToken();
    }

    ExpressionPtr expression();
    ExpressionPtr term();
    ExpressionPtr factor();
    ExpressionPtr functionDef();
};

inline ExpressionPtr Parser::parse() {
    auto root = std::make_shared<StatementListExpression>();
    while (currentToken.type != END) {
        if (currentToken.type == DEF) {
            root->addStatement(functionDef());
        } else {
            root->addStatement(expression());
        }
        if (currentToken.type == SEMICOLON) {
            advance();
        }
    }
    return root;
}

inline ExpressionPtr Parser::expression() {
    ExpressionPtr node = term();

    while (currentToken.type == PLUS || currentToken.type == MINUS) {
        char op = currentToken.value[0];
        advance();
        node = std::make_shared<BinaryExpression>(op, node, term());
    }

    return node;
}

inline ExpressionPtr Parser::term() {
    ExpressionPtr node = factor();

    while (currentToken.type == STAR || currentToken.type == SLASH) {
        char op = currentToken.value[0];
        advance();
        node = std::make_shared<BinaryExpression>(op, node, factor());
    }

    return node;
}

inline ExpressionPtr Parser::factor() {
    if (currentToken.type == NUMBER) {
        double value = std::stod(currentToken.value);
        advance();
        return std::make_shared<Number>(value);
    }

    if (currentToken.type == IDENTIFIER) {
        std::string name = currentToken.value;
        advance();

        if (currentToken.type == LPAREN) {
            advance();
            std::vector<ExpressionPtr> args;

            if (currentToken.type != RPAREN) {
                do {
                    args.push_back(expression());
                    if (currentToken.type == COMMA) {
                        advance();
                    }
                } while (currentToken.type != RPAREN);
            }

            advance();
            return std::make_shared<MethodCallExpression>(name, args);
        }

        if (currentToken.type == ASSIGN) {
            advance();
            return std::make_shared<AssignmentExpression>(name, expression());
        }
        return std::make_shared<VariableExpression>(name);
    }

    if (currentToken.type == LPAREN) {
        advance();
        ExpressionPtr node = expression();
        advance();
        return node;
    }

    throw std::runtime_error("Unexpected token");
}

inline ExpressionPtr Parser::functionDef() {
    advance(); // 'def'
    std::string name = currentToken.value;
    advance(); // name

    std::vector<std::string> params;
    advance(); // lparen
    while (currentToken.type != RPAREN) {
        params.push_back(currentToken.value);
        advance();
        if (currentToken.type == COMMA) {
            advance();
        }
    }
    advance(); // rparen

    advance(); // '{'
    ExpressionPtr body = expression();
    advance(); // '}'

    auto funcDef = std::make_shared<FunctionDefExpression>(name, params, body);
    userFunctions[name] = funcDef;
    return funcDef;
}




#endif //PARSER_H
