#include <iostream>
#include "src/bst.hpp"

using namespace std;
using namespace bst;

void inorder(Node* root)
{
    if (root == nullptr)
        return;
    else {
        inorder(root->left);
        std::cout << root->key << " ";
        inorder(root->right);
    }
}

int main()
{
    return 0;
}

//    /*
//         10
//        /  \
//       7    15
//      / \   / \
//      5  8 11 18
//
//    */
//    Node* root = new Node(10);
//    insert(root, 7);
//    insert(root, 5);
//    insert(root, 8);
//    insert(root, 15);
//    insert(root, 11);
//    insert(root, 18);
//
//    cout << "Inorder traversal "
//         << "of original BST:\n";
//    inorder(root);
//    cout << '\n';
//
//    // delete node with data value 11 (leaf)
//    remove(root, 11);
//    cout << "\nDeletion of 11\n";
//    cout << "Inorder traversal post deletion:\n";
//    inorder(root);
//    cout << '\n';
//
//    // delete node with data value 15
//    // (internal node with one child)
//    remove(root, 15);
//    cout << "\nDeletion of 15\n";
//    cout << "Inorder traversal post deletion:\n";
//    inorder(root);
//    cout << '\n';
//
//    // delete node with data value 10
//    // (root, two children)
//    remove(root, 10);
//    cout << "\nDeletion of 10\n";
//    cout << "Inorder traversal post deletion:\n";
//    inorder(root);
//    cout << '\n';
//
//    cout << contains(root, 8) << contains(root, 10) << std::endl;
//
//    Inorder traversal of original BST:
//    5 7 8 10 11 15 18
//
//    Deletion of 11
//    Inorder traversal post deletion:
//    5 7 8 10 15 18
//
//    Deletion of 15
//    Inorder traversal post deletion:
//    5 7 8 10 18
//
//    Deletion of 10
//    Inorder traversal post deletion:
//    5 7 8 18
//    10