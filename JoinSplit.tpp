/* JoinSplit.tpp  
 * ---------
 *  Implementation of join and split functions. 
 */
#include "SplayTree.h"
#include <stdexcept>

template <class T>
void SplayTree<T>::join(SplayTree<T>& S) {
    std::shared_ptr<Node> max = root; 
    if (root) {
        while (max->right) { max  = max->right; }
        splay(max);
        root->right = S.root;
        if (S.root)
            S.root->parent = root;
    } else {
        root = S.root;
    }
}

template <class T>
SplayTree<T> SplayTree<T>::split(T x) {
    std::shared_ptr<Node> n = find(root, x);
    if (n == nullptr)
        throw std::invalid_argument("Value not in SplayTree");
    splay(n);
    SplayTree r;
    r.root = root->right;
    if (root->right) {
        root->right->parent = std::weak_ptr<Node>(); 
        root->right = nullptr;
    }
    return r;
}
