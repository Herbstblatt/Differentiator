#pragma once

#include "Node.hpp"
#include "VariableNode.hpp"
#include <memory>

namespace expression {
    template<typename T>
    class BinaryOpNode : public INode<T> {
    public:
        BinaryOpNode(std::unique_ptr<INode<T>> left_operand, std::unique_ptr<INode<T>> right_operand)
            : m_left_operand { std::move(left_operand) }
            , m_right_operand { std::move(right_operand) }
            {}

        virtual BinaryOpNode<T>* clone_with(std::unique_ptr<INode<T>> left_operand, std::unique_ptr<INode<T>> right_operand) const = 0;      
        virtual T apply(T left_operand, T right_operand) const = 0;
        virtual std::string wrap_str(std::string left_operand, std::string right_operand) const = 0;
        
        T evaluate(std::unordered_map<std::string, T> symbols) const override {
            T result_left = m_left_operand->evaluate(symbols);
            T result_right = m_right_operand->evaluate(symbols);
            return apply(result_left, result_right);
        }
        
        std::string stringify() const override {
            std::string left_str = m_left_operand->stringify();
            if (m_left_operand->priority() > this->priority()) {
                left_str = "(" + left_str + ")";
            }

            std::string right_str = m_right_operand->stringify();
            if (m_right_operand->priority() > this->priority()) {
                right_str = "(" + right_str + ")";
            }

            return wrap_str(left_str, right_str);
        };

        void substitute(std::string symbol, T value) override {
            auto* d_left = dynamic_cast<VariableNode<T>*>(m_left_operand.get());
            if (d_left && d_left->m_name == symbol) {
                m_left_operand.reset(new LiteralNode(value));
            } else {
                m_left_operand->substitute(symbol, value);
            }

            auto* d_right = dynamic_cast<VariableNode<T>*>(m_right_operand.get());
            if (d_right && d_right->m_name == symbol) {
                m_right_operand.reset(new LiteralNode(value));
            } else {
                m_right_operand->substitute(symbol, value);
            }
        }
        
        BinaryOpNode<T>* clone() const override {
            std::unique_ptr<INode<T>> left_clone(m_left_operand->clone());
            std::unique_ptr<INode<T>> right_clone(m_right_operand->clone());

            return clone_with(std::move(left_clone), std::move(right_clone));
        }
    protected:
        std::unique_ptr<INode<T>> m_left_operand;
        std::unique_ptr<INode<T>> m_right_operand;
    };
}