#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define INPUT_FILE_PATH "input.txt"
#define LARGEST_BUF_SIZE 64

long get_largest_n_in_order(char* input, int n) {
    long long nLargest[LARGEST_BUF_SIZE] = {-1};
    int nLargestIndices[LARGEST_BUF_SIZE] = {-1};

    long long returnValue = 0;

    for (int i = 0; i < n; i++) {
        
        int startingIndex = i == 0 ? 0 : nLargestIndices[i-1]+1;

        for (size_t j = startingIndex; j < strlen(input) - (n - i - 1); j++) {
            int converted = input[j] - '0';

            if (converted == 9)
            {
                nLargest[i] = converted;
                nLargestIndices[i] = j;
                break;
            }

            if (converted > nLargest[i]) {
                nLargest[i] = converted;
                nLargestIndices[i] = j;
            }
        }

        returnValue += pow(10, n-i-1) * nLargest[i];
    }

    return returnValue;
}

int main() {
    FILE* file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    long long totalSum = 0;

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;

        long long get_joltage = get_largest_n_in_order(buffer, 12);
        printf("Largest %d in order multiplier: %lld\n", 12, get_joltage);

        totalSum += get_joltage;
    }

    printf("\nTotal Sum: %lld\n", totalSum);

    return 0;
}