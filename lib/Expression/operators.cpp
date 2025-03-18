#include "AST/Node.hpp"

#include "AST/binary_operators/AddNode.hpp"
#include "AST/binary_operators/SubstractNode.hpp"
#include "AST/binary_operators/MultiplyNode.hpp"
#include "AST/binary_operators/DivideNode.hpp"
#include "AST/binary_operators/PowerNode.hpp"

#include "AST/unary_operators/SinNode.hpp"
#include "AST/unary_operators/CosNode.hpp"
#include "AST/unary_operators/ExpNode.hpp"
#include "AST/unary_operators/LogNode.hpp"
#include "AST/unary_operators/MinusNode.hpp"

#include <memory>

namespace expression {
    template <template<typename> typename U, typename T>
    Expression<T> apply(const Expression<T>& operand) {
        std::unique_ptr<INode<T>> op_cloned(operand.m_tree->clone());
        std::unique_ptr<U<T>> new_expr = std::make_unique<U<T>>(std::move(op_cloned));
        return Expression<T>(std::move(new_expr));
    }

    template <template<typename> typename U, typename T>
    Expression<T> apply(const Expression<T>& left_operand, const Expression<T>& right_operand) {
        std::unique_ptr<INode<T>> left_cloned(left_operand.m_tree->clone());
        std::unique_ptr<INode<T>> right_cloned(right_operand.m_tree->clone());

        std::unique_ptr<U<T>> new_expr = std::make_unique<U<T>>(std::move(left_cloned), std::move(right_cloned));
        return Expression<T>(std::move(new_expr));
    }

    template<typename U>
    Expression<U> operator+(const Expression<U>& expr1, const Expression<U>& expr2) {
        return apply<AddNode, U>(expr1, expr2);
    }
    
    template<typename U>
    Expression<U> operator-(const Expression<U>& expr1, const Expression<U>& expr2) {
        return apply<SubtractNode, U>(expr1, expr2);
    }

    template<typename U>
    Expression<U> operator-(const Expression<U>& expr) {
        return apply<MinusNode, U>(expr);
    }
    
    template<typename U>
    Expression<U> operator*(const Expression<U>& expr1, const Expression<U>& expr2) {
        return apply<MultiplyNode, U>(expr1, expr2);
    }
    
    template<typename U>
    Expression<U> operator/(const Expression<U>& expr1, const Expression<U>& expr2) {
        return apply<DivideNode, U>(expr1, expr2);
    }
    
    template <typename U>
    Expression<U> operator^(const Expression<U> &expr1,
                            const Expression<U> &expr2) {
      return apply<PowerNode, U>(expr1, expr2);
    }

    template <typename U>
    Expression<U> sin(const Expression<U>& expr1) {
        return apply<SinNode, U>(expr1);
    }

    template<typename U>
    Expression<U> cos(const Expression<U>& expr1) {
        return apply<CosNode, U>(expr1);
    }

    template<typename U>
    Expression<U> log(const Expression<U>& expr1) {
        return apply<LogNode, U>(expr1);
    }

    template<typename U>
    Expression<U> exp(const Expression<U>& expr1) {
        return apply<ExpNode, U>(expr1);
    }
};