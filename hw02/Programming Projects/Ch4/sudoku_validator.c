#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

int sudoku[SIZE][SIZE];
int results[11] = {0};

typedef struct {
    int startRow;
    int startCol;
    int index;
} parameters;

void* check_row(void* arg) {
    for (int i = 0; i < SIZE; i++) {
        int seen[SIZE + 1] = {0};
        for (int j = 0; j < SIZE; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                pthread_exit(NULL);
            }
            seen[num] = 1;
        }
    }
    results[0] = 1;
    pthread_exit(NULL);
}

void* check_column(void* arg) {
    for (int i = 0; i < SIZE; i++) {
        int seen[SIZE + 1] = {0};
        for (int j = 0; j < SIZE; j++) {
            int num = sudoku[j][i];
            if (num < 1 || num > 9 || seen[num]) {
                pthread_exit(NULL);
            }
            seen[num] = 1;
        }
    }
    results[1] = 1;
    pthread_exit(NULL);
}

void* check_subgrid(void* param) {
    parameters* p = (parameters*)param;
    int seen[SIZE + 1] = {0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = sudoku[p->startRow + i][p->startCol + j];
            if (num < 1 || num > 9 || seen[num]) {
                pthread_exit(NULL);
            }
            seen[num] = 1;
        }
    }
    results[p->index] = 1;
    pthread_exit(NULL);
}

int load_sudoku_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("無法開啟檔案");
        return 0;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (fscanf(file, "%d", &sudoku[i][j]) != 1) {
                printf("格式錯誤：第 %d 行第 %d 列\n", i + 1, j + 1);
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("使用方式：%s <sudoku_input_file>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];

    if (!load_sudoku_from_file(filename)) {
        return 1;
    }

    pthread_t threads[11];
    int threadIndex = 0;

    pthread_create(&threads[threadIndex++], NULL, check_row, NULL);
    pthread_create(&threads[threadIndex++], NULL, check_column, NULL);

    int subgridIndex = 0;
    for (int row = 0; row < SIZE; row += 3) {
        for (int col = 0; col < SIZE; col += 3) {
            parameters* data = (parameters*)malloc(sizeof(parameters));
            data->startRow = row;
            data->startCol = col;
            data->index = 2 + subgridIndex;
            pthread_create(&threads[threadIndex++], NULL, check_subgrid, data);
            subgridIndex++;
        }
    }

    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
    }

    int isValid = 1;
    for (int i = 0; i < 11; i++) {
        if (results[i] == 0) {
            isValid = 0;
            break;
        }
    }

    printf("Sudoku solution is %s!\n", isValid ? "valid" : "invalid");

    return 0;
}
