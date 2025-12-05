#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_FILE_PATH "input.txt"
#define LINE_BUFFER 256
#define RANGES_BUFFER 1024

struct range {
    long long start;
    long long end;
};

int main()
{
    FILE *file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }   

    char line[LINE_BUFFER];
    struct range ranges[RANGES_BUFFER];

    long long lowest_start = __LONG_LONG_MAX__;
    long long highest_end = -1;

    int range_count = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n' || line[0] == '\0')
            break; // Ranges are over

        long long start, end;
        if (sscanf(line, "%lld-%lld", &start, &end) == 2)
        {
            ranges[range_count].start = start;
            ranges[range_count].end = end;
            range_count++;

            if (start < lowest_start)
                lowest_start = start;
            if (end > highest_end)
                highest_end = end;
        }
    }

    int fresh_count = 0;

    while(fgets(line, sizeof(line), file))
    {
        long long value = atoll(line);

        if (value < lowest_start || value > highest_end)
            continue;

        for (int i = 0; i < range_count; i++)
        {
            if (value >= ranges[i].start && value <= ranges[i].end)
            {
                fresh_count++;
                break;
            }
        }
    }

    printf("Total fresh values: %d\n", fresh_count);

    fclose(file);

    return 0;
}