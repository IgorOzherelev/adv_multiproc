#ifndef BST_CBST_HPP
#define BST_CBST_HPP

#include "iostream"
#include "vector"
#include "stack"
#include "atomic"
#include "shared_mutex"

#include "BST.hpp"

template<typename T>
class CNode {
public:
    T key;
    CNode<T>* left;
    CNode<T>* right;
    State state = DATA;

    std::atomic<bool> deleted{false};

    // lock_shared() - reader
    // lock() - writer
    std::shared_mutex left_lock;
    std::shared_mutex right_lock;
    std::shared_mutex state_lock;

    explicit CNode() = default;

    explicit CNode(T key, State state) : key(key), state(state) {}

    virtual ~CNode() = default;

    void tryLockLeftEdgeRef(CNode<T>* expRef) {
        right_lock.lock();

        if (deleted.load() || left != expRef) {
            throw std::runtime_error("error tryLockLeftEdgeRef");
        }
    }

    void tryLockRightEdgeRef(CNode<T>* expRef) {
        left_lock.lock();

        if (deleted.load() || right != expRef) {
            throw std::runtime_error("error tryLockRightEdgeRef");
        }
    }

    void tryLockEdgeRef(CNode<T>* expRef) {
        if (key < expRef->key) {
            tryLockRightEdgeRef(expRef);
        } else {
            tryLockLeftEdgeRef(expRef);
        }
    }

    void tryLockLeftEdgeVal(T expVal) {
        left_lock.lock();

        if (deleted.load() || left == nullptr || left->key != expVal) {
            throw std::runtime_error("error tryLockLeftEdgeVal");
        }
    }

    void tryLockRightEdgeVal(T expVal) {
        right_lock.lock();

        if (deleted.load() || right == nullptr || right->key != expVal) {
            throw std::runtime_error("error tryLockRightEdgeVal");
        }
    }

    void tryLockEdgeVal(CNode<T>* expRef) {
        if (key < expRef->key) {
            tryLockRightEdgeVal(expRef->key);
        } else {
            tryLockLeftEdgeVal(expRef->key);
        }
    }

    void tryReadLockState() {
        state_lock.lock_shared();

        if (deleted.load()) {
            throw std::runtime_error("error tryReadLockState");
        }
    }

    void tryReadLockState(State expState) {
        state_lock.lock_shared();

        if (deleted.load() || state != expState) {
            throw std::runtime_error("error tryReadLockState with state");
        }
    }

    void tryWriteLockState(State expState) {
        state_lock.lock();

        if (deleted.load() || state != expState) {
            throw std::runtime_error("error tryWriteLockState");
        }
    }
};

class CBST : public BST<uint32_t> {
private:
    bool checkDeleted(CNode<uint32_t>* node) {
        if (node != nullptr && node->deleted) {
            return true;
        }

        return false;
    }

    std::vector<CNode<uint32_t>*> traversal(uint32_t key) {
        while (true) {
            auto* g_prev = new CNode<uint32_t>();
            auto* prev = new CNode<uint32_t>();
            auto* curr = root;

            while (curr != nullptr) {
                if (curr->key == key) {
                    break;
                } else {
                    g_prev = prev;
                    prev = curr;
                    if (curr->key < key) {
                        curr = curr->left;
                    } else {
                        curr = curr->right;
                    }
                }

                if (checkDeleted(g_prev)) break;
                if (checkDeleted(prev)) break;
                if (checkDeleted(curr)) break;
            }
            if (checkDeleted(g_prev)) continue;
            if (checkDeleted(prev)) continue;
            if (checkDeleted(curr)) continue;

            return { g_prev, prev, curr };
        }
    }

    void lock_one_child_vertex(CNode<uint32_t> *prev, CNode<uint32_t> *curr, CNode<uint32_t> *child) {
        prev->tryLockEdgeRef(curr);
        curr->tryWriteLockState(DATA);

        if (curr->left != nullptr && curr->right != nullptr) {
            throw std::runtime_error("curr has 2 children");
        }

        if (curr->left == nullptr && curr->right == nullptr) {
            throw std::runtime_error("curr has 0 children");
        }

        curr->tryLockEdgeRef(child);
    }

    CNode<uint32_t> *lockLeaf(uint32_t key, CNode<uint32_t> *prev, CNode<uint32_t> *curr) {
        prev->tryLockEdgeVal(curr);

        if (key < prev->key) {
            curr = prev->left;
        } else {
            curr = prev->right;
        }

        curr->tryWriteLockState(DATA);
        if (curr->left != nullptr || curr->right != nullptr) {
            throw std::runtime_error("curr is not a leaf");
        }

        return curr;
    }

public:
    CNode<uint32_t>* root;

    explicit CBST() {
        this->root = new CNode<uint32_t>(INT32_MAX, DATA);
    }

    virtual ~CBST() = default;

