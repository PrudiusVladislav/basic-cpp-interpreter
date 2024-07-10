//
// Created by User on 7/10/2024.
//

#ifndef LEXER_H
#define LEXER_H
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>

enum TokenType {
    NUMBER,
    PLUS, MINUS, STAR, SLASH,
    LPAREN, RPAREN,
    LBRACE, RBRACE,
    IDENTIFIER,
    DEF,
    ASSIGN,
    COMMA,
    SEMICOLON,
    END
};

struct Token {
    TokenType type;
    std::string value;
};


class Lexer {
public:
    explicit Lexer(std::string input) : input(std::move(input)), position(0) {}
    Token getNextToken();
private:
    std::string input;
    size_t position;

    [[nodiscard]] char currentChar() const {
        return position < input.size() ? input[position] : '\0';
    }

    void advance() {
        position++;
    }

    void skipWhitespace() {
        while (std::isspace(currentChar())) {
            advance();
        }
    }

    Token number();
    Token identifier();
};

inline Token Lexer::number() {
    size_t start = position;
    while (std::isdigit(currentChar())) {
        advance();
    }
    return { NUMBER, input.substr(start, position - start) };
}

inline Token Lexer::identifier() {
    size_t start = position;
    while (std::isalnum(currentChar()) || currentChar() == '_') {
        advance();
    }
    return { IDENTIFIER, input.substr(start, position - start) };
}

inline Token Lexer::getNextToken() {
    skipWhitespace();

    char current = currentChar();
    if (std::isdigit(current)) {
        return number();
    }

    if (std::isalpha(current)) {
        const std::string text = identifier().value;
        if (text == "def") {
            return { DEF, text };
        }
        return { IDENTIFIER, text };
    }

    switch (current) {
        case '+': advance(); return { PLUS, "+" };
        case '-': advance(); return { MINUS, "-" };
        case '*': advance(); return { STAR, "*" };
        case '/': advance(); return { SLASH, "/" };
        case '(': advance(); return { LPAREN, "(" };
        case ')': advance(); return { RPAREN, ")" };
        case '=': advance(); return { ASSIGN, "=" };
        case ',': advance(); return { COMMA, "," };
        case ';': advance(); return { SEMICOLON, ";" };
        case '{': advance(); return { LBRACE, "{" };
        case '}': advance(); return { RBRACE, "}" };
        case '\0': return { END, "" };
        default: throw std::runtime_error("Unexpected character");
    }
}


#endif //LEXER_H
