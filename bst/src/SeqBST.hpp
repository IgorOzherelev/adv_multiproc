#ifndef BST_SEQBST_HPP
#define BST_SEQBST_HPP

#include "iostream"
#include "vector"
#include "stack"
#include <climits>
#include "BST.hpp"

template<typename T>
class SeqNode {
public:
    T key;
    SeqNode<T>* left = nullptr;
    SeqNode<T>* right = nullptr;
    State state = DATA;

    explicit SeqNode() = default;

    explicit SeqNode(T key, State state) : key(key), state(state) {}

    virtual ~SeqNode() = default;
};

class SeqBST : public BST<uint32_t> {
private:
    std::vector<SeqNode<uint32_t> *> traversal(uint32_t key) {
        auto* g_prev = new SeqNode<uint32_t>();
        auto* prev = new SeqNode<uint32_t>();
        auto* curr = this->root;

        while (curr != nullptr) {
            if (curr->key == key) {
                break;
            } else {
                g_prev = prev;
                prev = curr;
                if (curr->key > key) {
                    curr = curr->left;
                } else {
                    curr = curr->right;
                }
            }
        }

        return { g_prev, prev, curr };
    }

public:
    SeqNode<uint32_t>* root;

    explicit SeqBST() {
        this->root = new SeqNode<uint32_t>(INT32_MAX, DATA);
    }

    virtual ~SeqBST() {}

    bool contains(uint32_t key) override {
        std::vector<SeqNode<uint32_t>*> traversal = this->traversal(key);
        SeqNode<uint32_t>* curr = traversal[2];

        return curr != nullptr && curr->state == DATA;
    }

    bool insert(uint32_t key) override {
        std::vector<SeqNode<uint32_t>*> traversal = this->traversal(key);
        SeqNode<uint32_t>* prev = traversal[1];
        SeqNode<uint32_t>* curr = traversal[2];

        if (curr != nullptr) {
            if (curr->state == DATA) {
                return false;
            }

            curr->state = DATA;
        } else {
            auto* new_node = new SeqNode<uint32_t>(key, DATA);

            if (prev->key > key) {
                prev->left = new_node;
            } else {
                prev->right = new_node;
            }
        }

        return true;
    }

    bool remove(uint32_t key) override {
        std::vector<SeqNode<uint32_t>*> traversal = this->traversal(key);
        auto* g_prev = traversal[0];
        auto* prev = traversal[1];
        auto* curr = traversal[2];

        if (curr == nullptr || curr->state != DATA) {
            return false;
        }

        if (curr->left != nullptr && curr->right != nullptr) {
            curr->state = ROUTING;
        } else if (curr->left != nullptr || curr->right != nullptr) {
            SeqNode<uint32_t>* child = curr->left != nullptr ? curr->left : curr->right;

            if (curr->key < prev->key) {
                prev->left = child;
            } else {
                prev->right = child;
            }
        } else {
            if (prev->state == DATA) {
                if (curr == prev->left) {
                    prev->left = nullptr;
                } else {
                    prev->right = nullptr;
                }
            } else {
                SeqNode<uint32_t>* child;
                if (curr == prev->left) {
                    child = prev->right;
                } else {
                    child = prev->left;
                }

                if (prev == g_prev->left) {
                    g_prev->left = child;
                } else {
                    g_prev->right = child;
                }
            }
        }

        return true;
    }

    std::vector<uint32_t> inorder_traversal() override {
        std::vector<uint32_t> list = {};
        std::stack<SeqNode<uint32_t>*> stack = {};
        SeqNode<uint32_t>* curr = root;
        while (curr != nullptr || !stack.empty()) {
            while (curr != nullptr) {
                stack.push(curr);
                curr = curr->left;
            }

            curr = stack.top();
            stack.pop();
            if (curr->state == DATA) {
                list.push_back(curr->key);
            }
            curr = curr->right;
        }

        return list;
    }
};

#endif //BST_SEQBST_HPP
