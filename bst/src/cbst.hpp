#ifndef BST_CBST_HPP
#define BST_CBST_HPP

#include "CNode.hpp"

namespace cbst {
    CNode* search(CNode* root, int key) {
        CNode* node = root;
        while (true) {
            if (node->key == key) {
                return node;
            }

            auto child = const_cast<CNode *>(node->key < key ? node->right : node->left);
            if (child == nullptr) {
                return node;
            }
            node = child;
        }
    }

    bool contains(CNode* root, int key) {
        CNode* node = search(root, key);
        while (node->key > key) {
            node = const_cast<CNode *>(node->pred);
        }
        while (node->key < key) {
            node = const_cast<CNode *>(node->succ);
        }

        return (node->key == key && !(node->mark));
    }

    CNode* choose_parent(CNode* p, CNode* s, CNode* first_candidate) {
        CNode* candidate = first_candidate == p || first_candidate == s ? first_candidate : p;

        while (true) {
            candidate->tree_lock.lock();
            if (candidate == p) {
                if (candidate->right == nullptr) {
                    return candidate;
                }
                candidate->tree_lock.unlock();
                candidate = s;
            } else {
                if (candidate->left == nullptr) {
                    return candidate;
                }
                candidate->tree_lock.unlock();
                candidate = p;
            }
        }
    }

    void insert_to_tree(CNode* parent, CNode* new_node) {
        new_node->parent = parent;
        if (parent->key < new_node->key) {
            parent->right = new_node;
        } else {
            parent->left = new_node;
        }
    }

    bool insert(CNode* root, int key) {
        while (true) {
            CNode* node = search(root, key);
            auto* p = const_cast<CNode *>(node->key > key ? node->pred : node);
            p->succ_lock.lock();
            auto* s = const_cast<CNode *>(p->succ);

            if (p->key < key && s->key >= key && !p->mark) {
                if (s->key == key) {
                    p->succ_lock.unlock();
                    return false;
                }

                auto* new_node = new CNode(key);
                CNode* parent = choose_parent(p, s, node);
                new_node->succ = s;
                new_node->pred = p;
                s->pred = new_node;
                p->succ = new_node;

                p->succ_lock.unlock();

                insert_to_tree(parent, new_node);
                return true;
            }
            p->succ_lock.unlock();
        }
    }

    void lock_parent(CNode* node) {
        while (true) {
            auto* p = const_cast<CNode *>(node->parent);
            p->tree_lock.lock();
            if (node->parent == p && !(p->mark)) {
                return;
            }
            p->tree_lock.unlock();
        }
    }

    bool restart(CNode* node, CNode* parent) {
        if (parent != nullptr) {
            parent->tree_lock.unlock();
        }

        while (true) {
            node->tree_lock.unlock();
            node->tree_lock.lock();

            if (node->mark) {
                node->tree_lock.unlock();
                return false;
            }

            return true;
        }
    }

    bool acquire_tree_locks(CNode* n) {
        n->tree_lock.lock();
        lock_parent(n);
        if (n->right == nullptr || n->left == nullptr) {
            if (n->right != nullptr) {
                auto* right = const_cast<CNode *>(n->right);
                if (!right->tree_lock.try_lock()) {
                    restart(right, n); // restart
                }
            } else if (n->left != nullptr) {
                auto* left = const_cast<CNode *>(n->left);
                if (!left->tree_lock.try_lock()) {
                    restart(left, n); //restart
                }
            }

            return false;
        } else {
            auto* s = const_cast<CNode *>(n->succ);
            if (s->parent != n) {
                auto* parent = const_cast<CNode *>(s->parent);

                if (!parent->tree_lock.try_lock()) {
                    restart(s, parent); // restart
                }

                if (parent != s->parent || parent->mark) {
                    restart(s, parent); // restart
                }
            }

            if (!s->tree_lock.try_lock()) {
                restart(s, n); // restart
            }

            if (s->right != nullptr) {
                auto* s_right = const_cast<CNode *>(s->right);
                if (!s_right->tree_lock.try_lock()) {
                    restart(s_right, s); // restart
                }
            }

            return true;
        }
    }

    void update_child(CNode* parent, CNode* oldCh, CNode* newCh) {
        if (parent->left == oldCh) {
            parent->left = newCh;
        } else {
            parent->right = newCh;
        }

        if (newCh != nullptr) {
            newCh->parent = parent;
        }
    }

    void remove_from_tree(CNode* n, bool has_two_children) {
        if (!has_two_children) {
            auto* child = const_cast<CNode *>(n->right == nullptr ? n->left : n->right);
            auto* parent = const_cast<CNode *>(n->parent);

            update_child(parent, n, child);
        } else {
            auto* s = const_cast<CNode *>(n->succ);
            auto* child = const_cast<CNode *>(s->parent);
            auto* parent = const_cast<CNode *>(s->parent);
            update_child(parent, s, child);

            s->left = n->left;
            s->right = n->right;
            n->left = parent;

            if (n->right != nullptr) {
                n->right->parent = s;
            }

            update_child(const_cast<CNode *>(n->parent), n, s);

            if (parent == n) {
                parent = s;
            } else {
                s->tree_lock.unlock();
            }
        }

        n->tree_lock.unlock();
    }

    bool remove(CNode* root, int key) {
        while (true) {
            CNode* node = search(root, key);
            auto* p = const_cast<CNode *>(node->key > key ? node->pred : node);
            p->succ_lock.lock();
            auto* s = const_cast<CNode *>(p->succ);

            if (p->key < key && s->key >= key && !(p->mark)) {
                if (s->key > key) {
                    p->succ_lock.unlock();
                    return false;
                }
                s->succ_lock.lock();

                bool has_two_children = acquire_tree_locks(s);
                s->mark = true;
                auto* s_succ = const_cast<CNode *>(s->succ);
                s_succ->pred = p;
                p->succ = s_succ;

                s->succ_lock.unlock();
                p->succ_lock.unlock();

                remove_from_tree(s, has_two_children);
                return true;
            }

            p->succ_lock.unlock();
        }
    }
}

#endif //BST_CBST_HPP
