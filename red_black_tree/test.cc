#include "red_black_tree.h"

#include <cassert>
#include <cstdint>

#include <iostream>
#include <limits>
#include <random>
#include <set>
#include <string>

namespace algorithm {

using Tree = RedBlackTree<int>;
using Node = RedBlackTreeNode<int>;

void validate_rb_tree(Tree &tree) {
  using Color = typename Node::Color;
  using SubBranch = typename Node::SubBranch;
  if (tree.empty()) {
    return;
  }

  auto pre = [](RedBlackTreeNode<int> *curr) -> bool {
    if (curr->right && curr->left) {
      assert(curr->right->black_height() == curr->left->black_height());
    }
    if (curr->is_root()) {
      assert(curr->color == Color::black);
    } else {
      if (curr->branch() == SubBranch::left_branch) {
        assert(curr->val < curr->parent->val);
      } else {
        assert(curr->val >= curr->parent->val);
      }
      assert(curr == curr->parent->right || curr == curr->parent->left);
      assert(curr->color != Color::red || curr->parent->color != Color::red);
    }
    return true;
  };

  tree.root->tranverse(pre, Node::do_nothing_tranverse,
                       Node::do_nothing_tranverse);
}

void operator<<(std::ostream &out, const RedBlackTree<int> &tree) {
  std::vector<int> vi;
  auto print_space = [&] {
    for (auto i : vi) {
      out << std::string(i, ' ');
    }
  };
  auto pre = [&](Node *node) -> bool {
    int val = node->val;
    std::string node_print = std::to_string(val);

    print_space();
    if (node->color == Node::Color::red) {
      out << "\e[1;31m" << node_print << "\e[0m" << std::endl;
      ;
    } else {
      out << node_print << std::endl;
    }
    vi.push_back(node_print.size());

    return true;
  };
  auto post = [&](Node *node) -> bool {
    vi.pop_back();
    return true;
  };
  tree.root->tranverse(pre, Node::do_nothing_tranverse, post);
}
} // namespace algorithm

void random_insert_test() {
  std::uniform_int_distribution<std::uint_fast32_t> seed_generator;
  std::random_device rd;
  auto seed = seed_generator(rd);

  auto generator = std::mt19937(seed);
  std::uniform_int_distribution<int> dist;

  algorithm::RedBlackTree<int> tree;
  for (int i = 0; i <= 10000; i++) {
    tree.insert(dist(generator));
    if (i % 100 == 0)
      algorithm::validate_rb_tree(tree);
  }
  std::cout << __func__ << " completed." << std::endl;
}

void random_tree_search_test() {
  std::uniform_int_distribution<std::uint_fast32_t> seed_generator;
  std::random_device rd;
  auto seed = seed_generator(rd);

  auto generator = std::mt19937(seed);
  std::uniform_int_distribution<int> dist;

  algorithm::RedBlackTree<int> tree;
  std::set<int> set;
  for (int i = 0; i <= 10000; i++) {
    auto elm = dist(generator);
    tree.insert(elm);
    set.insert(elm);
  }
  for (auto elm : set) {
    auto result = tree.search(elm);
    assert(result != nullptr);
  }

  std::cout << __func__ << " compelted." << std::endl;
}

void random_tree_sequence_delete_test() {
  std::uniform_int_distribution<std::uint_fast32_t> seed_generator;
  std::random_device rd;
  auto seed = seed_generator(rd);

  auto generator = std::mt19937(seed);
  std::uniform_int_distribution<int> dist;

  algorithm::RedBlackTree<int> tree;
  std::set<int> set;
  for (int i = 0; i <= 10000; i++) {
    auto elm = dist(generator);
    if (set.find(elm) == set.end()) {
      tree.insert(elm);
      set.insert(elm);
    }
  }

  int i = 0;
  for (auto elm : set) {
    tree.del(elm);
    i++;
    if (i % 100 == 0) {
      algorithm::validate_rb_tree(tree);
    }
  }
  assert(tree.empty());
  std::cout << __func__ << " completed." <<std::endl;
}

void random_tree_delete_and_search_test() {
  std::uniform_int_distribution<std::uint_fast32_t> seed_generator;
  std::random_device rd;
  auto seed = seed_generator(rd);

  auto generator = std::mt19937(seed);
  std::uniform_int_distribution<int> dist;

  algorithm::RedBlackTree<int> tree;
  std::set<int> set;
  for (int i = 0; i <= 10000; i++) {
    auto elm = dist(generator);
    if (set.find(elm) == set.end()) {
      tree.insert(elm);
      set.insert(elm);
    }
  }

  enum op { op_delete = 0, op_do_nothing = 1 };
  std::uniform_int_distribution<int> op_dist(0, 1);
  auto new_set = std::set<int>(set);

  for (auto elm : set) {
    if (op_dist(generator) == op_delete && new_set.find(elm) != new_set.end()) {
      new_set.erase(elm);
      tree.del(elm);
    }
  }

  for (auto elm : set) {
    if (new_set.find(elm) == new_set.end()) {
      assert(tree.search(elm) == nullptr);
    } else {
      assert(tree.search(elm) != nullptr);
    }
  }
  std::cout<<__func__ << " completed." << std::endl;
}

void print_tree(const algorithm::Tree &tree) { std::cout << tree; }

void interact() {
  algorithm::Tree tree;
  for (;;) {
    int a;
    std::cin >> a;
    tree.insert(a);
    std::cout << tree;
    algorithm::validate_rb_tree(tree);
  }
}

int main() {
  random_insert_test();
  random_tree_search_test();
  random_tree_sequence_delete_test();
  random_tree_delete_and_search_test();
}