#include "AST/LiteralNode.hpp"

#include <AST/binary_operators/AddNode.hpp>
#include <AST/binary_operators/DivideNode.hpp>
#include <AST/binary_operators/MultiplyNode.hpp>
#include <AST/binary_operators/PowerNode.hpp>
#include <AST/binary_operators/SubstractNode.hpp>
#include <AST/unary_operators/CosNode.hpp>
#include <AST/unary_operators/ExpNode.hpp>
#include <AST/unary_operators/LogNode.hpp>
#include <AST/unary_operators/SinNode.hpp>

namespace expression {

template <typename T>
Parser<T>::Parser(const std::string_view string)
    : m_string { std::move(string) }, m_pos { 0 }
    {}

template <typename T>
bool Parser<T>::eof() const {
    return m_pos >= m_string.size();
}

template <typename T>
char Parser<T>::next()  {
    if (!eof()) {
        return m_string[m_pos++];
    }
    throw expression::UnexpectedEOF("Unexpected EOF at " + std::to_string(m_pos));
}

template <typename T>
char Parser<T>::peek() const {
    if (!eof()) {
        return m_string[m_pos];
    }
    throw expression::UnexpectedEOF("Unexpected EOF at " + std::to_string(m_pos));
}

template <typename T>
void Parser<T>::skip_ws()  {
    while (!eof() && isspace(peek())) {
        next();
    }
}

template <typename T>
void Parser<T>::expect(const char expected) {
    if (next() != expected) {
        throw expression::SyntaxError("Expected '" + std::string(1, expected) + "' at " + std::to_string(m_pos));
    }
}

template<typename T>
std::unique_ptr<INode<T>> Parser<T>::parse_number() {
    T number = 0;
    while (!eof() && isdigit(peek())) {
        number = number * T(10) + T(next() - '0');
    }
    auto node = std::make_unique<LiteralNode<T>>(number);
    return node;
}


template <typename T>
std::string Parser<T>::parse_word() {
    std::string word{};
    while (!eof() && isalpha(peek())) {
        word.push_back(next());
    }
    return word;
}

template <typename T>
std::unique_ptr<INode<T>> Parser<T>::parse_term() {
    std::unique_ptr<INode<T>> expr = parse_expression();
    while (!eof()) {
        skip_ws();
        char curr = peek();
        if (curr == '+') {
            next();
            std::unique_ptr<INode<T>> expr2 = parse_expression();
            auto new_expr = std::make_unique<AddNode<T>>(std::move(expr), std::move(expr2));
            expr = std::move(new_expr);
        }
        else if (curr == '-') {
            next();
            std::unique_ptr<INode<T>> expr2 = parse_expression();
            auto new_expr = std::make_unique<SubtractNode<T>>(std::move(expr), std::move(expr2));
            expr = std::move(new_expr);
        } else break;
    }
    return expr;
}

template <typename T>
std::unique_ptr<INode<T>> Parser<T>::parse_expression() {
    std::unique_ptr<INode<T>> power = parse_power();
    while (!eof()) {
        skip_ws();
        char curr = peek();
        if (curr == '*') {
            next();
            std::unique_ptr<INode<T>> power2 = parse_power();
            auto new_power = std::make_unique<MultiplyNode<T>>(std::move(power), std::move(power2));
            power = std::move(new_power);
        }
        else if (curr == '/') {
            next();
            std::unique_ptr<INode<T>> power2 = parse_power();
            auto new_power = std::make_unique<DivideNode<T>>(std::move(power), std::move(power2));
            power = std::move(new_power);
        } else break;
    }
    return power;
}

template <typename T>
std::unique_ptr<INode<T>> Parser<T>::parse_power() {
    std::unique_ptr<INode<T>> factor = parse_factor();
    while (!eof()) {
        skip_ws();
        if (peek() == '^') {
            next();
            std::unique_ptr<INode<T>> factor2 = parse_factor();
            auto new_factor = std::make_unique<PowerNode<T>>(std::move(factor), std::move(factor2));
            factor = std::move(new_factor);
        } else break;
    }
    return factor;
}

template <typename T>
std::unique_ptr<INode<T>> Parser<T>::parse_factor() {
    skip_ws();
    char nxt = peek();
    if (nxt == '(') {
        next();
        skip_ws();
        std::unique_ptr<INode<T>> term = parse_term();
        expect(')');
        return term;
    }
    else if (nxt == '-') {
        next();
        skip_ws();
        std::unique_ptr<INode<T>> term = parse_term();
        auto minus = std::make_unique<MinusNode<T>>(std::move(term));
        return minus;
    }
    else if (isdigit(nxt)) {
        return parse_number();
    }
    else {
        std::string curr_word = parse_word();
        if (curr_word == "sin" || curr_word == "cos" || curr_word == "ln" || curr_word == "exp") {
            skip_ws();
            expect('(');
            std::unique_ptr<INode<T>> arg = parse_term();
            expect(')');
            if (curr_word == "sin") {
                return std::make_unique<SinNode<T>>(std::move(arg));
            } else if (curr_word == "cos") {
                return std::make_unique<CosNode<T>>(std::move(arg));
            } else if (curr_word == "ln") {
                return std::make_unique<LogNode<T>>(std::move(arg));
            } else if (curr_word == "exp") {
                return std::make_unique<ExpNode<T>>(std::move(arg));
            }
        }
        return std::make_unique<VariableNode<T>>(std::move(curr_word));
    }
}

template<typename T>
std::unique_ptr<INode<T>> Parser<T>::parse() {
    std::unique_ptr<INode<T>> tree = parse_term();
    skip_ws();
    if (!eof()) {
        throw SyntaxError("Expected EOF, found '" + std::string{peek()} + "' at " + std::to_string(m_pos));
    }
    return tree;
}

}