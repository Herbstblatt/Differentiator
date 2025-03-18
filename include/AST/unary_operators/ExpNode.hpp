#pragma once

#include "../UnaryOpNode.hpp"
#include <cmath>
#include <complex>

namespace expression {
    template<typename T>
    class ExpNode : public UnaryOpNode<T> {
    public:
        using UnaryOpNode<T>::UnaryOpNode;

        ExpNode<T>* clone_with(std::unique_ptr<INode<T>> operand) const override {
            return new ExpNode(std::move(operand));
        }

        int priority() const override {
            return -1;
        }

        T apply(T operand) const override {
            return std::exp(operand);
        }

        std::string wrap_str(std::string operand) const override {
            return "exp(" + operand + ")";
        }

        INode<T>* differentiate(std::string by) const override {
            std::unique_ptr<INode<T>> inner(this->m_operand->clone());
            std::unique_ptr<INode<T>> inner_diff(this->m_operand->differentiate(by));
            auto orig_diff = std::make_unique<ExpNode>(std::move(inner));

            return new MultiplyNode<T>(std::move(orig_diff), std::move(inner_diff));
        }
    };
};