#pragma once

#include "Node.hpp"
#include "VariableNode.hpp"
#include "LiteralNode.hpp"

#include <memory>

namespace expression {
    template<typename T>
    class UnaryOpNode : public INode<T> {
    public:
        explicit UnaryOpNode(std::unique_ptr<INode<T>> operand)
            : m_operand { std::move(operand) }
            {}

        virtual UnaryOpNode<T>* clone_with(std::unique_ptr<INode<T>> operand) const = 0;      
        virtual T apply(T operand) const = 0;
        virtual std::string wrap_str(std::string operand) const = 0;

        T evaluate(std::unordered_map<std::string, T> symbols) const override {
            T result = m_operand->evaluate(symbols);
            return apply(result);
        };

        std::string stringify() const override {
            std::string str = m_operand->stringify();
            return wrap_str(str);
        };

        UnaryOpNode<T>* clone() const override {
            std::unique_ptr<INode<T>> inner_clone(m_operand->clone());
            return clone_with(std::move(inner_clone));
        };
        
        void substitute(std::string symbol, T value) override {
            auto* d = dynamic_cast<VariableNode<T>*>(m_operand.get());
            if (d && d->m_name == symbol) {
                m_operand.reset(new LiteralNode(value));
            } else {
                m_operand->substitute(symbol, value);
            }
        };
    protected:
        std::unique_ptr<INode<T>> m_operand;
    };
}
