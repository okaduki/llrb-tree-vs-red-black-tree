#pragma once

// ref Introduction to Algorithms, 3rd edition, chapter 13

#include <functional>
#include <string>
#include "Tree.hpp"

template <typename K, typename V>
class RBTree : public TreeAbst<K, V> {
 private:
  enum class OP_BASE {
    Left,
    Right,
  };
  static constexpr OP_BASE op_rev(OP_BASE op) {
    return (op == OP_BASE::Left ? OP_BASE::Right : OP_BASE::Left);
  }

  struct Node {
    K key;
    V val;
    bool red;
    Node* par;
    Node* left;
    Node* right;

    Node(const K& k, const V& v, bool red = true, Node* par = nullptr,
         Node* l = nullptr, Node* r = nullptr)
        : key(k), val(v), red(red), par(par), left(l), right(r) {}
    Node() : red(false), par(nullptr), left(nullptr), right(nullptr) {}
    ~Node() {}

    template <OP_BASE op>
    Node*& get() {
      return (op == OP_BASE::Left ? left : right);
    }
  };
#define GET(l) template get<l>()

  Node* nil;
  Node* root;

  // auxiliary functions
  bool isRed(const Node* u) {
    return u ? u->red : false;  // all leaves (NIL) is black
  }

  bool isBlack(const Node* u) { return !isRed(u); }

  std::string dump_dot(std::function<std::string(const K&)> k2s,
                       std::function<std::string(const V&)> v2s, Node* tree) {
    auto n2s = [&](Node* n) {
      return "\"" + k2s(n->key) + ": " + v2s(n->val) + "\"";
    };
    auto e2s = [&](Node* from, Node* to) {
      return n2s(from) + " -> " + n2s(to) + (to->red ? "[color = red ]" : "");
    };
    std::string res;
    res += "digraph LLRB {\n";

    std::function<void(Node*)> dfs = [&](Node* u) {
      res += n2s(u) + ";\n";
      if (u->left != nil) {
        res += e2s(u, u->left) + ";\n";
        dfs(u->left);
      }
      if (u->right != nil) {
        res += e2s(u, u->right) + ";\n";
        dfs(u->right);
      }
    };

    if (tree != nil) dfs(tree);

    res += "}\n";

    return res;
  }

  //! link vertex u to v (v is possibly nullptr)
  template <OP_BASE left>
  void link(Node* u, Node* v) {
    if (left == OP_BASE::Left)
      u->left = v;
    else
      u->right = v;
    v->par = u;
  }

  template <OP_BASE left>
  Node* rotate(Node* x) {
    constexpr auto right = op_rev(left);

    auto y = x->GET(right);
    x->GET(right) = y->GET(left);
    if (y->left != nil) {
      y->GET(left)->par = x;
    }
    y->par = x->par;
    if (x->par == nil) {
      root = y;
    } else if (x == x->par->GET(left)) {
      x->par->GET(left) = y;
    } else {
      x->par->GET(right) = y;
    }
    y->GET(left) = x;
    x->par = y;

    return y;
  }

  // precondition: z->par == z->par->par->GET(left)
  template <OP_BASE left>
  Node* insert_fixup_loop(Node* z) {
    constexpr OP_BASE right = op_rev(left);

    auto y = z->par->par->GET(right);
    if (isRed(y)) {
      z->par->red = false;
      y->red = false;
      z->par->par->red = true;

      return z->par->par;
    } else {
      if (z == z->par->GET(right)) {
        z = z->par;
        rotate<left>(z);
      }

      auto pp = z->par->par;
      z->par->red = false;
      pp->red = true;
      rotate<right>(pp);
      return z;
    }
  }

  void insert_fixup(Node* z) {
    /**
     *  loop invariants:
     *    (a) isRed(z)
     *    (b) z.par == root => isBlack(z.par)
     *    (c) if tree violates rb-conditions, then
     *          z == root && isRed(z),
     *          exor isRed(z) && isRed(z.par)
     */
    while (isRed(z->par)) {
      if (z->par == z->par->par->left) {
        z = insert_fixup_loop<OP_BASE::Left>(z);
      } else {
        z = insert_fixup_loop<OP_BASE::Right>(z);
      }
    }
    root->red = false;
  }

  void insert_(const K& key, const V& val) {
    if (root == nil) {
      root = new Node(key, val, false, nil, nil, nil);
      return;
    }

    // Tree is not nil
    Node* y = nil;
    Node* x = root;
    while (x != nil) {
      y = x;
      if (key < x->key)
        x = x->left;
      else if (key > x->key)
        x = x->right;
      else
        break;
    }
    if (x == nil) {  // key is not found
      x = new Node(key, val, true, nil, nil, nil);

      if (key < y->key)
        link<OP_BASE::Left>(y, x);
      else
        link<OP_BASE::Right>(y, x);
    } else {  // key already exists
      x->val = val;
    }

    insert_fixup(x);
    root->red = false;
  }

