#include <bits/stdc++.h>

namespace algorithm {

template <class T> class RedBlackTreeNode;
template <class T> class RedBlackTree;

template <class T> class RedBlackTreeNode {
  enum Color { red, black };
  enum SubBranch { left_branch, right_branch };
  RedBlackTreeNode *left, *right, *parent;
  Color color;
  T val;

  friend class RedBlackTree<T>;
  friend void validate_rb_tree(RedBlackTree<int> &tree);

  RedBlackTreeNode(T _val, RedBlackTreeNode *_parent)
      : val(_val), left(nullptr), right(nullptr), color(_parent ? red : black),
        parent(_parent) {}

  bool is_root() { return this->parent == nullptr; }
  bool is_leaf() { return this->right == nullptr && this->left == nullptr; }

  void leaf_insert(T _val, SubBranch b) {
    assert(this->sub_node(b) == nullptr);
    if (b == left_branch) {
      this->left = new RedBlackTreeNode(_val, this);
    } else {
      this->right = new RedBlackTreeNode(_val, this);
    }
  }

  void insert_right(T _val) { leaf_insert(_val, right_branch); }

  void insert_left(T _val) { leaf_insert(_val, left_branch); }

  RedBlackTreeNode *sub_node(SubBranch b) {
    return b == left_branch ? left : right;
  }

  static Color color_of(RedBlackTreeNode *ptr) {
    if (ptr) {
      return ptr->color;
    }
    return black;
  }

  SubBranch branch() {
    return this == this->parent->left ? left_branch : right_branch;
  }

  static SubBranch reverse_branch(SubBranch b) {
    return b == right_branch ? left_branch : right_branch;
  }

  RedBlackTreeNode *brother() {
    if (this->parent != nullptr) {
      return this->parent->left == this ? this->parent->right
                                        : this->parent->left;
    }
    return nullptr;
  }

  void rotate(SubBranch b) {
    auto rb = reverse_branch(b);
    auto node_new_parent = this->sub_node(rb);
    assert(node_new_parent != nullptr);

    // change the parent of related node
    auto node_1 = node_new_parent->sub_node(rb);
    if (node_1) {
      node_1->parent = this;
    }
    auto node_2 = this->sub_node(b);
    if (node_2) {
      node_2->parent = node_new_parent;
    }

    std::swap(this->val, node_new_parent->val);
    if (b == right_branch) {
      std::swap(this->right, node_new_parent->left);
    } else {
      std::swap(this->left, node_new_parent->right);
    }
    std::swap(node_new_parent->left, node_new_parent->right);
    std::swap(this->left, this->right);
  }

  void right_rotate() { rotate(right_branch); }

  void left_rotate() { rotate(left_branch); }

  RedBlackTreeNode *search(T val) {
    if (val == this->val)
      return this;
    else if (val < this->val)
      return this->left != nullptr ? this->left->search(val) : nullptr;
    else
      return this->right != nullptr ? this->right->search(val) : nullptr;
  }

  RedBlackTreeNode *min() {
    if (this->left == nullptr) {
      return this;
    } else {
      return this->left->min();
    }
  }

  RedBlackTreeNode *max() {
    if (this->right == nullptr) {
      return this;
    } else {
      return this->right->min();
    }
  }

  void insert(T _val) {
    RedBlackTreeNode *destination = this;
    for (;;) {
      auto sub_branch = _val < destination->val ? left_branch : right_branch;

      if (destination->sub_node(sub_branch) == nullptr) {
        destination->leaf_insert(_val, sub_branch);
        destination->sub_node(sub_branch)->insert_fixup();
        return;
      } else {
        destination = destination->sub_node(sub_branch);
      }
    }
  }

  void insert_fixup() {
    auto temp = this;
    while (color_of(temp->parent) == red) {
      RedBlackTreeNode *parent_inserted = temp->parent;
      RedBlackTreeNode *parent_bro = parent->brother();
      if (color_of(parent_bro) == red) {
        parent_inserted->color = black;
        parent_bro->color = black;
        parent_inserted->parent->color = red;
        temp = parent_inserted->parent;
      } else {
        if (parent_inserted->branch() != this->branch()) {
          temp->parent->rotate(reverse_branch(temp->branch()));
        }
        parent_inserted->parent->rotate(reverse_branch(temp->branch()));
        temp->parent->color = black;
        temp->brother()->color = red;
      }
    }
  }

  static constexpr auto do_nothing_tranverse =
      [](RedBlackTreeNode *node) -> bool { return true; };
  using tranversable_t = decltype(std::function(do_nothing_tranverse));

  void tranverse(tranversable_t pre_order, tranversable_t in_order,
                 tranversable_t post_order) {
    if (!pre_order(this)) {
      return;
    }

    if (this->left)
      this->left->tranverse(pre_order, in_order, post_order);

    if (!in_order(this)) {
      return;
    }

    if (this->right)
      this->right->tranverse(pre_order, in_order, post_order);

    if (!post_order(this)) {
      return;
    }
  }

  int black_height() {
    int bh = 0;
    tranverse(
        [&](RedBlackTreeNode *node) -> bool {
          if (node->color == black)
            bh++;
          if (node->is_leaf())
            return false;
          return true;
        },
        do_nothing_tranverse, do_nothing_tranverse);
    return bh;
  }
};

template <class T> class RedBlackTree {
  friend void validate_rb_tree(RedBlackTree<int>& tree);

  using Node = RedBlackTreeNode<T>;
  using Color = typename Node::Color;
  using SubBranch = typename Node::SubBranch;
  Node *root;

  void substitute(Node *old, Node *next) {
    if (next) {
      next->parent = old->parent;
    }
    if (old->is_root()) {
      root = next;
    } else {
      if (old->branch() == SubBranch::right_branch) {
        old->parent->right = next;
      } else {
        old->parent->left = next;
      }
    }
    if (old->color == Color::black) {
      del_fixup(next);
    }
    free(old);
  }

  void del_fixup(Node *node) {
    while (node->color == Color::black && node->parent != nullptr) {
      auto bro = node->brother();
      auto branch = node->branch();
      auto bro_branch = bro->branch();
      if (Node::color_of(bro) == Color::red) {
        node->parent->color = Color::red;
        bro->color = Color::black;
        node->parent->rotate(node->branch());
      }
      if (Node::color_of(bro->right) == Color::black &&
          Node::color_of(bro->left) == Color::black) {
        bro->color = Color::red;
        node = node->parent;
      } else {
        if (Node::color_of(bro->sub_node(
                Node::reverse_branch(node->branch()))) == Color::black) {
          bro->rotate(Node::reverse_branch(node->branch()));
        }
      }
    }
    node->color = Color::black;
  }

public:
  bool empty() { return root == nullptr; }

  int black_height() { return root ? root->black_height() : -1; }

  RedBlackTree() : root(nullptr) {}
  void del(Node *node) {
    if (!node) {
      return;
    }
    if (node->left == nullptr || node->right == nullptr) {
      substitute(node, node->left ? node->left : node->right);
    } else {
      auto nearest_node = node->right->min();
      std::swap(node->val, nearest_node->val);
      substitute(nearest_node, nearest_node->right);
    }
  }

  void insert(T val) {
    if (root) {
      root->insert(val);
    } else {
      root = new Node(val, nullptr);
    }
  }

  void tranverse(void (*preorder)(T), void (*inorder)(T),
                 void (*postorder)(T)) {
    auto temp = root;
    std::stack<Node *> s;
  }

  Node *search(T _val) { return root->search(_val); }
};

} // namespace algorithm