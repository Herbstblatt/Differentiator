#pragma once

#include "../BinaryOpNode.hpp"

namespace expression {
    template<typename T>
    class MultiplyNode : public BinaryOpNode<T> {
    public:
        using BinaryOpNode<T>::BinaryOpNode;

        MultiplyNode<T>* clone_with(std::unique_ptr<INode<T>> left_operand, std::unique_ptr<INode<T>> right_operand) const override {
            return new MultiplyNode(std::move(left_operand), std::move(right_operand));
        }

        int priority() const override {
            return 1;
        }

        T apply(T operand1, T operand2) const override {
            return operand1 * operand2;
        }

        std::string wrap_str(std::string left_operand, std::string right_operand) const override {
            return left_operand + " * " + right_operand;
        }

        INode<T>* differentiate(std::string by) const override {
            std::unique_ptr<INode<T>> left(this->m_left_operand->clone());
            std::unique_ptr<INode<T>> right(this->m_right_operand->clone());

            std::unique_ptr<INode<T>> left_diff(this->m_left_operand->differentiate(by));
            std::unique_ptr<INode<T>> right_diff(this->m_right_operand->differentiate(by));

            auto member1 = std::make_unique<MultiplyNode<T>>(std::move(left), std::move(right_diff));
            auto member2 = std::make_unique<MultiplyNode<T>>(std::move(left_diff), std::move(right));

            return new AddNode<T>(std::move(member1), std::move(member2));
        }
    };
}