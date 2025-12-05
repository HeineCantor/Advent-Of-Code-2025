#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_FILE_PATH "input.txt"

int main() {
    const char *filename = INPUT_FILE_PATH;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[256];
    bool input_matrix[1000][1000] = { false };

    // for line in file, fill row of input_matrix
    int row = 0;
    while (fgets(line, sizeof(line), file)) {
        for (int col = 0; col < strlen(line); col++) {
            if (line[col] == '@') {
                input_matrix[row][col] = true;
            } else if (line[col] == '.') {
                input_matrix[row][col] = false;
            }
        }
        row++;
    }

    fclose(file);

    int accessible_rolls = 0;
    bool no_more_accessible = false;

    while (!no_more_accessible)
    {
        no_more_accessible = true;

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < strlen(line); j++) {
                if (!input_matrix[i][j]) {
                    printf("%c", '.');
                    continue;
                }

                int adjacent_rolls = 0;
                
                if (i > 0)
                {
                    adjacent_rolls += input_matrix[i - 1][j];
                    if (j > 0)
                        adjacent_rolls += input_matrix[i - 1][j - 1];
                    if (j < strlen(line) - 1)
                        adjacent_rolls += input_matrix[i - 1][j + 1];
                }

                if (i < row - 1)
                {
                    adjacent_rolls += input_matrix[i + 1][j];
                    if (j > 0)
                        adjacent_rolls += input_matrix[i + 1][j - 1];
                    if (j < strlen(line) - 1)
                        adjacent_rolls += input_matrix[i + 1][j + 1];
                }

                if (j > 0)
                    adjacent_rolls += input_matrix[i][j - 1];
                if (j < strlen(line) - 1)
                    adjacent_rolls += input_matrix[i][j + 1];

                if (adjacent_rolls < 4)
                {
                    no_more_accessible = false;
                    input_matrix[i][j] = false;
                    accessible_rolls++;
                    printf("%c", 'x');
                }
                else
                    printf("%c", '@');
            }
            printf("\n");
        }
    }

    printf("Accessible rolls: %d\n", accessible_rolls);

    return 0;
}