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
#include <vector>
typedef struct Node {
    int key;
    Node* left;
    Node* right;
    Node* parent; // for ease of splay
} Node;

class SplayTree {
    public: 
        SplayTree();

        ~SplayTree();

        // BST ops
        void insert(int val);
        void remove(int val); // no-op if not in tree
        bool lookup(int val);

        std::vector<int> vec();

    private:
        Node* root;
        void splay(Node *last); // core splay operation

        // recursive helper functions
        std::vector<int> vec_r(Node* n);
        void delete_r(Node* n);
        Node* succ_r(Node * n);
        Node* find(Node * n, int val);
};
#endif
