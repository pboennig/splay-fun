#include "SplayTree.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <random>
#include <chrono>

int NUM_ELEMS = 100000;

void print_vecs(SplayTree<int>& st, std::set<int>& s) {
    std::cout << "Splay tree: ";
    for (const int& i: st)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "Set: ";
    for (const int& i: s)
        std::cout << i << " ";
    std::cout << std::endl;
}

bool check_equal(SplayTree<int>& st, std::set<int>& s) {
    auto st_it = st.begin();
    for (auto it = s.begin(); it != s.end(); ++it) {
        if (*it != *st_it) {
            return false;
        }
        ++st_it;
    }
    return true;
}

bool insert_tests(SplayTree<int>& st, std::set<int>& s, std::vector<int>& elems, std::mt19937 g) {
    std::shuffle(elems.begin(), elems.end(), g);
    for (int i = 0; i < NUM_ELEMS / 3; i++) {
        st.insert(elems[i]);
        s.insert(elems[i]);
    }
    return check_equal(st, s);
}

bool remove_tests(SplayTree<int>& st, std::set<int>& s, std::vector<int>& elems, std::mt19937 g) {
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

    SplayTree<int> st_s;
    SplayTree<int> st_t;

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
    for (const int& i: t)
      s.insert(i);
    st_s.join(st_t);
    return check_equal(st_s, s);
}

bool split_tests(std::vector<int>& elems, std::mt19937 g) {
    std::shuffle(elems.begin(), elems.end(), g);
    SplayTree<int> st;
    std::for_each(elems.begin(), elems.end(), [&st](int &i) { st.insert(i); });
    std::uniform_int_distribution<> dis(0, elems.size() - 1);
    int split_num = dis(g);
    SplayTree<int> r = st.split(split_num);
    int left_max = *std::max_element(st.begin(), st.end());
    auto it = std::find_if(r.begin(), r.end(), [&left_max](int i) { return left_max >= i; });
    return (left_max == split_num && it == r.end());
}

int MAX_N = 100000;

typedef enum STFunction {
  INSERT = 1,
  REMOVE = 2,
  LOOKUP = 3
} STFunction;


double time_st_function(SplayTree<int>& st, STFunction st_function, int arg) {
  if (st_function == INSERT) {
    auto start = std::chrono::high_resolution_clock::now();
    st.insert(arg);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
  } else if (st_function == REMOVE) {
    auto start = std::chrono::high_resolution_clock::now();
    st.remove(arg);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
  } else {
    auto start = std::chrono::high_resolution_clock::now();
    st.lookup(arg);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
  }
}

void time_test() {
  std::vector<int> elems(MAX_N);
  std::iota(elems.begin(), elems.end(), 0);

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(elems.begin(), elems.end(), g);
  SplayTree<int> st;
  std::ofstream csv;
  csv.open("balance.csv");
  csv << "N, Insert, Lookup, Remove" << std::endl;
  for (size_t i = 0; i < elems.size(); ++i) {
    csv << i << ",";
    csv << time_st_function(st, INSERT, elems[i]) << ",";
    csv << time_st_function(st, LOOKUP, elems[i]) << ",";
    csv << time_st_function(st, REMOVE, elems[i]) << std::endl;
    st.insert(elems[i]); // need to actually grow the st
  }
}
int main() {
    std::vector<int> elems(NUM_ELEMS);
    std::iota(elems.begin(), elems.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());

    SplayTree<int> splay_tree;
    std::set<int> s;
    bool i = insert_tests(splay_tree, s, elems, g);
    bool r = remove_tests(splay_tree, s, elems, g);
    bool j = join_tests(g);
    bool sp = split_tests(elems, g);
    std::cout << "Insertion tests: " << i << std::endl;
    std::cout << "Remove tests: " << r << std::endl;
    std::cout << "Join tests: " << j << std::endl;
    std::cout << "Split tests: " << sp << std::endl;

    time_test();
}
