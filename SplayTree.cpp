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
    delete_r(root);
}

void SplayTree::delete_r(Node* n) {
    if (!n) return;
    delete_r(n->left);
    delete_r(n->right);
    delete n;
}

void SplayTree::fix_parent(Node* curr, Node* succ) {
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
    Node* curr = find(root, val);
    if (!curr) return; // not in tree -> no-op
    remove_node(curr);
}

void SplayTree::remove_node(Node* curr) {
    if (!curr->left && !curr->right) {
        // Case 1: No children
        fix_parent(curr, nullptr);
        delete curr;
    } else if (!curr->left) {
        // Case 2a: Only right child 
        fix_parent(curr, curr->right);
        curr->right->parent = curr->parent;
        delete curr;
    } else if (!curr->right) {
        // Case 2b: Only left child 
        fix_parent(curr, curr->left);
        curr->left->parent = curr->parent;
        delete curr;
    } else {
        // Case 3: Two children
        Node* succ = succ_r(curr);
        curr->key = succ->key;
        remove_node(succ); 
    }
}

Node* SplayTree::succ_r(Node * n) {
    // find left-most right child (successor)
    Node* succ = n->right; 
    while (succ->left) { succ = succ->left; }
    return succ;
}

void SplayTree::insert(int val) {
    Node* prev = nullptr;
    Node* curr = root;
    while (curr) {
        prev = curr;
        if (val < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    Node *n = new Node{val, nullptr, nullptr, nullptr};

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
    Node* n = find(root, val);
    if (!n) return false;
    splay(n);
    return true;
}

Node* SplayTree::find(Node* n, int val) {
    if (!n || n->key == val) return n;
    if (val < n->key) return find(n->left, val);
    else return find(n->right, val);
}

std::vector<int> SplayTree::vec() {
    return vec_r(root);
}

std::vector<int> SplayTree::vec_r(Node* n) {
    if (!n) return std::vector<int>();
    std::vector l = vec_r(n->left);
    l.push_back(n->key);
    std::vector r = vec_r(n->right);
    l.insert(l.end(), r.begin(), r.end());
    return l;
}
