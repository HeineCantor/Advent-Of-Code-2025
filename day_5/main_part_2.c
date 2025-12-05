#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_FILE_PATH "input.txt"

#define LINE_BUFFER 256
#define RANGES_BUFFER 4096

struct range {
    long long start;
    long long end;
};

int compare_ranges(const void *a, const void *b) {
    struct range *range_a = (struct range *)a;
    struct range *range_b = (struct range *)b;
    if (range_a->start < range_b->start) return -1;
    if (range_a->start > range_b->start) return 1;
    return 0;
}

int main() {
    FILE *file = fopen(INPUT_FILE_PATH, "r");

    char line[LINE_BUFFER];
    struct range ranges[RANGES_BUFFER];

    int range_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0')
            break; // Ranges are over

        long long start, end;
        if (sscanf(line, "%lld-%lld", &start, &end) == 2) {
            ranges[range_count].start = start;
            ranges[range_count].end = end;
            range_count++;
        }
    }

    qsort(ranges, range_count, sizeof(struct range), compare_ranges);
    // printf("Sorted Ranges:\n");
    // for (int i = 0; i < range_count; i++) {
    //     printf("%lld-%lld\n", ranges[i].start, ranges[i].end);
    // }

    int merge_range_count = 0;
    int range_comparing_index = 0;
    struct range merged_ranges[RANGES_BUFFER];
    merged_ranges[0] = ranges[0];

    while (range_comparing_index < range_count) {
        if (merged_ranges[merge_range_count].end < ranges[range_comparing_index + 1].start) {
            merge_range_count++;
            merged_ranges[merge_range_count] = ranges[range_comparing_index + 1];
            range_comparing_index++;
        } else {
            merged_ranges[merge_range_count].end = 
                (ranges[range_comparing_index + 1].end > merged_ranges[merge_range_count].end) ? 
                ranges[range_comparing_index + 1].end : merged_ranges[merge_range_count].end;

            range_comparing_index++;
        }
    }
    
    long long fresh_count = 0;

    for (int i = 0; i <= merge_range_count; i++)
        fresh_count += (merged_ranges[i].end - merged_ranges[i].start + 1);

    fclose(file);

    printf("Number of values within any range: %lld\n", fresh_count);

    return 0;
}