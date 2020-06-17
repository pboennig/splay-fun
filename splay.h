/* SplayTree
 * ---------
 *  Implementation of a splay tree structure for ints, first described by
 *  Sleator and Tarjan in 1985. By splaying, O(log n) amortized for all
 *  operations and, as shown in CS166, fulfills balance (RB, etc.) property,
 *  entropy property (weight-balanced), dynamic finger (lookups "local"), and
 *  working set property in one structure!
 */

#ifndef Splay

#define Splay
#include <cstddef>
typedef struct Node {
    int key;
    Node* left;
    Node* right;
} Node;

class SplayTree {
    public: 
        SplayTree();

        ~SplayTree();

        // BST ops
        void insert(int val);
        void remove(int val); // no-op if not in tree
        bool lookup(int val);

        void print();

    private:
        Node* root;
        void splay(Node *last); // core splay operation

        // recursive helper functions
        Node* insert_r(int val, Node* n);
        void print_r(Node* n);
        void delete_r(Node* n);
        bool lookup_r(int val, Node* n);
};
#endif
