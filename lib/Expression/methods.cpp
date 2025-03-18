namespace expression {
    template<typename T>
    T Expression<T>::evaluate() const {
        return evaluate({});
    }

    template<typename T>
    T Expression<T>::evaluate(std::unordered_map<std::string, T> symbols) const {
        return m_tree->evaluate(symbols);
    }

    template<typename T>
    void Expression<T>::substitute(std::string_view symbol, T value) {
        VariableNode<T>* d = dynamic_cast<VariableNode<T>*>(m_tree.get());
        if (d && d->m_name == symbol) {
            m_tree.reset(new LiteralNode(value));
        } else {
            m_tree->substitute(std::string{symbol}, value);
        }
    }

    template <typename T>
    Expression<T> Expression<T>::differentiate(std::string by) const {
        std::unique_ptr<INode<T>> tree(m_tree->differentiate(by));
        return Expression<T>(std::move(tree));
    }

    template<typename T>
    std::string Expression<T>::to_string() const {
        return m_tree->stringify();
    }
}