/* JoinSplit.cpp  
 * ---------
 *  Implementation of join and split functions. 
 */
#include "SplayTree.h"
#include <stdexcept>

void SplayTree::join(SplayTree& T) {
    std::shared_ptr<Node> max = root; 
    if (root) {
        while (max->right) { max  = max->right; }
        splay(max);
        root->right = T.root;
        if (T.root)
            T.root->parent = root;
    } else {
        root = T.root;
    }
}

SplayTree SplayTree::split(int x) {
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
