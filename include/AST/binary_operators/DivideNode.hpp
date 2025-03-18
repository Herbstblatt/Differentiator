#pragma once

#include "../BinaryOpNode.hpp"
#include "MultiplyNode.hpp"
#include "PowerNode.hpp"
#include "SubstractNode.hpp"
#include "exceptions.h"

namespace expression {
    template<typename T>
    class DivideNode : public BinaryOpNode<T> {
    public:
        using BinaryOpNode<T>::BinaryOpNode;

        DivideNode<T>* clone_with(std::unique_ptr<INode<T>> left_operand, std::unique_ptr<INode<T>> right_operand) const override {
            return new DivideNode(std::move(left_operand), std::move(right_operand));
        }

        int priority() const override {
            return 1;
        }

        T apply(T operand1, T operand2) const override {
            if (operand2 == T(0)) {
                throw ZeroDivision("Division by zero");
            }
            return operand1 / operand2;
        }

        std::string wrap_str(std::string left_operand, std::string right_operand) const override {
            return left_operand + " / " + right_operand;
        }

        INode<T>* differentiate(std::string by) const override {
            std::unique_ptr<INode<T>> left(this->m_left_operand->clone());
            std::unique_ptr<INode<T>> right(this->m_right_operand->clone());
            std::unique_ptr<INode<T>> right2(this->m_right_operand->clone());

            std::unique_ptr<INode<T>> left_diff(this->m_left_operand->differentiate(by));
            std::unique_ptr<INode<T>> right_diff(this->m_right_operand->differentiate(by));

            auto two = std::make_unique<LiteralNode<T>>(T(2));

            auto member1 = std::make_unique<MultiplyNode<T>>(std::move(left_diff), std::move(right));
            auto member2 = std::make_unique<MultiplyNode<T>>(std::move(left), std::move(right_diff));

            auto num = std::make_unique<SubtractNode<T>>(std::move(member1), std::move(member2));
            auto denom = std::make_unique<PowerNode<T>>(std::move(right2), std::move(two));

            return new DivideNode(std::move(num), std::move(denom));
        };
    };
};