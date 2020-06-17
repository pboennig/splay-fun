/* SplayTree.cpp
 * ---------
 * Splay functionality required for amortized behavior
 * of Splay Trees.
 */
#include "SplayTree.h"

void SplayTree::splay(Node* n) {
    if (n->parent == root) {
        // Case 1: zig
        if (n->parent->left == n) 
            rotate_right(n);
        else
            rotate_left(n);
    }
    // Case 2: zig-zag
    // Case 3: zig-zig
}

void SplayTree::rotate_right(Node* root) {
    Node* left_child = root->left;
    root->left = left_child->right;
    left_child->right = root;
    left_child->parent = root->parent;
    if (!root->parent) root = left_child;
    root->parent = left_child;
}

void SplayTree::rotate_left(Node* root) {
    Node* right_child = root->right;
    root->right = right_child->left;
    right_child->left = root;
    right_child->parent = root->parent;
    if (!root->parent) root = right_child;
    root->parent = right_child;
}
