#include "splay.h"
#include <iostream>

int main() {

    SplayTree splay_tree;
    for (int i = 0; i < 10; i++)
        splay_tree.insert(i);

    splay_tree.print();

    for (int i = 2; i < 20; i += 2) 
        std::cout << i << ": " << splay_tree.lookup(i) << std::endl;
}
