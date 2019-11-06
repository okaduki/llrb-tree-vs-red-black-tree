#include <bits/stdc++.h>
#define ENABLE_TEST 0

#include "stdmap.hpp"
#include "LLRB.hpp"
#include "RBTree.hpp"

using namespace std;

/**
 * measure functions
 */

template<typename T, typename K, typename V>
tuple<double,double,double> run(int n, const vector<pair<K,V>>& items, const vector<pair<K,V>>& eraselist) {
  T tree;

  auto start = chrono::steady_clock::now();

  for(auto& item : items) {
    tree.insert(item.first, item.second);
  }

  auto stop1 = chrono::steady_clock::now();

  bool ok = true;
  for(const auto& item : items) {
    V v = 0;
    bool found = tree.find(item.first, v);
    if(!found || v != item.second){
      ok = false;
    }
  }

  auto stop2 = chrono::steady_clock::now();

  for(const auto& item : eraselist) {
    tree.erase(item.first);
  }

  auto stop3 = chrono::steady_clock::now();

  auto elapsed1 = chrono::duration_cast<chrono::microseconds>(stop1 - start).count();
  auto elapsed2 = chrono::duration_cast<chrono::microseconds>(stop2 - stop1).count();
  auto elapsed3 = chrono::duration_cast<chrono::microseconds>(stop3 - stop2).count();

  if(!ok){
    cout << "failed" << endl;
  }
  return make_tuple(elapsed1, elapsed2, elapsed3);
}

template<template<typename,typename> typename T>
void measure(string name, int n, int try_num){
  using DTYPE = int;
  using Tree = T<DTYPE,DTYPE>;

  mt19937 mt(123);
  vector<pair<DTYPE,DTYPE>> items(n);
  unordered_set<DTYPE> memo;
  for(int i=0;i<n;++i){
    DTYPE key = mt();

    if(memo.count(key)) --i;
    else{
      memo.insert(key);
      items[i] = make_pair(key, mt());
    }
  }

  auto eraselist = items;
  shuffle(begin(eraselist), end(eraselist), mt);

  vector<double> time_ins, time_fnd, time_del;
  for(int i=0;i<try_num;++i){
    auto time = run<Tree>(n, items, eraselist);
    time_ins.push_back(get<0>(time));
    time_fnd.push_back(get<1>(time));
    time_del.push_back(get<2>(time));
  }
  sort(begin(time_ins), end(time_ins));
  sort(begin(time_fnd), end(time_fnd));
  sort(begin(time_del), end(time_del));

  double avg_ins = accumulate(begin(time_ins), end(time_ins), 0.) * 1. / try_num;
  double avg_fnd = accumulate(begin(time_fnd), end(time_fnd), 0.) * 1. / try_num;
  double avg_del = accumulate(begin(time_del), end(time_del), 0.) * 1. / try_num;

  cout << name << endl
       << fixed << setprecision(3)
       << "median [us] : " << "insert = " << time_ins[try_num / 2] << " (" << time_ins[try_num / 2] * 1. / n << " per item)" << endl
       << "              " << "find   = " << time_fnd[try_num / 2] << " (" << time_fnd[try_num / 2] * 1. / n << " per item)" << endl
       << "              " << "delete = " << time_del[try_num / 2] << " (" << time_del[try_num / 2] * 1. / n << " per item)" << endl
       << "avg    [us] : " << "insert = " << avg_ins << " (" << avg_ins * 1. / n << " per item)" << endl
       << "              " << "find   = " << avg_fnd << " (" << avg_fnd * 1. / n << " per item)" << endl
       << "              " << "delete = " << avg_del << " (" << avg_del * 1. / n << " per item)" << endl;
}

template<template<typename,typename> typename T>
bool check(int n){
  using DTYPE = int;

  T<DTYPE,DTYPE> tree;
  mt19937 mt(123);
  vector<pair<DTYPE,DTYPE>> items(n);
  unordered_set<DTYPE> memo;
  for(int i=0;i<n;++i){
    DTYPE key = mt();

    if(memo.count(key)) --i;
    else{
      memo.insert(key);
      items[i] = make_pair(key, mt());
    }
  }

  for(const auto& item : items) {
    tree.insert(item.first, item.second);
  }

  shuffle(begin(items), end(items), mt);

  for(const auto& item: items) {
    DTYPE v;
    bool found = tree.find(item.first, v);
    if(!found || v != item.second) return false;
    
    tree.erase(item.first);
    if(tree.find(item.first, v)) return false;
  }
  return true;
}

int main() {
#if ENABLE_TEST
  {
    cout << "test Stdmap ..." << endl;
    if(!check<Stdmap>(12345)) cout << "Stdmap failed" << endl;
    cout << "test LLRB ..." << endl;
    if(!check<LLRB>(12345)) cout << "LLRB failed" << endl;
    cout << "test RBTree ..." << endl;
    if(!check<RBTree>(12345)) cout << "LLRB failed" << endl;
    return 0;
  }
#endif

  constexpr int TRY_NUM = 10;

  vector<int> sizes = { 100, 1000, 10000, 100000, 1000000, 10000000 };
  // vector<int> sizes = { 1000 };
  sort(begin(sizes), end(sizes));

  for(auto n : sizes) {
    cout << "n = " << n << endl;

    {
      string name = "std::map";
      measure<Stdmap>(name, n, TRY_NUM);
    }
    {
      string name = "LLRB";
      measure<LLRB>(name, n, TRY_NUM);
    }
    {
      string name = "RBTree";
      measure<RBTree>(name, n, TRY_NUM);
    }
  }

  return 0;
}