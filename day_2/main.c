#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INPUT_FILE_PATH "input.txt"
#define MULTIPLE_SUBSTRING_CHECK true

bool has_double_substring(char* input) {
    if (strlen(input) % 2 != 0)
        return false;

    size_t half_len = strlen(input) / 2;
    
    if (strncmp(input, input + half_len, half_len) == 0)
        return true;

    return false;
}

bool has_multiple_substring(char* input) {
    size_t half_lenf = strlen(input) / 2;

    for (size_t len = 1; len <= half_lenf; len++) {
        if (strlen(input) % len != 0)
            continue;
            
        int check_num = strlen(input) / len;

        for (int i = 1; i < check_num; i++) {
            if (strncmp(input, input + i * len, len) != 0)
                break;

            if (i == check_num - 1)
                return true;
        }
    }

    return false;
}

int main() {
    FILE* file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    char buffer[4096];
    char* substrings = strtok(fgets(buffer, sizeof(buffer), file), ",");

    long totalSum = 0;

    for (; substrings != NULL; substrings = strtok(NULL, ",")) {
        char* dashPtr = strchr(substrings, '-');
        
        long rangeStart = atol(strndup(substrings, dashPtr - substrings));
        long rangeEnd = atol(dashPtr + 1);

        for(long i = rangeStart; i <= rangeEnd; i++)
        {
            char numString[256];
            sprintf(numString, "%ld", i);

            bool check_condition = MULTIPLE_SUBSTRING_CHECK ? has_multiple_substring(numString) : has_double_substring(numString);

            if (check_condition)
            {
                totalSum+=i;
                printf("Found substring matching: %s in number: %s\n", substrings, numString);
            }
        }
    }

    printf("\nTotal Sum: %ld\n", totalSum);

    return 0;
}