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
    Node* curr = find(root, val);

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
    
    if (curr->parent) {
        if (val < curr->parent->key)
            curr->parent->left = succ;
        else
            curr->parent->right = succ;
    } else {
        root = succ; // removing root
    }
    delete curr;
}

Node* SplayTree::succ_r(Node * n) {
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
    
    // splay(); // splay the tree
}

bool SplayTree::lookup(int val) {
    return find(root, val) != nullptr;
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
