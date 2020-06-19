/* Splay.cpp
 * ---------
 * Splay functionality required for amortized behavior
 * of Splay Trees.
 */
#include "SplayTree.h"

void SplayTree::splay(std::shared_ptr<Node> n) {
    if (root == n) 
        return; // root case
    if (n->parent == root) {
        // Case 1: zig
        if (n->parent->left == n) 
            rotate_right(n->parent);
        else
            rotate_left(n->parent);
    } else {
        std::shared_ptr<Node> parent = n->parent;
        std::shared_ptr<Node> grandparent = parent->parent;
        if (parent->left == n && grandparent->right == parent) {
            /* Case 2a: zig-zag
             *   x
             *    \
             *     y
             *    /
             *   n
             */
            rotate_right(parent);
            rotate_left(grandparent);
        } else if (parent->right == n && grandparent->left == parent) {
            /* Case 2b: zig-zag
             *     x
             *    /
             *   y
             *    \
             *     n
             */
            rotate_left(parent);
            rotate_right(grandparent);
        } else if (parent->right == n && grandparent->right == parent) {
            /* Case 3a: zig-zig right
             *  x
             *   \
             *    y
             *     \
             *      n
             */
            rotate_left(grandparent);
            rotate_left(parent);
        } else {
            /* Case 3b: zig-zig left
             *        x
             *      /
             *    y
             *  /
             * n
             */
            rotate_right(grandparent);
            rotate_right(parent);
        }
        splay(n);
    }
}

void SplayTree::rotate_right(std::shared_ptr<Node> n) {
    std::shared_ptr<Node> left_child = n->left;
    std::shared_ptr<Node> parent = n->parent;
    n->left = left_child->right;
    if (n->left)
        n->left->parent = n;
    left_child->right = n;
    left_child->parent = parent;
    if (!parent)
        root = left_child;
    else if (parent->left == n)
        parent->left = left_child;
    else
        parent->right = left_child;
    n->parent = left_child;
}

void SplayTree::rotate_left(std::shared_ptr<Node> n) {
    std::shared_ptr<Node> right_child = n->right;
    std::shared_ptr<Node> parent = n->parent;
    n->right = right_child->left;
    if (n->right)
        n->right->parent = n;
    right_child->left = n;
    right_child->parent = parent; 
    if (!parent)
        root = right_child;
    else if (parent->left == n)
        parent->left = right_child;
    else
        parent->right = right_child;
    n->parent = right_child;
}
