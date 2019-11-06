#pragma once

template <typename K, typename V>
struct TreeAbst {
  virtual void insert(const K& key, const V& val) = 0;
  virtual void erase(const K& key) = 0;
  virtual bool find(const K& key, V& res) = 0;
};
