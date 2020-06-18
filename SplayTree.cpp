/* SplayTree.cpp
 * ---------
 * Core BST functions for SplayTree, using a normal BST with
 * parent pointer.
 */
#include "SplayTree.h"

SplayTree::SplayTree() {
    root = nullptr;
}

SplayTree::~SplayTree() {
}


void SplayTree::fix_parent(std::shared_ptr<Node> curr, std::shared_ptr<Node> succ) {
    int val = curr->key;
    if (curr->parent) {
        if (val < curr->parent->key)
            curr->parent->left = succ;
        else
            curr->parent->right = succ;
    } else {
        root = succ;
    }
}

void SplayTree::remove(int val) {
    std::shared_ptr<Node> curr = find(root, val);
    if (!curr) return; // not in tree -> no-op
    remove_node(curr);
}

void SplayTree::remove_node(std::shared_ptr<Node> curr) {
    if (!curr->left && !curr->right) {
        // Case 1: No children
        fix_parent(curr, nullptr);
    } else if (!curr->left) {
        // Case 2a: Only right child 
        fix_parent(curr, curr->right);
        curr->right->parent = curr->parent;
    } else if (!curr->right) {
        // Case 2b: Only left child 
        fix_parent(curr, curr->left);
        curr->left->parent = curr->parent;
    } else {
        // Case 3: Two children
        std::shared_ptr<Node> succ = succ_r(curr);
        curr->key = succ->key;
        remove_node(succ); 
    }
}

std::shared_ptr<Node> SplayTree::succ_r(std::shared_ptr<Node>  n) {
    // find left-most right child (successor)
    std::shared_ptr<Node> succ = n->right; 
    while (succ->left) { succ = succ->left; }
    return succ;
}

void SplayTree::insert(int val) {
    std::shared_ptr<Node> prev = nullptr;
    std::shared_ptr<Node> curr = root;
    while (curr) {
        prev = curr;
        if (val < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    std::shared_ptr<Node> n = std::make_shared<Node>(Node{val, nullptr, nullptr, nullptr});

    if (prev) {
        n->parent = prev;
        if (val < prev->key)
            prev->left = n;
        else
            prev->right = n;
    } else {
        root = n;
    }
    splay(n);
}

bool SplayTree::lookup(int val) {
    std::shared_ptr<Node> n = find(root, val);
    if (!n) return false;
    splay(n);
    return true;
}

std::shared_ptr<Node> SplayTree::find(std::shared_ptr<Node> n, int val) {
    if (!n || n->key == val) return n;
    if (val < n->key) return find(n->left, val);
    else return find(n->right, val);
}

std::vector<int> SplayTree::vec() {
    return vec_r(root);
}

std::vector<int> SplayTree::vec_r(std::shared_ptr<Node> n) {
    if (!n) return std::vector<int>();
    std::vector l = vec_r(n->left);
    l.push_back(n->key);
    std::vector r = vec_r(n->right);
    l.insert(l.end(), r.begin(), r.end());
    return l;
}
