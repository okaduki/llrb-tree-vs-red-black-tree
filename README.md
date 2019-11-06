# Left-leaning red-black tree (LLRB) vs red-black tree (RB)

## References

* LLRB
  * http://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf
  * http://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf
* RB
  * Introduction to Algorithms, 3rd Edition

## 性能測定方法

* n = {10,100,...} 個のデータ (key,value: int) を乱数で生成
* 以下の手順でかかった時間をそれぞれ計測
  * n 個のデータを順にinsert
  * n 個のデータを順にfind (このため探索は常に成功する)
  * n 個のデータを挿入した順序とは別の無関係な順序で削除
* これを同じデータセットに対して10 回行い、中央値と平均時間を計測
* 対象は `std::map, LLRB-tree, RB-tree`

## 測定結果


| n        | algorithm | insert  [us] | find        [us] | delete [us]  |
| -------- | --------- | ------------ | ---------------- | ------------ |
| 100      | map       | 5.000        | 2.000            | 4.000        |
|          | LLRB      | 4.000        | 2.000            | 6.000        |
|          | RB        | 6.000        | 3.000            | 4.000        |
| 1000     | map       | 80.000       | 50.000           | 82.000       |
|          | LLRB      | 94.000       | 46.000           | 142.000      |
|          | RB        | 71.000       | 45.000           | 69.000       |
| 10000    | map       | 1298.000     | 1028.000         | 1359.000     |
|          | LLRB      | 1438.000     | 889.000          | 2233.000     |
|          | RB        | 1104.000     | 747.000          | 1177.000     |
| 100000   | map       | 25766.000    | 23170.000        | 25985.000    |
|          | LLRB      | 26909.000    | 19160.000        | 38273.000    |
|          | RB        | 22649.000    | 18668.000        | 22177.000    |
| 1000000  | map       | 668816.000   | 654584.000       | 703955.000   |
|          | LLRB      | 784830.000   | 595054.000       | 1002592.000  |
|          | RB        | 513800.000   | 454942.000       | 636762.000   |
| 10000000 | map       | 11503619.000 | 11160440.000     | 11988701.000 |
|          | LLRB      | 13958427.000 | 10827408.000     | 17078928.000 |
|          | RB        | 9168139.000  | 8429294.000      | 10743120.000 |

raw output

