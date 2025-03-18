#include "../include/Expression.hpp"
#include <complex>
#include <iostream>

int main() {
  expression::Expression<std::complex<double>> expr(std::complex<double>(0.0, 1.0));
  std::cout << (expression::log(expr) / expression::Expression(std::complex(0.0, 0.0))).evaluate() << std::endl;
}