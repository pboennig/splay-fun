#include "SplayTree.h"

void SplayTree::join(SplayTree T) {
    std::shared_ptr<Node> max = root; 
    while (max->right) { max  = max->right; }
    splay(max);
    root->right = T.root;
    T.root->parent = root;
    T.root = nullptr; 
}
