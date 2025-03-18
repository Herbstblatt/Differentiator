#include "Expression.hpp"
#include "tclap/CmdLine.h"
#include <iostream>
#include <sstream>

std::istream &operator>>(std::istream &is, std::pair<std::string, int> &p) {
    std::stringbuf buf{};
    is.get(buf, '=');
    is.get();
    p.first = buf.str();
    return is >> p.second;
}

int main(int argc, char** argv) {
    TCLAP::CmdLine cmd("A simple calculator with differentiation support", ' ', "0.1");

    TCLAP::ValueArg<std::string> eval_arg("","eval","An expression to evaluate",false,"","expr");
    TCLAP::ValueArg<std::string> diff_arg("","diff","An expression to differentiate",false,"","expr");
    TCLAP::ValueArg<std::string> by_arg("","by","A variable to differentiate by",false,"","variable");

    TCLAP::OneOf input;
    input.add(eval_arg).add(diff_arg);
    cmd.add(input);
    cmd.add(by_arg);

    TCLAP::UnlabeledMultiArg<std::pair<std::string, int>> args("vars", "Variable values", false, "name=value");

    cmd.add(args);
    cmd.parse(argc, argv);

    if (eval_arg.isSet()) {
        if (by_arg.isSet()) {
            std::cerr << "ERROR: --by arg only allowed in differentiation mode. Run with -h to learn more." << std::endl;
            return 1;
        }
        try {
            expression::Expression<int> expr(eval_arg.getValue());

            std::vector<std::pair<std::string, int>> vars = args.getValue();
            std::unordered_map<std::string, int> var_map(vars.begin(), vars.end());

            int result = expr.evaluate(var_map);
            std::cout << result << std::endl;
        } catch (const expression::ParseError &e) {
            std::cerr << "ERROR when parsing the expression: " << e.what() << std::endl;
            return 1;
        } catch (const expression::SymbolsUnspecified &e) {
            std::cerr << "ERROR during evaluation: " << e.what() << std::endl;
            return 1;
        }
    }
    else if (diff_arg.isSet()) {
        if (!by_arg.isSet()) {
            std::cerr << "ERROR: --by arg should be set to specify which variable to differentiate by. Run with -h to learn more." << std::endl;
            return 1;
        }
        try {
            expression::Expression<int> expr(diff_arg.getValue());

            std::vector<std::pair<std::string, int>> vars = args.getValue();
            for (const auto& [name, value] : vars) {
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
