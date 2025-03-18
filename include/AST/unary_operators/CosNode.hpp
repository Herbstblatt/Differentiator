#pragma once

#include "AST/UnaryOpNode.hpp"
#include "AST/unary_operators/MinusNode.hpp"
#include <cmath>
#include <complex>

namespace expression {
    template<typename  T>
    class SinNode;

    template<typename T>
    class CosNode : public UnaryOpNode<T> {
    public:
        using UnaryOpNode<T>::UnaryOpNode;

        CosNode<T>* clone_with(std::unique_ptr<INode<T>> operand) const override {
            return new CosNode(std::move(operand));
        }

        int priority() const override {
            return -1;
        }

        T apply(T operand) const override {
            return std::cos(operand);
        }

        std::string wrap_str(std::string operand) const override {
            return "cos(" + operand + ")";
        }

        INode<T>* differentiate(std::string by) const override {
            std::unique_ptr<INode<T>> inner(this->m_operand->clone());
            std::unique_ptr<INode<T>> inner_diff(this->m_operand->differentiate(by));
            auto orig_sin = std::make_unique<SinNode<T>>(std::move(inner));
            auto orig_diff = std::make_unique<MinusNode<T>>(std::move(orig_sin));

            return new MultiplyNode<T>(std::move(orig_diff), std::move(inner_diff));
        }
    };
};