#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_BUFFER_SIZE 1024

#define ROW_MAX_SIZE 512
#define COL_MAX_SIZE 512

#define INPUT_FILE_PATH "input.txt"

void print_beam_matrix(char matrix[ROW_MAX_SIZE][COL_MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            putchar(matrix[i][j]);
        }
    }

    putchar('\n');
}

int main(){
    FILE *file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    char line[LINE_BUFFER_SIZE];
    char beam_matrix[ROW_MAX_SIZE][COL_MAX_SIZE] = {0};

    int row_count = 0;
    int col_count = 0;

    // Just copy the input file in the matrix for ease of use
    while (fgets(line, sizeof(line), file)) {
        for (int col = 0; col < strlen(line); col++)
            beam_matrix[row_count][col] = line[col];
        row_count++;
    }

    col_count = strlen(line);

    int split_count = 0;

    for (int row = 0; row < row_count; row++) {
        for (int col = 0; col < COL_MAX_SIZE; col++) {
            if (beam_matrix[row][col] == 'S')
                beam_matrix[row+1][col] = '|';
            else if (beam_matrix[row][col] == '|') {
                if (beam_matrix[row+1][col] == '^'){
                    beam_matrix[row+1][col-1] = '|';
                    beam_matrix[row+1][col+1] = '|';

                    split_count++;
                } 
                else {
                    beam_matrix[row+1][col] = '|';
                }
            }
        }
    }

    print_beam_matrix(beam_matrix, row_count, col_count);

    printf("Split count: %d\n", split_count);

    fclose(file);

    return 0;
}
