//
// Created by User on 7/10/2024.
//

#ifndef ABSTRACTSYNTAXTREE_H
#define ABSTRACTSYNTAXTREE_H
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

class FunctionDefExpression;
class Expression;
using ExpressionPtr = std::shared_ptr<Expression>;

class Expression {
public:
    virtual ~Expression() = default;
    [[nodiscard]] virtual double evaluate() = 0;
};

class Number final : public Expression {
    double value;
public:
    explicit Number(double value) : value(value) {}
    double evaluate() override {
        return value;
    }
};

class BinaryExpression final : public Expression {
    char op;
    ExpressionPtr left;
    ExpressionPtr right;
public:
    BinaryExpression(char op, ExpressionPtr left, ExpressionPtr right)
        :op(op), left(std::move(left)), right(std::move(right)) {}

    double evaluate() override {
        switch (op) {
            case '+': return left->evaluate() + right->evaluate();
            case '-': return left->evaluate() - right->evaluate();
            case '*': return left->evaluate() * right->evaluate();
            case '/': return left->evaluate() / right->evaluate();
            default: throw std::runtime_error("Unknown operator");
        }
    }
};

using FunctionDef = std::function<double(const std::vector<double>&)>;
inline std::map<std::string, FunctionDef> functions;
inline std::map<std::string, std::shared_ptr<FunctionDefExpression>> userFunctions;
inline std::map<std::string, double> variables;

class FunctionDefExpression final : public Expression {
public:
    std::string name;
    std::vector<std::string> params;
    ExpressionPtr body;

    FunctionDefExpression(std::string name, std::vector<std::string> params, ExpressionPtr body)
        : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}

    double evaluate() override { return 0; }
};

class MethodCallExpression final : public Expression {
    std::string name;
    std::vector<ExpressionPtr> args;
public:
    MethodCallExpression(std::string name, std::vector<ExpressionPtr> args) : name(std::move(name)), args(std::move(args)) {}
    double evaluate() override {
        auto it = userFunctions.find(name);
        if (it != userFunctions.end()) {
            const auto& funcDef = it->second;
            std::map<std::string, double> oldVariables = variables;

            for (size_t i = 0; i < funcDef->params.size(); ++i) {
                variables[funcDef->params[i]] = args[i]->evaluate();
            }

            double result = funcDef->body->evaluate();
            variables = oldVariables;
            return result;
        }

        std::vector<double> evaluatedArgs;
        for (auto& arg : args) {
            evaluatedArgs.push_back(arg->evaluate());
        }
        return functions[name](evaluatedArgs);
    }
};

class VariableExpression final : public Expression {
    std::string name;
public:
    explicit VariableExpression(std::string name) : name(std::move(name)) {}
    double evaluate() override {
        return variables[name];
    }
};

class AssignmentExpression final : public Expression {
    std::string name;
    ExpressionPtr value;
public:
    AssignmentExpression(std::string name, ExpressionPtr value) : name(std::move(name)), value(std::move(value)) {}
    double evaluate() override {
        variables[name] = value->evaluate();
        return variables[name];
    }
};

class StatementListExpression final : public Expression {
public:
    std::vector<ExpressionPtr> statements;
    void addStatement(ExpressionPtr stmt) {
        statements.push_back(std::move(stmt));
    }
    double evaluate() override {
        double result = 0;
        for (const auto& stmt : statements) {
            result = stmt->evaluate();
        }
        return result;
    }
};


#endif //ABSTRACTSYNTAXTREE_H