    bool contains(uint32_t key) override {
        std::vector<CNode<uint32_t>*> traversal = this->traversal(key);
        CNode<uint32_t>* curr = traversal[2];

        return curr != nullptr && curr->state == DATA;
    }

    bool insert(uint32_t key) override {
        std::vector<CNode<uint32_t>*> traversal = this->traversal(key);
        auto* prev = traversal[1];
        auto* curr = traversal[2];

        if (curr != nullptr) {
            if (curr->state == DATA) {
                return false;
            }

            curr->tryWriteLockState(ROUTING);
            curr->state = DATA;
            curr->state_lock.unlock();
        } else {
            auto* new_node = new CNode<uint32_t>(key, DATA);

            if (key < prev->key) {
                prev->tryLockLeftEdgeRef(nullptr);
                prev->tryReadLockState();

                prev->left = new_node;

                prev->left_lock.unlock();
                prev->state_lock.unlock_shared();
            } else {
                prev->tryLockRightEdgeRef(nullptr);
                prev->tryReadLockState();

                prev->right = new_node;

                prev->right_lock.unlock();
                prev->state_lock.unlock_shared();
            }
        }

        return true;
    }

    bool remove(uint32_t key) override {
        std::vector<CNode<uint32_t>*> traversal = this->traversal(key);
        auto* g_prev = traversal[0];
        auto* prev = traversal[1];
        auto* curr = traversal[2];

        if (curr == nullptr || curr->state != DATA) {
            return false;
        }

        if (curr->left != nullptr && curr->right != nullptr) {
            curr->tryWriteLockState(DATA);

            if (curr->left != nullptr || curr->right == nullptr) {
                throw std::runtime_error("doesnt have 2 children");
            }

            curr->state = ROUTING;
            curr->state_lock.unlock();
        } else if (curr->left != nullptr || curr->right != nullptr) {
            CNode<uint32_t>* child = curr->left == nullptr ? curr->left : curr->right;

            if (curr->key < prev->key) {
                lock_one_child_vertex(prev, curr, child);
                curr->deleted = true;
                prev->left = child;

                prev->left_lock.unlock();
                prev->right_lock.unlock();
                curr->state_lock.unlock();
                curr->left_lock.unlock();
                curr->right_lock.unlock();
            } else {
                lock_one_child_vertex(prev, curr, child);
                curr->deleted = true;
                prev->right = child;

                prev->left_lock.unlock();
                prev->right_lock.unlock();
                curr->state_lock.unlock();
                curr->left_lock.unlock();
                curr->right_lock.unlock();
            }
        } else {
            if (prev->state == DATA) {
                if (curr->key < prev->key) {
                    prev->tryReadLockState(DATA);

                    curr = lockLeaf(key, prev, curr);

                    curr->deleted = true;
                    prev->left = nullptr;

                    prev->state_lock.unlock_shared();
                    prev->left_lock.unlock();
                    prev->right_lock.unlock();
                    curr->state_lock.unlock();
                } else {
                    prev->tryReadLockState(DATA);

                    curr = lockLeaf(key, prev, curr);

                    curr->deleted = true;
                    prev->right = nullptr;

                    prev->state_lock.unlock_shared();
                    prev->left_lock.unlock();
                    prev->right_lock.unlock();
                    curr->state_lock.unlock();
                }
            } else {
                CNode<uint32_t>* child;
                if (curr->key < prev->key) {
                    child = prev->right;
                } else {
                    child = prev->left;
                }

                if (g_prev->left != nullptr && prev == g_prev->left) {
                    g_prev->tryLockEdgeRef(prev);
                    prev->tryWriteLockState(ROUTING);
                    prev->tryLockEdgeRef(child);

                    curr = lockLeaf(key, prev, child);

                    prev->deleted = true;
                    curr->deleted = true;

                    g_prev->left = child;

                    g_prev->left_lock.unlock();
                    g_prev->right_lock.unlock();

                    prev->left_lock.unlock();
                    prev->right_lock.unlock();

                    prev->state_lock.unlock();
                } else if (g_prev->right != nullptr && prev == g_prev->right) {
                    g_prev->tryLockEdgeRef(prev);
                    prev->tryWriteLockState(ROUTING);
                    prev->tryLockEdgeRef(child);

                    curr = lockLeaf(key, prev, curr);

                    prev->deleted = true;
                    curr->deleted = true;
                    g_prev->right = child;

                    g_prev->left_lock.unlock();
                    g_prev->right_lock.unlock();

                    prev->left_lock.unlock();
                    prev->right_lock.unlock();

                    prev->state_lock.unlock();
                }
            }
        }

        return true;
    }

    std::vector<uint32_t> inorder_traversal() override {
        std::vector<uint32_t> list = {};
        std::stack<CNode<uint32_t>*> stack = {};
        CNode<uint32_t>* curr = root;
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


#endif //BST_CBST_HPP
