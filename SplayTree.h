/* SplayTree
 * ---------
 *  Implementation of a splay tree container, first described by
 *  Sleator and Tarjan in 1985. By splaying, O(log n) amortized for all
 *  operations and, as shown in CS166, fulfills balance propery (equivalent to RB, etc.),
 *  entropy property (weight-balanced), dynamic finger (lookups "local"), and
 *  working set property in one structure!
 */

#ifndef Splay

#define Splay
#include <cstddef>
#include <vector>
#include <iterator>
#include <memory>

template <class T>
class SplayTree {
    typedef struct Node {
        T key;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent; // weak_ptr to prevent memory leak via cyclic references
    } Node;

    // beginnings of iterator class, not implemented yet
    class iterator {
        public: 
          using value_type = T;
          using difference_type = std::ptrdiff_t;
          using pointer = std::shared_ptr<Node>;
          using reference = Node&;
          using iterator_category = std::forward_iterator_tag;
          iterator(SplayTree* st, pointer ptr) : st_(st), ptr_(ptr) {};
          iterator operator++(int x) { iterator res = this; ++*this; return iterator(st_, res); };
          iterator operator++() { ptr_ = st_->succ_r(ptr_); return *this; };
          value_type operator*() { return ptr_->key; };
          bool operator==(const iterator& rhs) {return (st_ == rhs.st_ && ptr_ == rhs.ptr_);};
          bool operator!=(const iterator& rhs) {return !(this == rhs);};
        private:
          SplayTree *st_; 
          pointer ptr_;
    };
    public: 

        SplayTree();
        ~SplayTree() = default; // smart pointers remove need for recursive frees

        // BST ops
        void insert(T);
        void remove(T); // no-op if not in tree
        bool lookup(T); // splays node (if in tree) to top for working set property

        // returns SplayTree as std::vector<int>, no need once iterator is completed
        std::vector<T> vec() const;

        // join S into this, assuming that all elements in S are greater than the largest element in this
        void join(SplayTree<T>& S);

        // split this based on element x. After calling, this contains all elements <= x while we return tree T
        // with all elements >= x
        SplayTree split(T x);

        iterator begin() {
          std::shared_ptr<Node> curr = root;
          if (curr)  
            while(curr->left) { curr = curr->left; }

          return iterator(this, curr);
        }
        iterator end() {
          return iterator(this, nullptr);
        }

    private:
        std::shared_ptr<Node> root;
        // splays Node n to root
        void splay(std::shared_ptr<Node> n);

        //recursive helpers
        std::vector<T> vec_r(std::shared_ptr<Node> n) const; 
        std::shared_ptr<Node> succ_r(std::shared_ptr<Node> n) const;
        std::shared_ptr<Node> find(std::shared_ptr<Node> n, T val) const;

        // remove helpers
        void remove_node(std::shared_ptr<Node> curr); // removes node (not value)
        void fix_parent(std::shared_ptr<Node> curr, std::shared_ptr<Node> succ); // cleans up parent pointers

        // rotations, same as in RB trees
        void rotate_right(std::shared_ptr<Node> n);
        void rotate_left(std::shared_ptr<Node> n);
};

#include "SplayTree.tpp"
#include "Splay.tpp"
#include "JoinSplit.tpp"
#endif
