#pragma once

#include <string>
#include "AST/Node.hpp"
#include "exceptions.h"

namespace expression {

template<typename T>
class Parser {
public:
	explicit Parser(std::string_view string);
	std::unique_ptr<INode<T>> parse();

private:
	bool eof() const;
        char next();
        char peek() const;
        void expect(char expected);
        void skip_ws();

	std::unique_ptr<INode<T>> parse_number();
    std::string parse_word();
    std::unique_ptr<INode<T>> parse_term();
    std::unique_ptr<INode<T>> parse_expression();
    std::unique_ptr<INode<T>> parse_factor();
    std::unique_ptr<INode<T>> parse_power();

	std::string m_string;
	size_t m_pos = 0;
};

}

#include "../lib/Parser.cpp"