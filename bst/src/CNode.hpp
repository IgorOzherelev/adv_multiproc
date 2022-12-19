#ifndef BST_CNODE_HPP
#define BST_CNODE_HPP

#include "iostream"
#include "pthread.h"

class CNode {
public:
    int key;
    volatile bool mark;

    volatile CNode* left;
    volatile CNode* right;
    volatile CNode* parent;

    volatile CNode* succ;
    volatile CNode* pred;

    std::mutex tree_lock;
    std::mutex succ_lock;

    explicit CNode(int key) {
        this->key = key;

        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }
};

#endif //BST_CNODE_HPP
