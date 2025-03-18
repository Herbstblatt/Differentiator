#pragma once

#include "../BinaryOpNode.hpp"

namespace expression {
    template<typename T>
    class AddNode : public BinaryOpNode<T> {
    public:
        using BinaryOpNode<T>::BinaryOpNode;

        AddNode<T>* clone_with(std::unique_ptr<INode<T>> left_operand, std::unique_ptr<INode<T>> right_operand) const override {
            return new AddNode(std::move(left_operand), std::move(right_operand));
        }

        int priority() const override {
            return 2;
        }

        T apply(T operand1, T operand2) const override {
            return operand1 + operand2;
        }

        std::string wrap_str(std::string left_operand, std::string right_operand) const override {
            return left_operand + " + " + right_operand;
        }

        INode<T>* differentiate(std::string by) const override {
            std::unique_ptr<INode<T>> left(this->m_left_operand->differentiate(by));
            std::unique_ptr<INode<T>> right(this->m_right_operand->differentiate(by));

            return new AddNode(std::move(left), std::move(right));
        }
    };
};