#ifndef BST_NODE_HPP
#define BST_NODE_HPP

class Node {
public:
    int key;
    Node* left;
    Node* right;
    Node* parent;

    explicit Node(int key) {
        this->key = key;

        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }
};

#endif //BST_NODE_HPP
