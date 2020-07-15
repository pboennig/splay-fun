/* SplayTree
 * ---------
 *  Implementation of a splay tree container, first described by
 *  Sleator and Tarjan in 1985. By splaying, O(log n) amortized for all
 *  operations and, as shown in CS166, fulfills balance propery (equivalent to RB, etc.),
 *  entropy property (weight-balanced), dynamic finger (lookups "local"), and
 *  working set property in one structure!
 */

#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>

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
          bool operator!=(const iterator& rhs) {return !(*this == rhs);};
        private:
          SplayTree *st_; 
          pointer ptr_;
    };
    class const_iterator {
        public: 
          using value_type = T;
          using difference_type = std::ptrdiff_t;
          using pointer = std::shared_ptr<Node>;
          using reference = Node&;
          using iterator_category = std::forward_iterator_tag;
          const_iterator(SplayTree* st, pointer ptr) : st_(st), ptr_(ptr) {};
          const_iterator operator++(int x) { iterator res = this; ++*this; return iterator(st_, res); };
          const_iterator operator++() { ptr_ = st_->succ_r(ptr_); return *this; };
          value_type operator*() const { return ptr_->key; };
          bool operator==(const iterator& rhs) {return (st_ == rhs.st_ && ptr_ == rhs.ptr_);};
          bool operator!=(const iterator& rhs) {return !(*this == rhs);};
        private:
          const SplayTree *st_; 
          const pointer ptr_;
    };
    public: 

        SplayTree();
        ~SplayTree() = default; // smart pointers remove need for recursive frees

        
        /* Functions : insert, remove, and lookup 
         * ---------
         * Standard BST operations. Note tha lookup is not const because of the
         * splaying necessary to insure working set property. Therefore,
         * read-write locks don't work on splay trees
         */
        void insert(T);
        void remove(T); // no-op if not in tree
        bool lookup(T); // splays node (if in tree) to top for working set property

        /* Function: split
         * --------
         * Join S into this, assuming that all elements in S are greater than the largest element in this
         */ 
        void join(SplayTree<T>& S);

        /* Function: split
         * --------
         * Split this based on element x. After calling, this contains all elements <= x while we return tree T
         * with all elements >= x. 
         */ 
        SplayTree split(T x);

        /* Functions: begin() and end() 
         * --------
         * Iterator begin and end functions for use with STL functions.
         */
        iterator begin() {
          std::shared_ptr<Node> curr = root;
          if (curr)  
            while(curr->left) { curr = curr->left; }

          return iterator(this, curr);
        }
        iterator end() {
          return iterator(this, nullptr);
        }

        const_iterator begin() const {
          std::shared_ptr<Node> curr = root;
          if (curr)  
            while(curr->left) { curr = curr->left; }

          return const_iterator(this, curr);
        }
        const_iterator end() const {
          return const_iterator(this, nullptr);
        }

    private:
        std::shared_ptr<Node> root;

        /* Function: splay
         * -------
         * Splays Node n to root. See implementation for mechanical details,
         * but we can think of this as balancing the tree by removing "kinks"
         * in the branches.
         */
        void splay(std::shared_ptr<Node> n);

        /* Functions: succ_r and find
         * --------
         * Simple helper functions for finding the successor to a node
         * and finding a particular value starting at a given node.
         */
        std::shared_ptr<Node> succ_r(std::shared_ptr<Node> n) const;
        std::shared_ptr<Node> find(std::shared_ptr<Node> n, T val) const;

        /* Functions: remove_node and fix_parent 
         * --------
         * Helpers for removing a value. Remove_node removes a node (not a value), while
         * fix_parent cleans up the parent pointers as necessary in doubly-linked tree.
         */
        void remove_node(std::shared_ptr<Node> curr); // removes node (not value)
        void fix_parent(std::shared_ptr<Node> curr, std::shared_ptr<Node> succ); // cleans up parent pointers

        /* Functions:: rotate_right and rotate_left
         * --------
         * Rotation functions, same behavior as in red-black trees.
         * Necessary for splaying, as we attempt to balance the tree
         * by removing runs of same-direction edges.
         */
        void rotate_right(std::shared_ptr<Node> n);
        void rotate_left(std::shared_ptr<Node> n);
};

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
    if (n->right) {
      std::shared_ptr<Node> succ = n->right; 
      while (succ->left) { succ = succ->left; }
      return succ;
    }
    std::shared_ptr<Node> parent = n->parent.lock();
    while (n->parent.lock() && n == parent->right) {
      n = parent;
      parent = parent->parent.lock();
    }
    return parent;
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


template<class T>
void SplayTree<T>::splay(std::shared_ptr<Node> n) {
    if (root == n) 
        return; // root case
    std::shared_ptr<Node> parent = n->parent.lock();
    std::shared_ptr<Node> grandparent = parent->parent.lock();
    if (parent == root) {
        // Case 1: zig
        if (parent->left == n) 
            rotate_right(parent);
        else
            rotate_left(parent);
    } else {
        if (parent->left == n && grandparent->right == parent) {
            /* Case 2a: zig-zag
             *   x
             *    \
             *     y
             *    /
             *   n
             */
            rotate_right(parent);
            rotate_left(grandparent);
        } else if (parent->right == n && grandparent->left == parent) {
            /* Case 2b: zig-zag
             *     x
             *    /
             *   y
             *    \
             *     n
             */
            rotate_left(parent);
            rotate_right(grandparent);
        } else if (parent->right == n && grandparent->right == parent) {
            /* Case 3a: zig-zig right
             *  x
             *   \
             *    y
             *     \
             *      n
             */
            rotate_left(grandparent);
            rotate_left(parent);
        } else {
            /* Case 3b: zig-zig left
             *        x
             *      /
             *    y
             *  /
             * n
             */
            rotate_right(grandparent);
            rotate_right(parent);
        }
        splay(n);
    }
}



template<class T>
void SplayTree<T>::rotate_right(std::shared_ptr<Node> n) {
    std::shared_ptr<Node> left_child = n->left;
    std::shared_ptr<Node> parent = n->parent.lock();
    n->left = left_child->right;
    if (n->left)
        n->left->parent = n;
    left_child->right = n;
    left_child->parent = parent;
    if (!parent)
        root = left_child;
    else if (parent->left == n)
        parent->left = left_child;
    else
        parent->right = left_child;
    n->parent = left_child;
}

template<class T>
void SplayTree<T>::rotate_left(std::shared_ptr<Node> n) {
    std::shared_ptr<Node> right_child = n->right;
    std::shared_ptr<Node> parent = n->parent.lock();
    n->right = right_child->left;
    if (n->right)
        n->right->parent = n;
    right_child->left = n;
    right_child->parent = parent; 
    if (!parent)
        root = right_child;
    else if (parent->left == n)
        parent->left = right_child;
    else
        parent->right = right_child;
    n->parent = right_child;
}

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
