#pragma once

#include "AST/UnaryOpNode.hpp"
#include <cmath>
#include <complex>

namespace expression {
    template<typename  T>
    class CosNode;

    template<typename T>
    class SinNode : public UnaryOpNode<T> {
    public:
        using UnaryOpNode<T>::UnaryOpNode;

        SinNode<T>* clone_with(std::unique_ptr<INode<T>> operand) const override {
            return new SinNode(std::move(operand));
        }

        int priority() const override {
            return -1;
        }

        T apply(T operand) const override {
            return std::sin(operand);
        }

        std::string wrap_str(std::string operand) const override {
            return "sin(" + operand + ")";
        }

        INode<T>* differentiate(std::string by) const override {
            std::unique_ptr<INode<T>> inner(this->m_operand->clone());
            std::unique_ptr<INode<T>> inner_diff(this->m_operand->differentiate(by));
            auto orig_diff = std::make_unique<CosNode<T>>(std::move(inner));

            return new MultiplyNode<T>(std::move(orig_diff), std::move(inner_diff));
        }
    };
};