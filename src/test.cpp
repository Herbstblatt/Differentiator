#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <complex>
#include "Expression.hpp"

using namespace expression;

TEST_CASE("Construction from string") {
    Expression<double> expr("2 * x + 5");
    CHECK(expr.evaluate({{"x", 3}}) == 11);
}

TEST_CASE("Substitution") {
    Expression<double> expr("2 * x + y");
    expr.substitute("y", 4);
    CHECK(expr.evaluate({{"x", 3}}) == 10);
}

TEST_CASE("Arithmetic operations with real numbers") {
    Expression<double> expr1("2 * x + 5");
    Expression<double> expr2("3 * x - 1");

    auto sum = expr1 + expr2;
    auto diff = expr1 - expr2;
    auto prod = expr1 * expr2;
    auto div = expr1 / expr2;

    CHECK(sum.evaluate({{"x", 2}}) == 14);
    CHECK(diff.evaluate({{"x", 2}}) == 4);
    CHECK(prod.evaluate({{"x", 2}}) == 45);
    CHECK(div.evaluate({{"x", 2}}) == doctest::Approx(1.8));
}

TEST_CASE("Arithmetic operations with complex numbers") {
    using namespace std::complex_literals;
    Expression<std::complex<double>> expr1("2 * x + (1 + 1*i)");
    Expression<std::complex<double>> expr2("3 * x - (2 - 1*i)");
    expr1.substitute("i", std::complex<double>(0.0, 1.0));
    expr2.substitute("i", std::complex<double>(0.0, 1.0));

    auto sum = expr1 + expr2;
    auto diff = expr1 - expr2;
    auto prod = expr1 * expr2;
    auto div = expr1 / expr2;

    std::complex<double> x_val = 2.0 + 1i;
    std::unordered_map<std::string, std::complex<double>> symbols = {{"x", x_val}};

    CHECK(sum.evaluate(symbols) == std::complex<double>(9, 7));
    CHECK(diff.evaluate(symbols) == std::complex<double>(1, -1));
    CHECK(prod.evaluate(symbols) == std::complex<double>(8, 32));
    CHECK(div.evaluate(symbols) == std::complex<double>(1, -0.25));
}

TEST_CASE("Differentiation") {
    Expression<double> expr("x^2 + 3*x + 5");
    auto derivative = expr.differentiate("x");

    CHECK(derivative.evaluate({{"x", 2}}) == 7);
}

TEST_CASE("Trigonometric and exponential functions") {
    Expression<double> expr("sin(x) + cos(x) + exp(x)");
    auto result = expr.evaluate({{"x", 0}});

    CHECK(result == doctest::Approx(2.0));
}

TEST_CASE("Complex logarithm") {
    using namespace std::complex_literals;
    Expression<std::complex<double>> expr("ln(x)");
    std::complex<double> x_val = 1.0 + 1i;
    std::unordered_map<std::string, std::complex<double>> symbols = {{"x", x_val}};

    auto result = expr.evaluate(symbols);
    std::complex<double> expected = std::log(x_val);

    CHECK(result.real() == doctest::Approx(expected.real()));
    CHECK(result.imag() == doctest::Approx(expected.imag()));
}