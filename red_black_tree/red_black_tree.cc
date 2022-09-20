#include <bits/stdc++.h>

template <class T> class RedBlackTreeNode;
template <class T> class RedBlackTree;

template <class T> class RedBlackTreeNode {
  enum Color { red, black };
  enum SubBranch { left_branch, right_branch };
  RedBlackTreeNode *left, *right, *parent;
  Color color;
  T val;

  friend class RedBlackTree<T>;

  RedBlackTreeNode(T _val, RedBlackTreeNode *_parent)
      : val(_val), left(nullptr), right(nullptr), color(_parent ? red : black),
        parent(_parent) {}

  bool is_root() { return this->parent == nullptr; }

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

  RedBlackTreeNode *brother() {
    if (this->parent != nullptr) {
      return this->parent->left == this ? this->parent->right
                                        : this->parent->left;
    }
    return nullptr;
  }

  void right_rotate_default(RedBlackTreeNode *this_right,
                            RedBlackTreeNode *this_left) {
    assert(this_left != nullptr);

    std::swap(this->val, this_left->val);
    std::swap(this_left, this_right);
    std::swap(this_left, this_right->left);
    std::swap(this_right->left, this_right->right);
  }

  void right_rotate() { right_rotate_default(this->right, this->left); }

  void left_rotate() { right_rotate_default(this->left, this->right); }

  void rotate(SubBranch b) {
    if (b == right_branch) {
      right_rotate();
    } else {
      left_rotate();
    }
  }

  /**
   * @brief Return the pointer to the first element equal to `val`.
   * If there is no such element in the subtree, return `nullptr`.
   *
   * @param val
   * @return T*
   */
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
        destination = destination->left;
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
        if (parent_inserted->branch() != parent_inserted->parent->branch()) {
          temp->parent->rotate(temp->branch() == right_branch ? left_branch
                                                              : right_branch);
        }
        temp->parent->rotate(temp->branch() == right_branch ? left_branch
                                                            : right_branch);
        temp->parent->color = black;
        temp->brother()->color = red;
      }
    }
  }
};

template <class T> class RedBlackTree {

public:
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
        if (Node::color_of(bro->sub_branch()))
      }
        
      }
    node->color = Color::black;
    }

public:
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

  Node *search(T _val) { return root->search(_val); }
};

int main() {
  RedBlackTree<int> tree;
  tree.insert(3);
  tree.del(tree.search(3));
}