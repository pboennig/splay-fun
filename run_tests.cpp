#include "SplayTree.h"
#include <cmath>
#include <iostream>
#include <set>
#include <algorithm>
#include <random>

int NUM_ELEMS = 100000;

void print_vecs(SplayTree& st, std::set<int>& s) {
    std::vector<int> v = st.vec();
    std::cout << "Splay tree: ";
    for (const int& i: v)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "Set: ";
    for (const int& i: s)
        std::cout << i << " ";
    std::cout << std::endl;
}

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

bool insert_tests(SplayTree& st, std::set<int>& s, std::vector<int>& elems, std::mt19937 g) {
    std::shuffle(elems.begin(), elems.end(), g);
    for (int i = 0; i < NUM_ELEMS / 3; i++) {
        st.insert(elems[i]);
        s.insert(elems[i]);
    }
    return check_equal(st, s);
}

bool remove_tests(SplayTree& st, std::set<int>& s, std::vector<int>& elems, std::mt19937 g) {
    std::shuffle(elems.begin(), elems.end(), g);
    for (int i = 0; i < NUM_ELEMS; i++) {
        st.remove(elems[i]);
        s.erase(elems[i]);
    }
    return check_equal(st, s);
}
bool join_tests(std::mt19937 g) {
    std::set<int> s;
    std::set<int> t;

    SplayTree st_s;
    SplayTree st_t;

    std::vector<int> v_1(100);
    std::vector<int> v_2(100);
    
    std::iota(v_1.begin(), v_1.end(), 0);
    std::iota(v_2.begin(), v_2.end(), 100);

    std::shuffle(v_1.begin(), v_1.end(), g);
    std::shuffle(v_2.begin(), v_2.end(), g);

    std::for_each(v_1.begin(), v_1.end(), [&s, &st_s](int &i) {
                                           s.insert(i); st_s.insert(i);});
    std::for_each(v_2.begin(), v_2.end(), [&t, &st_t](int &i) {
                                           t.insert(i); st_t.insert(i);});
    s.merge(t);
    st_s.join(st_t);
    return check_equal(st_s, s);
}
int main() {
    std::vector<int> elems;
    for (int i = 0; i < NUM_ELEMS; i++) 
        elems.push_back(i);

    std::random_device rd;
    std::mt19937 g(rd());

    SplayTree splay_tree;
    std::set<int> s;
    bool i = insert_tests(splay_tree, s, elems, g);
    bool r = remove_tests(splay_tree, s, elems, g);
    bool j = join_tests(g);
    std::cout << "Insertion tests: " << i << std::endl;
    std::cout << "Remove tests: " << r << std::endl;
    std::cout << "Join tests: " << j << std::endl;

}