```
n = 100
std::map
median [us] : insert = 5.000 (0.050 per item)
              find   = 2.000 (0.020 per item)
              delete = 4.000 (0.040 per item)
avg    [us] : insert = 5.800 (0.058 per item)
              find   = 2.500 (0.025 per item)
              delete = 5.000 (0.050 per item)
LLRB
median [us] : insert = 4.000 (0.040 per item)
              find   = 2.000 (0.020 per item)
              delete = 6.000 (0.060 per item)
avg    [us] : insert = 5.400 (0.054 per item)
              find   = 2.400 (0.024 per item)
              delete = 7.400 (0.074 per item)
RBTree
median [us] : insert = 6.000 (0.060 per item)
              find   = 3.000 (0.030 per item)
              delete = 4.000 (0.040 per item)
avg    [us] : insert = 6.000 (0.060 per item)
              find   = 2.600 (0.026 per item)
              delete = 4.400 (0.044 per item)
n = 1000
std::map
median [us] : insert = 80.000 (0.080 per item)
              find   = 50.000 (0.050 per item)
              delete = 82.000 (0.082 per item)
avg    [us] : insert = 85.100 (0.085 per item)
              find   = 50.200 (0.050 per item)
              delete = 83.100 (0.083 per item)
LLRB
median [us] : insert = 94.000 (0.094 per item)
              find   = 46.000 (0.046 per item)
              delete = 142.000 (0.142 per item)
avg    [us] : insert = 95.800 (0.096 per item)
              find   = 49.000 (0.049 per item)
              delete = 154.400 (0.154 per item)
RBTree
median [us] : insert = 71.000 (0.071 per item)
              find   = 45.000 (0.045 per item)
              delete = 69.000 (0.069 per item)
avg    [us] : insert = 71.400 (0.071 per item)
              find   = 45.300 (0.045 per item)
              delete = 69.500 (0.070 per item)
n = 10000
std::map
median [us] : insert = 1298.000 (0.130 per item)
              find   = 1028.000 (0.103 per item)
              delete = 1359.000 (0.136 per item)
avg    [us] : insert = 1362.800 (0.136 per item)
              find   = 1047.900 (0.105 per item)
              delete = 1419.000 (0.142 per item)
LLRB
median [us] : insert = 1438.000 (0.144 per item)
              find   = 889.000 (0.089 per item)
              delete = 2233.000 (0.223 per item)
avg    [us] : insert = 1486.300 (0.149 per item)
              find   = 884.400 (0.088 per item)
              delete = 2266.800 (0.227 per item)
RBTree
median [us] : insert = 1104.000 (0.110 per item)
              find   = 747.000 (0.075 per item)
              delete = 1177.000 (0.118 per item)
avg    [us] : insert = 1156.500 (0.116 per item)
              find   = 752.900 (0.075 per item)
              delete = 1206.000 (0.121 per item)
n = 100000
std::map
median [us] : insert = 25766.000 (0.258 per item)
              find   = 23170.000 (0.232 per item)
              delete = 25985.000 (0.260 per item)
avg    [us] : insert = 25070.100 (0.251 per item)
              find   = 23474.900 (0.235 per item)
              delete = 26698.700 (0.267 per item)
LLRB
median [us] : insert = 26909.000 (0.269 per item)
              find   = 19160.000 (0.192 per item)
              delete = 38273.000 (0.383 per item)
avg    [us] : insert = 27951.800 (0.280 per item)
              find   = 18948.500 (0.189 per item)
              delete = 38225.300 (0.382 per item)
RBTree
median [us] : insert = 22649.000 (0.226 per item)
              find   = 18668.000 (0.187 per item)
              delete = 22177.000 (0.222 per item)
avg    [us] : insert = 22714.400 (0.227 per item)
              find   = 18653.800 (0.187 per item)
              delete = 22253.100 (0.223 per item)
n = 1000000
std::map
median [us] : insert = 668816.000 (0.669 per item)
              find   = 654584.000 (0.655 per item)
              delete = 703955.000 (0.704 per item)
avg    [us] : insert = 661276.700 (0.661 per item)
              find   = 636703.400 (0.637 per item)
              delete = 697243.500 (0.697 per item)
LLRB
median [us] : insert = 784830.000 (0.785 per item)
              find   = 595054.000 (0.595 per item)
              delete = 1002592.000 (1.003 per item)
avg    [us] : insert = 800449.300 (0.800 per item)
              find   = 620157.000 (0.620 per item)
              delete = 1012310.500 (1.012 per item)
RBTree
median [us] : insert = 513800.000 (0.514 per item)
              find   = 454942.000 (0.455 per item)
              delete = 636762.000 (0.637 per item)
avg    [us] : insert = 520140.700 (0.520 per item)
              find   = 454589.100 (0.455 per item)
              delete = 645545.800 (0.646 per item)
n = 10000000
std::map
median [us] : insert = 11503619.000 (1.150 per item)
              find   = 11160440.000 (1.116 per item)
              delete = 11988701.000 (1.199 per item)
avg    [us] : insert = 11393522.400 (1.139 per item)
              find   = 10888870.400 (1.089 per item)
              delete = 11922033.600 (1.192 per item)
LLRB
median [us] : insert = 13958427.000 (1.396 per item)
              find   = 10827408.000 (1.083 per item)
              delete = 17078928.000 (1.708 per item)
avg    [us] : insert = 13754955.200 (1.375 per item)
              find   = 10584920.900 (1.058 per item)
              delete = 17018014.600 (1.702 per item)
RBTree
median [us] : insert = 9168139.000 (0.917 per item)
              find   = 8429294.000 (0.843 per item)
              delete = 10743120.000 (1.074 per item)
avg    [us] : insert = 9149798.200 (0.915 per item)
              find   = 8304497.800 (0.830 per item)
              delete = 10745559.700 (1.075 per item)
```

## 雑な考察など

* http://read.seas.harvard.edu/~kohler/notes/llrb.html にも書いてあるが、LLRBは回転が起きる回数が定数回ではない(?)ため delete が特に遅いらしい
  * 実際 `n=1000000` 個のデータを挿入したあとにdelete すると1回のdelete で多いときで回転が30回ほど行われた
* `std::map` よりRB が少しだけ早いのは単に定数部分の問題な気がする
