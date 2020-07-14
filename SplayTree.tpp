/* SplayTree.tpp
 * ---------
 * Core BST functions for SplayTree, using a normal BST with
 * parent pointer.
 */
#include "SplayTree.h"

template <class T>
SplayTree<T>::SplayTree() {
    root = nullptr;
}

template <class T>
void SplayTree<T>::fix_parent(std::shared_ptr<Node> curr, std::shared_ptr<Node> succ) {
    T val = curr->key;
    std::shared_ptr<Node> parent = curr->parent.lock();
    if (parent) {
        if (val < parent->key)
            parent->left = succ;
        else
            parent->right = succ;
    } else {
        root = succ;
    }
}

template <class T>
void SplayTree<T>::remove(T val) {
    try {
        SplayTree<T> r = split(val); // splay value to top
        remove_node(root); // remove from root
        join(r); // join remaining trees
    } catch (const std::exception &e) {
        return; // not in tree, no-op
    }
}

template <class T>
void SplayTree<T>::remove_node(std::shared_ptr<Node> curr) {
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

template <class T>
std::shared_ptr<typename SplayTree<T>::Node> SplayTree<T>::succ_r(std::shared_ptr<Node>  n) const {
    // find left-most right child (successor)
    if (n == nullptr || n->right == nullptr) return nullptr;
    std::shared_ptr<Node> succ = n->right; 
    while (succ->left) { succ = succ->left; }
    return succ;
}

template <class T>
void SplayTree<T>::insert(T val) {
    std::shared_ptr<Node> prev = nullptr;
    std::shared_ptr<Node> curr = root;
    while (curr) {
        prev = curr;
        if (val < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }
    std::shared_ptr<Node> n = std::make_shared<Node>(Node{val, nullptr, nullptr, std::weak_ptr<Node>()});

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

template <class T>
bool SplayTree<T>::lookup(T val) {
    std::shared_ptr<Node> n = find(root, val);
    if (!n) return false;
    splay(n); // hot nodes go to top
    return true;
}

template <class T>
std::shared_ptr<typename SplayTree<T>::Node> SplayTree<T>::find(std::shared_ptr<Node> n, T val) const {
    if (!n || n->key == val) return n;
    if (val < n->key) return find(n->left, val);
    else return find(n->right, val);
}

template <class T>
std::vector<T> SplayTree<T>::vec() const {
    return vec_r(root);
}

template <class T>
std::vector<T> SplayTree<T>::vec_r(std::shared_ptr<Node> n) const {
    if (!n) return std::vector<int>();
    std::vector<T> l = vec_r(n->left);
    l.push_back(n->key);
    std::vector<T> r = vec_r(n->right);
    l.insert(l.end(), r.begin(), r.end());
    return l;
}
