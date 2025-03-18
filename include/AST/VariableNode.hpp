#pragma once

#include "LiteralNode.hpp"
#include "Node.hpp"

#include <exceptions.h>
#include <string>
#include <unordered_map>

namespace expression {
    template<typename T>
    class Expression;

    template<typename T>
    class BinaryOpNode;

    template<typename T>
    class UnaryOpNode;

    template<typename T>
    class VariableNode : public INode<T> {
        friend class Expression<T>;
        friend class UnaryOpNode<T>;
        friend class BinaryOpNode<T>;

    public:
        explicit VariableNode(const std::string_view name)
            : m_name { name }
            {}
        
        int priority() const override {
            return -1;
        }

        T evaluate(std::unordered_map<std::string, T> symbols) const override {
            if (!symbols.contains(m_name)) {
                throw SymbolsUnspecified("Unable to evaluate: missing required symbol " + m_name);
            }
            return symbols[m_name];
        }

        std::string stringify() const override {
            return m_name;
        };

        INode<T>* differentiate(std::string by) const override {
            if (by == m_name) {
                return new LiteralNode<T>(T(1));
            } else {
                return new LiteralNode<T>(T(0));
            }
        }
        
        void substitute(std::string symbol, T value) override {};

        VariableNode<T>* clone() const override {
            return new VariableNode<T>(m_name);
        }
    private:
        std::string m_name;
    };
}