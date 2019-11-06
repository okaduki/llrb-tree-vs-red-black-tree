#pragma once

// ref https://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf

#include <functional>
#include <string>
#include "Tree.hpp"

template <typename K, typename V>
class LLRB : public TreeAbst<K, V> {
 private:
  struct Node {
    K key;
    V val;
    bool red;
    Node* left;
    Node* right;

    Node(const K& k, const V& v, bool red = true, Node* l = nullptr,
         Node* r = nullptr)
        : key(k), val(v), red(red), left(l), right(r) {}
    ~Node() {
      delete left;
      left = nullptr;
      delete right;
      right = nullptr;
    }
  };
  bool isRed(const Node* u) {
    return u ? u->red : false;  // all leaves (NIL) is black
  }

  bool isBlack(const Node* u) { return !isRed(u); }

  std::string dump_dot(std::function<std::string(const K&)> k2s,
                       std::function<std::string(const V&)> v2s, Node* root) {
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
      if (u->left) {
        res += e2s(u, u->left) + ";\n";
        dfs(u->left);
      }
      if (u->right) {
        res += e2s(u, u->right) + ";\n";
        dfs(u->right);
      }
    };

    if (root) dfs(root);

    res += "}\n";

    return res;
  }

  Node* root = nullptr;

  // precondition: h->right->red == true
  Node* rotateLeft(Node* u) {
    auto r = u->right;
    u->right = r->left;
    r->left = u;
    r->red = u->red;
    u->red = true;

    return r;
  }

  // precondition: h->left->red == true
  Node* rotateRight(Node* u) {
    auto l = u->left;
    u->left = l->right;
    l->right = u;
    l->red = u->red;
    u->red = true;

    return l;
  }

  void flipColors(Node* u) {
    u->red = !u->red;

    assert(u->left && u->right);
    u->left->red = !u->left->red;
    u->right->red = !u->right->red;
  }

  Node* insert(Node* u, const K& key, const V& val) {
    if (!u) return new Node(key, val);

    if (key < u->key)
      u->left = insert(u->left, key, val);
    else if (key > u->key)
      u->right = insert(u->right, key, val);
    else
      u->val = val;

    if (isBlack(u->left) && isRed(u->right)) u = rotateLeft(u);
    if (isRed(u->left) && isRed(u->left->left)) u = rotateRight(u);
    if (isRed(u->left) && isRed(u->right)) flipColors(u);

    return u;
  }

  Node* fixup(Node* u) {
    if (isRed(u->right)) u = rotateLeft(u);
    if (isRed(u->left) && isRed(u->left->left)) u = rotateRight(u);
    if (isRed(u->left) && isRed(u->right)) flipColors(u);

    return u;
  }

  // precondition:
  //   isRed(h) && isBlack(h->left) && isBlack(h->left->left)
  // postcondition:
  //   isRed(h->left) || isRed(h->left->left)
  Node* moveRedLeft(Node* h) {
    flipColors(h);
    if (h->right && isRed(h->right->left)) {
      h->right = rotateRight(h->right);
      h = rotateLeft(h);
      flipColors(h);
    }
    return h;
  }

  // precondition:
  //   isRed(h) && isBlack(h->right) && isBlack(h->right->left)
  // postcondition:
  //   isRed(h->right) || isRed(h->right->right)
  Node* moveRedRight(Node* h) {
    flipColors(h);
    if (h->left && isRed(h->left->left)) {
      h = rotateRight(h);
      flipColors(h);
    }
    return h;
  }

  Node* getmin(Node* h) {
    while (h->left) h = h->left;
    return h;
  }

  // invariant: isRed(h) || isRed(h->left)
  Node* deleteMin(Node* h) {
    if (h->left == nullptr) {
      delete h;
      return nullptr;
    }

    if (isBlack(h->left) && h->left && isBlack(h->left->left)) {
      // here: isRed(h) holds
      h = moveRedLeft(h);
    }
    h->left = deleteMin(h->left);
    return fixup(h);
  }

  // invariant: (isRed(h) || isRed(h->left) || isRed(h->right))
  Node* erase(Node* h, const K& key) {
    if (key < h->key) {  // left
      if (isRed(h->left) || isRed(h->left->left)) {
        // already invariant holds.
        h->left = erase(h->left, key);
      } else {
        // here: isRed(h) holds (induced by left lean)
        h = moveRedLeft(h);
        h->left = erase(h->left, key);
      }
    } else {  // go right or hit key
      if (isRed(h->left)) {
        h = rotateRight(h);
        // here: isRed(h->right) holds
      }
      // else isRed(h) holds

      if (key == h->key && h->right == nullptr) {
        delete h;
        return nullptr;
      }

      if (isBlack(h->right) && h->right && isBlack(h->right->left)) {
        // here: isRed(h) holds
        h = moveRedRight(h);
      }
      // here: isRed(h->right) || isRed(h->right->right) ||
      // isRed(h->right->left) holds

      if (key == h->key) {
        Node* succ = getmin(h->right);
        h->key = succ->key;
        h->val = succ->val;
        h->right = deleteMin(h->right);
      } else
        h->right = erase(h->right, key);
    }

    return fixup(h);
  }

  bool check_blackheight() {
    int c = 0;
    auto u = root;
    while (u != nullptr) {
      if (!u->red) ++c;
      u = u->left;
    }

    std::function<bool(Node*, int)> dfs = [&](Node* u, int c) {
      if (u == nullptr) return c == 0;
      if (!u->red) --c;
      return dfs(u->left, c) && dfs(u->right, c);
    };

    return dfs(root, c);
  }

 public:
  void insert(const K& key, const V& val) {
    root = insert(root, key, val);
    root->red = false;
    
#if ENABLE_TEST
    if(!check_blackheight()) {
      std::puts("insert failed");
      exit(0);
    }
#endif
  }

  void erase(const K& key) {
    root = erase(root, key);
    if (root) root->red = false;

#if ENABLE_TEST
    if(!check_blackheight()) {
      std::puts("erase failed");
      exit(0);
    }
#endif
  }

  bool find(const K& key, V& res) {
    Node* u = root;
    while (u) {
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