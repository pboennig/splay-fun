#include "splay.h"
#include <iostream>
#include <set>

bool check_equal(SplayTree& st, std::set<int>& s) {
    std::vector<int> v = st.vec();
    size_t i = 0;
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (*it != v[i]) {
            return false;
        }
        ++i;
    }
    return true;
}

bool insert_tests(SplayTree& st, std::set<int>& s) {
    for (int i = 0; i < 10; i++) {
        st.insert(i);
        s.insert(i);
    }
    return check_equal(st, s);
}

bool remove_tests(SplayTree& st, std::set<int>& s) {
    for (int i = 2; i < 20; i += 1) {
        st.remove(i);
        s.erase(i);
    }

    return check_equal(st, s);
}

int main() {
    SplayTree splay_tree;
    std::set<int> s;
    bool i = insert_tests(splay_tree, s);
    bool r = remove_tests(splay_tree, s);
    std::cout << "Insertion tests: " << i << std::endl;
    std::cout << "Remove tests: " << r << std::endl;
}
