#pragma once

#include "../UnaryOpNode.hpp"
#include <cmath>
#include <complex>

namespace expression {
template<typename T>
class MinusNode : public UnaryOpNode<T> {
public:
  using UnaryOpNode<T>::UnaryOpNode;

  MinusNode<T>* clone_with(std::unique_ptr<INode<T>> operand) const override {
    return new MinusNode(std::move(operand));
  }

  int priority() const override {
    return -1;
  }

  T apply(T operand) const override {
    return -operand;
  }

  std::string wrap_str(std::string operand) const override {
    return "-" + operand;
  }

  INode<T>* differentiate(std::string by) const override {
    std::unique_ptr<INode<T>> operand(this->m_operand->differentiate(by));
    return new MinusNode(std::move(operand));
  };
};
};