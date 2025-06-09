# Programming Project

## Unit 7

> [!NOTE]  
> 在本次 `Unit 7` 的問題中有諸多問題  
> 我們選擇實作 `生產者消費者問題 (Producer Consumer Problem)` 以及 `哲學家用餐問題 (Dining Philosophers Problem)`  
> 分別是 `prodcons.c` 以及 `dining.c`  
> 由於原始題目中並沒有特別指明使用次數，因此都是無限次循環  
> 但在 `生產者消費者問題 (Producer Consumer Problem)` 中我們有特別做處理，避免結束之後 `Thread` 卡死在 `sem_wait` 無法結束

### How to Compile

```
gcc -pthread -o dining dining.c
gcc -pthread -o prodcons prodcons.c
```

### How to use

```
./dining
./prodcons
```

### Picture

> 以下會附上程式執行畫面截圖，包含：
>
> - 生產者消費者結果
> - 哲學家用餐過程模擬

### 生產者消費者問題

![CH7_IMG1](./Ch7/product_exe.png)
![CH7_IMG2](./Ch7/product_res.png)

> [!IMPORTANT]  
> 結果保留在 `pro_res.txt` 內，因為輸出內容太多所以截圖把他導出到這裡  
> Producer 1 produced: 42 Producer ，代表執行緒 #1 產生了數字 42 並成功放入 buffer  
> Consumer 2 consumed: 42 Consumer ，代表執行緒 #2 成功從 buffer 中取出並處理了數字 42

### 哲學家用餐問題

![CH7_IMG3](./Ch7/dining_exe.png)
![CH7_IMG4](./Ch7/dining_res.png)

> [!IMPORTANT]  
> 結果保留在 `din_res.txt` 內，因為輸出內容太多所以截圖把他導出到這裡  
> Philosopher 0: EATING 🍽️ ，代表哲學家在吃飯  
> Philosopher 1: THINKING 🤔 ，代表哲學家在思考  
> Philosopher 2: HUNGRY 🙏 ，代表哲學家在飢餓(有可能筷子被占用沒辦法吃飯，所以很餓)

## Unit 8&9

> [!NOTE]  
> 在 `Unit 8&9` 的實作 `Project` 裡面，我們選擇兩個都做。
>
> - Banker’s Algorithm
>   - 銀行家演算法
> - Contiguous Memory Allocation
>   - 連續記憶體分配

### How to Compile

> [!NOTE]
>
> 請使用 `make` 指令自動編譯本專案中包含的程式碼：
>
> - `banker.c`：實作銀行家演算法（Banker's Algorithm），使用多執行緒模擬多位客戶資源請求與釋放過程。
> - `memory_manager.c`：模擬動態記憶體管理，包括分配、釋放與記憶體壓縮等操作。
>
> 請確認你的作業系統為 Linux，並且有安裝 GCC 編譯器與 POSIX Thread 函式庫。

### How to use

#### banker

```bash
# 執行 banker（需輸入每種資源的總數）
./banker 10 5 7
```

接著會模擬顧客隨機向系統請求資源

#### memory_manager

```bash
# 執行 memory_manager（需輸入初始記憶體大小）
./memory_manager 100
```

接著程式會印出選單，等待使用者輸入指令進行操作。

### Picture

#### Banker’s Algorithm

![baner](./Ch8&9/banker.png)

#### Contiguous Memory Allocation

![memory_manager](./Ch8&9/memory_manager.png)

# 組員名單及貢獻度

- 111590004 張意昌，進行 `Ch7 Producer Consumer Problem` 程式撰寫。[25%]
- 111590011 吳耀東，進行 `Ch8 Banker’s Algorithm` 撰寫程式和使用文件。[25%]
- 111590012 林品緯，進行 `Ch9 Contiguous Memory Allocation` 撰寫程式和使用文件。[25%]
- 111590028 張睿恩，進行 `Ch7 Dining Philosophers Problem` 程式撰寫。[25%]
- 每人貢獻皆為 `25%` ，總共為 `100%` 。
