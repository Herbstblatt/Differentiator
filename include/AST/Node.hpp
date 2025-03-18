#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace expression {
    template <typename T>
    class INode {
    public:
        virtual int priority() const = 0;

        virtual T evaluate(std::unordered_map<std::string, T> symbols) const = 0;
        virtual std::string stringify() const = 0;
        virtual INode<T>* differentiate(std::string by) const = 0;
        virtual void substitute(std::string symbol, T value) = 0;

        virtual ~INode() = default;
        virtual INode<T>* clone() const = 0;
    };    
}