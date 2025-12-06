#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LINE_BUFFER_SIZE 8196
#define VALUES_BUFFER 256
#define OPERATION_BUFFER 1024

#define INPUT_FILE_PATH "input.txt"

struct operation {
    unsigned long values[VALUES_BUFFER];
    size_t values_count;

    bool is_sum;
};

unsigned long get_operation_result(struct operation *op) {
    unsigned long result = op->is_sum ? 0 : 1;

    for (size_t i = 0; i < op->values_count; i++) {
        if (op->is_sum)
            result += op->values[i];
        else
        {
            result *= op->values[i];
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
    size_t op_index = 0;

    size_t lines_count = 0;

    char line[LINE_BUFFER_SIZE];
    while(fgets(line, sizeof(line), file)) {
        char* op_string_val = strtok(line, " ");
        op_index = 0;

        while (op_string_val != NULL) {
            if (strlen(op_string_val) == 1 && (op_string_val[0] == '+' || op_string_val[0] == '*')) {
                operations[op_index].is_sum = (op_string_val[0] == '+');
                
                op_string_val = strtok(NULL, " ");
                op_index++;
                continue;
            }

            unsigned long value = atol(op_string_val);

            operations[op_index].values[lines_count] = value;
            operations[op_index].values_count = lines_count + 1;

            op_string_val = strtok(NULL, " ");
            op_index++;
        }

        lines_count++;
    }

    operations_count = op_index - 1; // Just because I'm lazy and there is an extra operation for new line conversion shit

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