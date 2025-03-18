#include "Expression.hpp"
#include "tclap/CmdLine.h"

#include <complex>
#include <iostream>
#include <sstream>


int main(int argc, char** argv) {
    TCLAP::CmdLine cmd("A simple calculator with differentiation support", ' ', "0.1");

    TCLAP::ValueArg<std::string> eval_arg("","eval","An expression to evaluate",false,"","expr");
    TCLAP::ValueArg<std::string> diff_arg("","diff","An expression to differentiate",false,"","expr");
    TCLAP::ValueArg<std::string> by_arg("","by","A variable to differentiate by",false,"","variable");
    TCLAP::SwitchArg complex_arg("c", "complex", "Whether to allow complex numbers", false);

    TCLAP::OneOf input;
    input.add(eval_arg).add(diff_arg);
    cmd.add(input);
    cmd.add(by_arg);
    cmd.add(complex_arg);

    TCLAP::UnlabeledMultiArg<std::string> args("vars", "Variable values", false, "name=value");

    cmd.add(args);
    cmd.parse(argc, argv);

    if (eval_arg.isSet()) {
        if (by_arg.isSet()) {
            std::cerr << "ERROR: --by arg only allowed in differentiation mode. Run with -h to learn more." << std::endl;
            return 1;
        }
        if (complex_arg.isSet()) {
            try {
                expression::Expression<std::complex<double>> expr(eval_arg.getValue());

                std::vector<std::string> vars = args.getValue();
                std::unordered_map<std::string, std::complex<double>> var_map;
                for (const auto &var : vars) {
                    std::stringbuf buf{};
                    std::istringstream is(var);
                    is.get(buf, '=');
                    is.get();

                    std::string name = buf.str();
                    double value;
                    is >> value;
                    var_map[name] = std::complex(value, 0.0);
                }


                expr.substitute("i", std::complex(0.0,1.0));
                std::complex<double> result = expr.evaluate(var_map);
                std::cout << result << std::endl;
            } catch (const expression::ParseError &e) {
                std::cerr << "ERROR when parsing the expression: " << e.what() << std::endl;
                return 1;
            } catch (const expression::SymbolsUnspecified &e) {
                std::cerr << "ERROR during evaluation: " << e.what() << std::endl;
                return 1;
            }
        } else {
            try {
                expression::Expression<double> expr(eval_arg.getValue());

                std::vector<std::string> vars = args.getValue();
                std::unordered_map<std::string, double> var_map;
                for (const auto &var : vars) {
                    std::stringbuf buf{};
                    std::istringstream is(var);
                    is.get(buf, '=');
                    is.get();

                    std::string name = buf.str();
                    double value;
                    is >> value;
                    var_map[name] = value;
                }

                double result = expr.evaluate(var_map);
                std::cout << result << std::endl;
            } catch (const expression::ParseError &e) {
                std::cerr << "ERROR when parsing the expression: " << e.what() << std::endl;
                return 1;
            } catch (const expression::SymbolsUnspecified &e) {
                std::cerr << "ERROR during evaluation: " << e.what() << std::endl;
                return 1;
            }
        }

    }
    else if (diff_arg.isSet()) {
        if (!by_arg.isSet()) {
            std::cerr << "ERROR: --by arg should be set to specify which variable to differentiate by. Run with -h to learn more." << std::endl;
            return 1;
        }
        try {
            expression::Expression<double> expr(diff_arg.getValue());

            std::vector<std::string> vars = args.getValue();
            std::unordered_map<std::string, std::complex<double>> var_map;
            for (const auto &var : vars) {
                std::stringbuf buf{};
                std::istringstream is(var);
                is.get(buf, '=');
                is.get();

                std::string name = buf.str();
                double value;
                is >> value;
                expr.substitute(name, value);
            }

            auto result = expr.differentiate(by_arg.getValue());
            std::cout << result.to_string() << std::endl;
        } catch (const expression::ParseError &e) {
            std::cerr << "ERROR when parsing the expression: " << e.what() << std::endl;
            return 1;
        }
    }

    return 0;
}
