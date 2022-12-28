#ifndef BST_SEQBST_HPP
#define BST_SEQBST_HPP

#include "BST.hpp"

template<typename T>
class SeqNode {
public:
    T key;
    SeqNode<T>* left;
    SeqNode<T>* right;
    State state;
};

class SeqBST : public BST<int> {
public:

};

#endif //BST_SEQBST_HPP
