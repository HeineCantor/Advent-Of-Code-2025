#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INPUT_FILE_PATH "input.txt"

bool has_double_substring(char* input, char* substring) {
    if (strlen(input) % 2 != 0)
        return false;

    size_t half_len = strlen(input) / 2;
    
    if (strncmp(input, input + half_len, half_len) == 0)
    {
        strncpy(substring, input, half_len);
        return true;
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

    int totalSum = 0;

    for (; substrings != NULL; substrings = strtok(NULL, ",")) {
        char* dashPtr = strchr(substrings, '-');
        
        int rangeStart = atoi(strndup(substrings, dashPtr - substrings));
        int rangeEnd = atoi(dashPtr + 1);

        for(int i = rangeStart; i <= rangeEnd; i++)
        {
            char numString[256];
            sprintf(numString, "%d", i);

            char tmp_buffer[256];
            if (has_double_substring(numString, tmp_buffer))
            {
                totalSum+=i;
                printf("Found double substring: %s in number: %s\n", substrings, numString);
            }
        }
    }

    printf("\nTotal Sum: %d\n", totalSum);

    return 0;
}