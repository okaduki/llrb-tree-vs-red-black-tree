#pragma once

#include <map>
#include "Tree.hpp"

template <typename K, typename V>
class Stdmap : public TreeAbst<K, V> {
 private:
  std::map<K, V> mp;

 public:
  void insert(const K& key, const V& val) { mp[key] = val; }

  void erase(const K& key) { mp.erase(key); }

  bool find(const K& key, V& res) {
    auto it = mp.find(key);
    if (it == std::end(mp)) return false;
    res = it->second;
    return true;
  }
};