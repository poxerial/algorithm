#include "red_black_tree.h"
#include <limits>

namespace algorithm {

using Tree = RedBlackTree<int>;
using Node = RedBlackTreeNode<int>;

void validate_rb_tree(Tree &tree) {
    using Color = typename Node::Color;
    using SubBranch = typename Node::SubBranch;
    if (tree.empty()) {
        return;
    }

    bool status = true;

    auto pre = [&](RedBlackTreeNode<int> *curr)->bool{
        if (curr->is_root()) {
            assert(curr->color != Color::black);
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


    tree.root->tranverse(pre, Node::do_nothing_tranverse, Node::do_nothing_tranverse);
}

}

int main() {
  algorithm::RedBlackTree<int> tree;

    for (int i = 0; i < 7; i++) {
        tree.insert(i);
        std::cout<< i << "inserted." << std::endl;
    }
}