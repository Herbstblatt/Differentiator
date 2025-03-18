#pragma once

#include "AST/BinaryOpNode.hpp"
#include "AST/unary_operators/LogNode.hpp"
#include <cmath>
#include <complex>

namespace expression {
    template<typename T>
    class PowerNode : public BinaryOpNode<T> {
    public:
        using BinaryOpNode<T>::BinaryOpNode;

        PowerNode<T>* clone_with(std::unique_ptr<INode<T>> left_operand, std::unique_ptr<INode<T>> right_operand) const override {
            return new PowerNode(std::move(left_operand), std::move(right_operand));
        }

        int priority() const override {
            return 0;
        }

        T apply(T operand1, T operand2) const override {
            return std::pow(operand1, operand2);
        }

        std::string wrap_str(std::string left_operand, std::string right_operand) const override {
            return left_operand + " ^ " + right_operand;
        }

        INode<T>* differentiate(std::string by) const override {
            std::unique_ptr<INode<T>> left1(this->m_left_operand->clone());
            std::unique_ptr<INode<T>> right1(this->m_right_operand->clone());

            std::unique_ptr<INode<T>> left2(this->m_left_operand->clone());
            std::unique_ptr<INode<T>> right2(this->m_right_operand->clone());

            auto operand1 = std::make_unique<PowerNode<T>>(std::move(left1), std::move(right1));
            auto log_right = std::make_unique<LogNode<T>>(std::move(left2));
            auto mul = std::make_unique<MultiplyNode<T>>(std::move(right2), std::move(log_right));
            std::unique_ptr<INode<T>> operand2(mul->differentiate(by));

            return new MultiplyNode<T>(std::move(operand1), std::move(operand2));
        }
    };
};