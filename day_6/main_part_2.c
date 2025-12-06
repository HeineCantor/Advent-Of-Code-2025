#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_BUFFER_SIZE 8196
#define VALUES_BUFFER 256
#define OPERATION_BUFFER 1024

#define INPUT_FILE_PATH "input.txt"

struct operation {
    char* values[VALUES_BUFFER];
    
    size_t values_count;

    bool is_sum;
};

unsigned long get_operation_result(struct operation *op) {
    unsigned long result = op->is_sum ? 0 : 1;

    char str_operands[VALUES_BUFFER][VALUES_BUFFER] = {0};
    size_t char_len = strlen(op->values[0]);

    for (size_t i = 0; i < char_len; i++) {
        int chars_found = 0;

        for (size_t j = 0; j < op->values_count; j++) {
            if (op->values[j][i] == ' ') {
                if (chars_found)
                    break;
                else
                    continue;
            }

            str_operands[i][chars_found] = op->values[j][i];
            chars_found++;
        }

        str_operands[i][chars_found] = '\0';
    }

    for (size_t i = 0; i < char_len; i++) {
        if (strlen(str_operands[i]) == 0)
            continue;

        unsigned long val = atol(str_operands[i]);

        if (op->is_sum)
            result += val;
        else {
            result *= val;
            if (result == 0) // If just one zero is found, every other multiplication is pointless
                break;
        }
    }

    return result;
}

int main() {
    FILE *file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    struct operation operations[OPERATION_BUFFER];
    size_t operations_count = 0;

    char line[LINE_BUFFER_SIZE];

    // Getting last line first
    while(fgets(line, sizeof(line), file));
    
    size_t last_line_indices[OPERATION_BUFFER];

    // I want all non-blank indices (operations)
    for (size_t i = 0; i < strlen(line); i++) {
        if (line[i] == '+' || line[i] == '*')
        {
            operations[operations_count].is_sum = (line[i] == '+');
            last_line_indices[operations_count++] = i;
        }
    }

    // Let's start again :D
    fseek(file, 0, SEEK_SET);
    while(fgets(line, sizeof(line), file)) {
        if (line[0] == '+' || line[0] == '*')
            break;

        for(size_t i = 0; i < operations_count; i++) {
            size_t start_index = last_line_indices[i];
            size_t end_index = (i + 1 < operations_count) ? last_line_indices[i + 1] : strlen(line);

            operations[i].values[operations[i].values_count] = strndup(&line[start_index], end_index - start_index - 1);
            operations[i].values_count++;
        }
    }

    unsigned long total_result = 0;

    for (size_t i = 0; i < operations_count; i++) {
        unsigned long result = get_operation_result(&operations[i]);
        printf("Operation %zu result: %lu\n", i + 1, result);

        total_result += result;
    }

    printf("\nTotal result: %lu\n", total_result);

    fclose(file);
    return 0;
}