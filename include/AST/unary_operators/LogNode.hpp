#pragma once

#include "../UnaryOpNode.hpp"
#include "exceptions.h"
#include <cmath>
#include <complex>

namespace expression {
    template<typename  T>
    class DivideNode;

    template<typename T>
    class LogNode : public UnaryOpNode<T> {
    public:
        using UnaryOpNode<T>::UnaryOpNode;

        LogNode<T>* clone_with(std::unique_ptr<INode<T>> operand) const override {
            return new LogNode(std::move(operand));
        }

        int priority() const override {
            return -1;
        }

        T apply(T operand) const override {
            if (operand == T(0)) {
                throw DefAreaViolation("Unable to calculate logarithm");
            }
            return std::log(operand);
        }

        std::string wrap_str(std::string operand) const override {
            return "ln(" + operand + ")";
        }

        INode<T>* differentiate(std::string by) const override {
            std::unique_ptr<INode<T>> inner(this->m_operand->clone());
            std::unique_ptr<INode<T>> inner_diff(this->m_operand->differentiate(by));
            auto one = std::make_unique<LiteralNode<T>>(1);
            auto orig_diff = std::make_unique<DivideNode<T>>(std::move(one), std::move(inner));

            return new MultiplyNode<T>(std::move(orig_diff), std::move(inner_diff));
        }
    };
};