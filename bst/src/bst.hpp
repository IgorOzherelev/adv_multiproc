#ifndef BST_BST_HPP
#define BST_BST_HPP

#include "iostream"
#include "Node.hpp"

namespace bst {
    bool insert(Node* root, int key) {
        Node* node = new Node(key);
        Node* curr = root;
        Node* trailing = nullptr;
        if (root == nullptr) {
            return false;
        }

        while (curr != nullptr) {
            trailing = curr;
            if (node->key > curr->key) {
                curr = curr->right;
            } else if (node->key < curr->key) {
                curr = curr->left;
            } else {
                return false;
            }
        }

        if (trailing == nullptr) {
            trailing = node;
        } else if (key < trailing->key) {
            trailing->left = node;
            node->parent = trailing;
        } else {
            trailing->right = node;
            node->parent = trailing;
        }

        return true;
    }

    bool contains(Node* root, int key) {
        Node* curr = root;
        while (curr != nullptr) {
            if (key > curr->key) {
                curr = curr->right;
            } else if (key < curr->key) {
                curr = curr->left;
            } else {
                return true;
            }
        }

        return false;
    }

    bool remove(Node* root, int key) {
        Node* curr = root;
        while (curr != nullptr && curr->key != key) {
            if (key > curr->key) {
                curr = curr->right;
            } else if (key < curr->key) {
                curr = curr->left;
            }
        }

        if (curr == nullptr) {
            return false;
        }

        if (curr->left == nullptr || curr->right == nullptr) {
            Node* newCurr;
            newCurr = curr->left == nullptr ? curr->right : curr->left;
            if (curr->parent == nullptr) {
                return true;
            }

            if (curr == curr->parent->left) {
                curr->parent->left = newCurr;
            } else {
                curr->parent->right = newCurr;
            }

            free(curr);
        } else {
            Node* p = nullptr;
            Node* temp;
            temp = curr->right;
            while (temp->left != nullptr) {
                p = temp;
                temp = temp->left;
            }

            if (p != nullptr) {
                p->left = temp->right;
            } else {
                curr->right = temp->right;
            }

            curr->key = temp->key;
            free(temp);
        }

        return true;
    }
}

#endif //BST_BST_HPP
