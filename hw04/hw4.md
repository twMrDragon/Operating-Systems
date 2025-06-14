# 作業系統 hw4

## 資工三 111590012 林品緯

### 10.21

設 page fault rate 為 $p$

$$
\begin{aligned}
EAT=&(1-p)\times 100+p\times \text{page fault time} \\
\text{page fault time} =& 0.7\times20000000+0.3\times 8000000 \\
=&14000000+2400000 \\
=& 16400000
\end{aligned}
$$

$$
\begin{aligned}
(1-p)\times 100+p\times 16400000\leq& 200 \\
100-100p+16400000p \leq& 200 \\
100+p\times(16400000-100)\leq&200 \\
16399900\times p\leq&100 \\
p\leq&\frac{100}{16399900} \\
p\approx&6.1\times 10^{-6}
\end{aligned}
$$

### 10.24

FIFO: 18 次 page fault

| page | frames  | is page fault |
| ---- | ------- | ------------- |
| 2    | 2,\_,\_ | yes           |
| 6    | 2,6,\_  | yes           |
| 9    | 2,6,9   | yes           |
| 2    | 2,6,9   |               |
| 4    | 6,9,4   | yes           |
| 2    | 9,4,2   | yes           |
| 1    | 4,2,1   | yes           |
| 7    | 2,1,7   | yes           |
| 3    | 1,7,3   | yes           |
| 0    | 7,3,0   | yes           |
| 5    | 3,0,5   | yes           |
| 2    | 0,5,2   | yes           |
| 1    | 5,2,1   | yes           |
| 2    | 5,2,1   |               |
| 9    | 2,1,9   | yes           |
| 5    | 1,9,5   | yes           |
| 7    | 9,5,7   | yes           |
| 3    | 5,7,3   | yes           |
| 8    | 7,3,8   | yes           |
| 5    | 3,8,5   | yes           |

LRU: 17 次 page fault

| page | frames  | is page fault |
| ---- | ------- | ------------- |
| 2    | 2,\_,\_ | yes           |
| 6    | 2,6,\_  | yes           |
| 9    | 2,6,9   | yes           |
| 2    | 2,6,9   |               |
| 4    | 2,4,9   | yes           |
| 2    | 2,4,9   |               |
| 1    | 2,4,1   | yes           |
| 7    | 2,7,1   | yes           |
| 3    | 3,7,1   | yes           |
| 0    | 3,7,0   | yes           |
| 5    | 3,5,0   | yes           |
| 2    | 2,5,0   | yes           |
| 1    | 2,5,1   | yes           |
| 2    | 2,5,1   |               |
| 9    | 2,9,1   | yes           |
| 5    | 2,9,5   | yes           |
| 7    | 7,9,5   | yes           |
| 3    | 7,3,5   | yes           |
| 8    | 7,3,8   | yes           |
| 5    | 5,3,8   | yes           |

Optimal: 13 次 page fault

| page | frames  | is page fault |
| ---- | ------- | ------------- |
| 2    | 2,\_,\_ | yes           |
| 6    | 2,6,\_  | yes           |
| 9    | 2,6,9   | yes           |
| 2    | 2,6,9   |               |
| 4    | 2,4,9   | yes           |
| 2    | 2,4,9   |               |
| 1    | 2,1,9   | yes           |
| 7    | 2,1,7   | yes           |
| 3    | 2,1,3   | yes           |
| 0    | 2,1,0   | yes           |
| 5    | 2,1,5   | yes           |
| 2    | 2,1,5   |               |
| 1    | 2,1,5   |               |
| 2    | 2,1,5   |               |
| 9    | 2,9,5   | yes           |
| 5    | 2,9,5   |               |
| 7    | 7,9,5   | yes           |
| 3    | 7,3,5   | yes           |
| 8    | 8,3,5   | yes           |
| 5    | 8,3,5   |               |

### 10.37

原因:

系統花過多時間在處理 page fault，而不是執行實際命令。

如何偵測:

CPU 使用率下降且 page fault 率上升。

解決方法:

暫停部分程序，讓剩下的程序有足夠的記憶體。

### 11.13

FCFS: 13011

| order | acc   |
| ----- | ----- |
| 2150  | 0     |
| 2069  | 81    |
| 1212  | 938   |
| 2296  | 2022  |
| 2800  | 2526  |
| 544   | 4782  |
| 1618  | 5856  |
| 356   | 7118  |
| 1523  | 8285  |
| 4965  | 11727 |
| 3681  | 13011 |

SCAN: 7492

| order | acc  |
| ----- | ---- |
| 2150  | 0    |
| 2296  | 146  |
| 2800  | 650  |
| 3681  | 1531 |
| 4965  | 2815 |
| 4999  | 2849 |
| 2069  | 5779 |
| 1618  | 6230 |
| 1523  | 6325 |
| 1212  | 6636 |
| 544   | 7304 |
| 356   | 7492 |

C-SCAN: 9917

| order | acc  |
| ----- | ---- |
| 2150  | 0    |
| 2296  | 146  |
| 2800  | 650  |
| 3681  | 1531 |
| 4965  | 2815 |
| 4999  | 2849 |
| 0     | 7848 |
| 356   | 8204 |
| 544   | 8392 |
| 1212  | 9060 |
| 1523  | 9371 |
| 1618  | 9466 |
| 2069  | 9917 |

### 11.17

- caching
  - 優點
    - 常用資料可以從 NVM 快取中快速存取，減少等待 I/O 的時間
  - 缺點
    - 成本高，價格比 HDD 昂貴
    - NVM 有寫入次數限制
- disk-drive replacement
  - 優點
    - NVM 有更快的讀取寫入速度，和較低的功耗
  - 缺點
    - 成本高，價格比 HDD 昂貴
    - NVM 有寫入次數限制

### 11.21

(a)

RAID 1 吞吐量比較大

(b)

RAID 5 吞吐量比較大

### 14.14

(a)

Contiguous:

- 檔案在磁碟上的實體區塊是連續的，假設起始實體區塊號為 `b`
- 則邏輯區塊 `i` 的實體區塊號為： `physical_block = b + i`

Linked:

- 每個區塊儲存「下一個區塊的指標」
- 檔案資訊中只儲存「第一個區塊位置」
- 若要找第 i 個邏輯區塊 → 需從第一個實體區塊開始，一個一個跳到第 i 個

Indexed:

- 每個檔案有一個索引區塊（index block），儲存檔案所有區塊的實體位址
- 存取邏輯區塊 `i`：直接查詢 index block 的第 `i` 個條目即可

(b)

Contiguous:

- 因為區塊連續，可以直接計算 b + 4

讀取 1 個實體區塊

Linked:

$0\rightarrow 1 \rightarrow 2 \rightarrow 3 \rightarrow 4$

讀取 5 個實體區塊

Indexed:

先讀取 index block，再透過 index block 找到 block 4

讀取 2 個實體區塊

### 14.15

$\frac{8KB}{4B} = 2048$ 個指標

$12\times 8KB = 96KB$

single:

$2048\times8KB = 16MB$

double:

$2048^2\times8KB = 32GB$

triple:

$2048^3\times8KB = 64TB$

total:

$64TB+32GB+16MB+96KB\approx64.03TB$
