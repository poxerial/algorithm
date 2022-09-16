#include <bits/stdc++.h>
#include <vector>

template <class T> class RedBlackTree {
  enum Color { red, black };
  enum SubBranch { left_branch, right_branch };

  class RedBlackTreeNode {
    RedBlackTreeNode *left, *right, *parent;
    Color color;
    T val;

    RedBlackTreeNode(T _val, RedBlackTreeNode *_parent)
        : val(_val), left(nullptr), right(nullptr),
          color(_parent ? red : black), parent(_parent) {}

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

    Color color_of(RedBlackTreeNode *ptr) {
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

    template <RedBlackTreeNode *this_right, RedBlackTreeNode *this_left>
    void right_rotate_default() {
      assert(this_left != nullptr);

      std::swap(this->val, this_left->val);
      std::swap(this_left, this_right);
      std::swap(this_left, this_right->left);
      std::swap(this_right->left, this_right->right);
    }

    void right_rotate() { right_rotate_default<this->right, this->left>(); }

    void left_rotate() { right_rotate_default<this->left, this->right>(); }

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
            auto rotate =
                temp->branch() == left_branch ? right_rotate() : left_rotate();
            temp->parent->rotate();
          }
          auto rotate =
              temp->branch() == left_branch ? right_rotate() : left_rotate();
          temp->parent->parent->rotate();
          temp->parent->color = black;
          temp->brother()->color = red;
        }
      }
    }
  };
  RedBlackTreeNode *root;

  void substitute(RedBlackTreeNode *old, RedBlackTreeNode *next) {
    if (next) {
      next->parent = old->parent;
    }
    if (old->is_root()) {
      root = next;
    } else {
      if (old->branch() == right_branch) {
        old->parent->right = next;
      } else {
        old->parent->left = next;
      }
    }
    if (old->color == black) {
      del_fixup(next);
    }
    free(old);
  }

public:
  RedBlackTree() : root(nullptr) {}

  void del(RedBlackTreeNode *node) {
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
      root = new RedBlackTreeNode(val, nullptr);
    }
  }
};
