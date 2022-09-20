#include "red_black_tree.h"
#include <limits>

using Tree = RedBlackTree<int>;
using Node = RedBlackTreeNode<int>;

void validate_rb_tree(Tree tree) {
    using Color = typename Node::Color;
    using SubBranch = typename Node::SubBranch;
    if (tree.empty()) {
        return;
    }

    bool status = true;

    auto pre = [&](RedBlackTreeNode<int> *curr)->void {
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
        return;
    };


    tree.root->tranverse(pre);
}

int main() {
  RedBlackTree<int> tree;

  for (;;) {
    int a;
    std::cin>>a;
    tree.insert(a);
    std::cout<<tree.black_height()<<std::endl;
  }
}