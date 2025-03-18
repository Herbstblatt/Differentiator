#pragma once

#include <stdexcept>

namespace expression {
class ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class UnexpectedEOF : public ParseError {
    using ParseError::ParseError;
};

class SyntaxError : public ParseError {
    using ParseError::ParseError;
};

class SymbolsUnspecified : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class ZeroDivision : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

}

