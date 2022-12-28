#include "iostream"

#include "src/SeqBST.hpp"

void seq_correctness_test() {
    auto* seq_bst = new SeqBST();
    std::cout << seq_bst->insert(1);
    std::cout << seq_bst->insert(2);
    std::cout << seq_bst->insert(3);
    std::cout << seq_bst->contains(3);
    std::cout << seq_bst->remove(1);
    std::cout << seq_bst->contains(1);

    seq_bst->inorder_traversal();
}

int main() {
    seq_correctness_test();
}