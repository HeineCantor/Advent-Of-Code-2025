#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_BUFFER_SIZE 1024

#define ROW_MAX_SIZE 512
#define COL_MAX_SIZE 512

#define START_INT -3
#define SPLIT_INT -2
#define BEAM_INT -1
#define NULL_INT 0

#define INPUT_FILE_PATH "input.txt"

void print_beam_matrix(long matrix[ROW_MAX_SIZE][COL_MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(matrix[i][j] < 0)
                printf("\\ ");
            else
            printf("%ld ", matrix[i][j]);
        }
        putchar('\n');
    }

    putchar('\n');
}

int get_int_from_char(char c) {
    switch (c) {
        case 'S':
            return START_INT;
        case '^':
            return SPLIT_INT;
        case '|':
            return BEAM_INT;
        default:
            return NULL_INT;
    }
}

int main(){
    FILE *file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    char line[LINE_BUFFER_SIZE];
    long beam_matrix[ROW_MAX_SIZE][COL_MAX_SIZE] = {0};

    int row_count = 0;
    int col_count = 0;


    while (fgets(line, sizeof(line), file)) {
        for (int col = 0; col < strlen(line); col++)
            beam_matrix[row_count][col] = get_int_from_char(line[col]);
        row_count++;
    }

    col_count = strlen(line);

    long paths_count = 0;

    for (int row = 0; row < row_count; row++) {
        for (int col = 0; col < COL_MAX_SIZE; col++) {
            if (beam_matrix[row][col] == START_INT)
                beam_matrix[row+1][col] = 1;
            else if (beam_matrix[row][col] > 0) { // There is at least one path here
                if (beam_matrix[row+1][col] == SPLIT_INT){
                    beam_matrix[row+1][col-1] += beam_matrix[row][col];
                    beam_matrix[row+1][col+1] += beam_matrix[row][col];
                } 
                else {
                    beam_matrix[row+1][col] += beam_matrix[row][col];
                }
            }
        }
    }

    print_beam_matrix(beam_matrix, row_count, col_count);

    // Sum all paths in the last row
    for (int col = 0; col < COL_MAX_SIZE; col++) {
        if (beam_matrix[row_count-1][col] > 0)
            paths_count += beam_matrix[row_count-1][col];
    }

    printf("Possible paths count: %ld\n", paths_count);

    fclose(file);

    return 0;
}