  // y is possibly nullptr
  void transplant(Node* x, Node* y) {
    if (x->par == nil) {  // x is root
      root = y;
      y->par = nil;
    } else if (x == x->par->left) {
      link<OP_BASE::Left>(x->par, y);
    } else {
      link<OP_BASE::Right>(x->par, y);
    }
  }

  Node* get_min(Node* u) {
    while (u->left != nil) u = u->left;
    return u;
  }

  // precondition: x != nullptr && isBlack(x) && x == x->par->GET(left)
  template <OP_BASE left>
  Node* erase_fixup_loop(Node* x) {
    constexpr OP_BASE right = op_rev(left);
    auto x_par = x->par;
    auto w = x_par->GET(right);

    if (isRed(w)) {
      rotate<left>(x_par);
      w->red = false;
      x_par->red = true;
      w = x_par->GET(right);
    }
    if (isBlack(w->GET(left)) && isBlack(w->GET(right))) {
      w->red = true;
      return x_par;
    }
    if (isBlack(w->GET(right))) {
      rotate<right>(w);
      w->red = true;
      w = w->par;
      w->red = false;
    }
    auto xp_col = x_par->red;
    rotate<left>(x_par);
    x_par->red = false;
    w->GET(right)->red = false;
    w->red = xp_col;

    return root;
  }

  void erase_fixup(Node* x) {
    if (root == nil) return;
    /**
     *  loop invariants:
     *    tree violates only the rb-condition:
     *      x is black-black or red-black vertex (not black)
     */
    while (x != root && isBlack(x)) {
      if (x == x->par->left)
        x = erase_fixup_loop<OP_BASE::Left>(x);
      else
        x = erase_fixup_loop<OP_BASE::Right>(x);
    }
    x->red = false;
  }

  void erase_(const K& key) {
    if (root == nil) return;

    // Tree is not nil
    Node* x = root;
    while (x != nil) {
      if (key < x->key)
        x = x->left;
      else if (key > x->key)
        x = x->right;
      else
        break;
    }
    if (x == nil) {  // key is not found
      return;
    }
    if (x == root && x->left == nil && x->right == nil) {  // single node
      delete x;
      root = nil;
      return;
    }

    Node* will_remove = x;
    Node* y = x;
    auto original_y_red = y->red;
    if (x->left == nil) {
      transplant(x, x->right);
      x = x->right;
    } else if (x->right == nil) {
      transplant(x, x->left);
      x = x->left;
    } else {
      Node* z = will_remove;
      y = get_min(z->right);
      original_y_red = y->red;
      x = y->right;

      if (y->par == z) {
        x->par = y;
      } else {
        transplant(y, y->right);
        link<OP_BASE::Right>(y, z->right);
      }
      transplant(z, y);
      link<OP_BASE::Left>(y, z->left);
      y->red = z->red;
    }

    if (will_remove != nil) {
      will_remove->left = will_remove->right = nullptr;
      delete will_remove;
    }

    if (!original_y_red) {  // y is black
      erase_fixup(x);
    }
  }

  bool check_blackheight() {
    int c = 0;
    auto u = root;
    while (u != nil) {
      if (!u->red) ++c;
      u = u->left;
    }

    std::function<bool(Node*, int)> dfs = [&](Node* u, int c) {
      if (u == nil) return c == 0;
      if (!u->red) --c;
      return dfs(u->left, c) && dfs(u->right, c);
    };

    return dfs(root, c);
  }
#undef GET

 public:
  RBTree() {
    nil = new Node();
    nil->left = nil->right = nil->par = nil;
    root = nil;
  }
  ~RBTree() {
    std::stack<Node*> st;
    if (root != nil) st.push(root);
    while (!st.empty()) {
      auto u = st.top();
      st.pop();

      if (u->left != nil) st.push(u->left);
      if (u->right != nil) st.push(u->right);
      delete u;
    }
    delete nil;
  }

  void insert(const K& key, const V& val) {
    insert_(key, val);

#if ENABLE_TEST
    if (!check_blackheight()) {
      std::puts("insert failed");
      std::cerr << dump_dot([](const K& k) { return std::to_string((int)k); },
                            [](const V& k) { return std::to_string((int)k); },
                            root)
                << std::endl;
      exit(0);
    }
#endif
  }

  void erase(const K& key) {
    erase_(key);

#if ENABLE_TEST
    if (!check_blackheight()) {
      std::puts("erase failed");
      std::cerr << dump_dot([](const K& k) { return std::to_string((int)k); },
                            [](const V& k) { return std::to_string((int)k); },
                            root)
                << std::endl;
      exit(0);
    }
#endif
  }

  bool find(const K& key, V& res) {
    Node* u = root;
    while (u != nil) {
      if (key < u->key)
        u = u->left;
      else if (key > u->key)
        u = u->right;
      else {
        res = u->val;
        return true;
      }
    }
    return false;
  }

  std::string dump_dot(std::function<std::string(const K&)> k2s,
                       std::function<std::string(const V&)> v2s) {
    return dump_dot(k2s, v2s, root);
  }
};
