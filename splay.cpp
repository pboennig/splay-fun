#include "splay.h"
#include <iostream>

SplayTree::SplayTree() {
    root = nullptr;
}

SplayTree::~SplayTree() {
    root = nullptr;
}

void::SplayTree::delete_r(Node* n) {
    if (!n) return;
    delete_r(n->left);
    delete_r(n->right);
    delete n;
}

void SplayTree::insert(int val) {
    root = insert_r(val, root); // normal insert
    // splay(); // splay the tree
}

Node* SplayTree::insert_r(int val, Node* n) {
    if (!n)
        return new Node{val, nullptr, nullptr};
    else if (val < n->key)
        n->left = insert_r(val, n->left);
    else
        n->right = insert_r(val, n->right);
    return n; 
}

bool SplayTree::lookup(int val) {
    return lookup_r(val, root);
}

bool SplayTree::lookup_r(int val, Node* n) {
    if (!n) return false;
    if (n->key == val) return true;
    
    if (val < n->key)
        return lookup_r(val, n->left);
    else
        return lookup_r(val, n->right);
}

void SplayTree::print() {
    print_r(root);
}

void SplayTree::print_r(Node* n) {
    if (!n) return;
    print_r(n->left);
    std::cout << n->key << std::endl;
    print_r(n->right);
}
