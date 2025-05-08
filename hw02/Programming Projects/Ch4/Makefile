# Makefile

CC = gcc
CFLAGS = -lpthread
SRC = merge_sort_threaded.c sudoku_validator.c
OBJ = merge_sort_threaded.o sudoku_validator.o
EXEC = merge_sort_threaded sudoku_validator

# 預設目標
all: $(EXEC)

# 編譯 merge_sort_threaded.o
merge_sort_threaded.o: merge_sort_threaded.c
	$(CC) -c merge_sort_threaded.c -o merge_sort_threaded.o

# 編譯 sudoku_validator.o
sudoku_validator.o: sudoku_validator.c
	$(CC) -c sudoku_validator.c -o sudoku_validator.o

# 連結成執行檔
merge_sort_threaded: merge_sort_threaded.o
	$(CC) merge_sort_threaded.o -o merge_sort_threaded $(CFLAGS)

sudoku_validator: sudoku_validator.o
	$(CC) sudoku_validator.o -o sudoku_validator $(CFLAGS)

# 清除物件檔案與可執行檔案
clean:
	rm -f $(OBJ) $(EXEC)

# 用於顯示變數內容
print-%:
	@echo $* = $($*)
