#pragma once

#include "AST/Node.hpp"
#include "Parser.hpp"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string_view>

namespace expression {
    template <typename T>
    class Expression {
    public:
        explicit Expression(char literal);
        explicit Expression(T number);
        explicit Expression(std::string_view string);
    
        Expression(const Expression& other);
        Expression& operator=(const Expression& other);

        T evaluate() const;
        T evaluate(std::unordered_map<std::string, T> symbols) const;
        void substitute(std::string_view symbol, T value);
        Expression<T> differentiate(std::string by) const;
        std::string to_string() const;

        template <template<typename> typename U, typename R>
        friend Expression<R> apply(const Expression<R>&);

        template <template<typename> typename U, typename R>
        friend Expression<R> apply(const Expression<R>&, const Expression<R>&);

        template<typename U>
        friend Expression<U> operator+(const Expression<U>& expr1, const Expression<U>& expr2);
        
        template<typename U>
        friend Expression<U> operator-(const Expression<U>& expr1, const Expression<U>& expr2);

        template<typename U>
        friend Expression<U> operator-(const Expression<U>& expr);
        
        template<typename U>
        friend Expression<U> operator*(const Expression<U>& expr1, const Expression<U>& expr2);
        
        template<typename U>
        friend Expression<U> operator/(const Expression<U>& expr1, const Expression<U>& expr2);
        
        template<typename U>
        friend Expression<U> operator^(const Expression<U>& expr1, const Expression<U>& expr2);

        template<typename U>
        friend Expression<U> sin(const Expression<U>& expr1);

        template<typename U>
        friend Expression<U> cos(const Expression<U>& expr1);

        template<typename U>
        friend Expression<U> log(const Expression<U>& expr1);

        template<typename U>
        friend Expression<U> exp(const Expression<U>& expr1);
    private:
        std::unique_ptr<INode<T>> m_tree;

        explicit Expression(std::unique_ptr<INode<T>> tree);
    };

    template<typename U>
    Expression<U> sin(const Expression<U>& expr1);

    template<typename U>
    Expression<U> cos(const Expression<U>& expr1);

    template<typename U>
    Expression<U> log(const Expression<U>& expr1);

    template<typename U>
    Expression<U> exp(const Expression<U>& expr1);
}

#include "../lib/Expression/operators.cpp"
#include "../lib/Expression/constructors.cpp"
#include "../lib/Expression/methods.cpp"
