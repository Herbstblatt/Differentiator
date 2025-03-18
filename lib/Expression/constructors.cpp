namespace expression {
    template<typename T>
    Expression<T>::Expression(char literal)
        : m_tree(new VariableNode<T>(std::string{literal}))
        {}

    template<typename T>
    Expression<T>::Expression(T number)
        : m_tree(new LiteralNode<T>(number))
        {}

    template<typename T>
    Expression<T>::Expression(std::string_view string) {
        Parser<T> parser(string);
        std::unique_ptr<INode<T>> tree = parser.parse();
        m_tree.swap(tree);
    }

    template<typename T>
    Expression<T>::Expression(const Expression<T>& other) {
        INode<T>* tree_clone = other.m_tree.clone();
        m_tree.reset(tree_clone);
    }
    
    template<typename T>
    Expression<T>& Expression<T>::operator=(const Expression<T>& other) {
        if (other != *this) {
            INode<T>* tree_clone = other.m_tree.clone();
            m_tree.reset(tree_clone);
        }
        return *this;
    }

    template <typename T>
    Expression<T>::Expression(std::unique_ptr<INode<T>> tree)
        : m_tree{std::move(tree)}
        {}
}
