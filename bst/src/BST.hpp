#ifndef BST_BST_HPP
#define BST_BST_HPP

enum State {
    DATA,
    ROUTING
};

template<typename T>
class BST {
public:
    virtual bool contains() = 0;
    virtual bool insert() = 0;
    virtual bool remove() = 0;
};

#endif //BST_BST_HPP
