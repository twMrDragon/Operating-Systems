# 作業系統 hw3

## 資工三 111590012 林品緯

### 7.8

為了避免 deadlock 問題

### 8.20

|     |          |
| --- | -------- |
| (a) | safely   |
| (b) | unsafely |
| (c) | unsafely |
| (d) | safely   |
| (e) | unsafely |
| (f) | safely   |

### 8.28

#### (a)

Available:
$(2,2,2,4)$

Need 表:
| | A | B | C | D |
| ---- | --- | --- | --- | --- |
| P0 | 3 | 3 | 3 | 2 |
| P1 | 2 | 1 | 3 | 0 |
| P2 | 0 | 1 | 2 | 0 |
| P3 | 2 | 2 | 2 | 2 |
| P4 | 3 | 4 | 5 | 3 |

1. P2 可以，更新 available $(4,6,3,7)$
2. P0 可以，更新 available $(7,7,7,8)$
3. P1 可以，更新 available $(9,8,7,10)$
4. P3 可以，更新 available $(13,9,8,10)$
5. P4 可以，更新 available $(15,11,10,11)$

$P2 \rightarrow P0 \rightarrow P1 \rightarrow P3 \rightarrow P4$

系統處於安全狀態

#### (b)

Available 檢查:

$(2,2,2,4) \geq (2,2,1,2)$

P3 檢查

$(4,1,1,0)+(2,2,1,2) = (6,3,2,2)$

P3 Max:
$(6,3,3,2) \geq (6,3,2,2)$

更新 P3 Allocation: $(6,3,2,2)$

更新 Available: $(0,0,1,2)$

Need 表:
| | A | B | C | D |
| ---- | --- | --- | --- | --- |
| P0 | 3 | 3 | 3 | 2 |
| P1 | 2 | 1 | 3 | 0 |
| P2 | 0 | 1 | 2 | 0 |
| P3 | 0 | 0 | 1 | 0 |
| P4 | 3 | 4 | 5 | 3 |

1. P3 可以，更新 available $(6,3,3,4)$
2. P0 可以，更新 available $(9,4,7,5)$
3. P1 可以，更新 available $(11,5,7,7)$
4. P2 可以，更新 available $(13,9,8,10)$
5. P4 可以，更新 available $(15,11,10,11)$$

$P3 \rightarrow P0 \rightarrow P1 \rightarrow P2 \rightarrow P4$

可以批准，批准後系統還是處於安全狀態

### 8.30

```
mutex = 1
northbound = 0
southbound = 0

function north_farmer():
    wait(mutex)

    while (southbound > 0):
        signal(mutex)
        wait(northbound)
        wait(mutex)

    print("Northbound farmer is crossing the bridge")

    signal(mutex)

    signal(northbound)

function south_farmer():
    wait(mutex)

    while (northbound > 0):
        signal(mutex)
        wait(southbound)
        wait(mutex)

    print("Southbound farmer is crossing the bridge")

    signal(mutex)

    signal(southbound)
```

### 9.11

- Internal Fragmentation
  - 分配記憶體後，進程的記憶體大小小於分配到的記憶體大小鎖剩下的記憶體
  - 通常發生在固定記憶體大小分配中
- External Fragmentation
  - 記憶體被變成很多個小碎塊散落在不同地址，無法滿足單個進程的需求
  - 通常發生在動態記憶體大小分配中

### 9.13

#### First fit

|        | 選擇 | 剩下                                            |
| ------ | ---- | ----------------------------------------------- |
|        |      | {100 MB, 170 MB, 40 MB, 205 MB, 300 MB, 185 MB} |
| 200 MB | P4   | {100 MB, 170 MB, 40 MB, 5 MB, 300 MB, 185 MB}   |
| 15 MB  | P1   | {85 MB, 170 MB, 40 MB, 5 MB, 300 MB, 185 MB}    |
| 185 MB | P5   | {85 MB, 170 MB, 40 MB, 5 MB, 115 MB, 185 MB}    |
| 75 MB  | P1   | {15 MB, 170 MB, 40 MB, 5 MB, 115 MB, 185 MB}    |
| 175 MB | P6   | {15 MB, 170 MB, 40 MB, 5 MB, 115 MB, 10 MB}     |
| 80 MB  | P2   | {15 MB, 90 MB, 40 MB, 5 MB, 115 MB, 10 MB}      |

#### Best fit

|        | 選擇 | 剩下                                            |
| ------ | ---- | ----------------------------------------------- |
|        |      | {100 MB, 170 MB, 40 MB, 205 MB, 300 MB, 185 MB} |
| 200 MB | P4   | {100 MB, 170 MB, 40 MB, 5 MB, 300 MB, 185 MB}   |
| 15 MB  | P3   | {100 MB, 170 MB, 25 MB, 5 MB, 300 MB, 185 MB}   |
| 185 MB | P6   | {100 MB, 170 MB, 25 MB, 5 MB, 300 MB, 0 MB}     |
| 75 MB  | P1   | {25 MB, 170 MB, 25 MB, 5 MB, 300 MB, 0 MB}      |
| 175 MB | P5   | {25 MB, 170 MB, 25 MB, 5 MB, 125 MB, 0 MB}      |
| 80 MB  | P2   | {25 MB, 170 MB, 25 MB, 5 MB, 45 MB, 0 MB}       |

#### Worst fit

|        | 選擇 | 剩下                                            |
| ------ | ---- | ----------------------------------------------- |
|        |      | {100 MB, 170 MB, 40 MB, 205 MB, 300 MB, 185 MB} |
| 200 MB | P5   | {100 MB, 170 MB, 40 MB, 205 MB, 100 MB, 185 MB} |
| 15 MB  | P4   | {100 MB, 170 MB, 40 MB, 190 MB, 100 MB, 185 MB} |
| 185 MB | P4   | {100 MB, 170 MB, 40 MB, 5 MB, 100 MB, 185 MB}   |
| 75 MB  | P6   | {100 MB, 170 MB, 40 MB, 5 MB, 100 MB, 110 MB}   |
| 175 MB | No   | {100 MB, 170 MB, 40 MB, 5 MB, 100 MB, 110 MB}   |
| 80 MB  | P2   | {100 MB, 90 MB, 40 MB, 5 MB, 100 MB, 110 MB}    |

- First fit
  - 完成需求
  - 效率高，很快就可以找到位置，記憶體利用率不錯。
- Best fit
  - 完成需求
  - 效率較低，因為要遍歷全部的洞。並且會留下很多小的洞。
- Worst fit
  - 未完成需求 (175 MB 的進程沒有地方可放)
  - 效率較低，因為要遍歷全部的洞。並且會留下中等的洞。

### 9.24

$8$ K $=2^{13}$

#### (a)

$$
\begin{aligned}
2^{32}-2^{13} = 2^{19}
\end{aligned}
$$

$2^{19}=524,288$

#### (b)

$$
\begin{aligned}
2^{30}-2^{13} = 2^{17}
\end{aligned}
$$

$2^{17}=131,072$
