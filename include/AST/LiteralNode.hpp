#pragma once

#include "Node.hpp"
#include <sstream>

namespace expression {
    template<typename T>
    class LiteralNode : public INode<T> {
    public:
        LiteralNode(T value)
            : m_value { value }
            {}

        int priority() const override {
            return -1;
        }

        T evaluate(std::unordered_map<std::string, T> symbols) const override {
            return m_value;
        }
        
        std::string stringify() const override {
            std::ostringstream oss{};
            oss << m_value;
            return oss.str();
        }
        
        LiteralNode<T>* differentiate(std::string by) const /*override*/ {
            return new LiteralNode(T(0));
        }
        
        void substitute(std::string symbol, T value) override {}

        LiteralNode<T>* clone() const override {
            return new LiteralNode<T>(m_value);
        }
    private:
        T m_value;
    };
};