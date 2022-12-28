#ifndef BST_BST_HPP
#define BST_BST_HPP

enum State {
    DATA,
    ROUTING
};

template<typename T>
class BST {
public:
    virtual bool contains(T key) = 0;
    virtual bool insert(T key) = 0;
    virtual bool remove(T key) = 0;
    virtual void inorder_traversal() = 0;
};

#endif //BST_BST_HPP
