#include "splay.h"
#include <iostream>

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

void SplayTree::remove(int val) {
    Node* prev = nullptr;
    Node* curr = root;
    while (curr) {
        if (curr->key == val) break;
        
        prev = curr;
        if (val < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    if (!curr) return; // not in tree

    Node* succ;
    if (!curr->left && !curr->right)
        succ = nullptr; //leaf node, just delete
    else if (!curr->left)
        succ = curr->right;
    else if (!curr->right)
        succ = curr->left;
    else
        succ = succ_r(curr); //in-order successor
       
    delete curr;

    if (val < prev->key)
        prev->left = succ;
    else
        prev->right = succ;
}

Node* SplayTree::succ_r(Node * n) {
    Node* succ = n->right;
    while (succ) { succ = succ->left; }
    return succ;
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
