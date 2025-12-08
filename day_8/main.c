#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 100
#define MAX_COORDINATES 1000

#define NUM_CONNECTIONS 1000
#define NUM_LARGEST_TO_MULTIPLY 3

#define LIMITLESS true

#define INPUT_FILE_PATH "input.txt"

struct coordinate {
    int x;
    int y;
    int z;

    int father_circuit_id;
    int circuit_counter;
};

struct distance_pair {
    int coord_index_a;
    int coord_index_b;
    unsigned long sq_distance;
};

unsigned long get_sq_distance(struct coordinate a, struct coordinate b) {
    long dx = a.x - b.x;
    long dy = a.y - b.y;
    long dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

int compare_distance_pairs(const void *a, const void *b) {
    struct distance_pair *pair_a = (struct distance_pair *)a;
    struct distance_pair *pair_b = (struct distance_pair *)b;

    if (pair_a->sq_distance < pair_b->sq_distance) return -1;
    if (pair_a->sq_distance > pair_b->sq_distance) return 1;
    return 0;
}

int compare_cluster_counters(const void *a, const void *b) {
    struct coordinate *coord_a = (struct coordinate *)a;
    struct coordinate *coord_b = (struct coordinate *)b;

    return coord_b->circuit_counter - coord_a->circuit_counter;
}

int main(){
    FILE *file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];

    struct coordinate coordinates[MAX_COORDINATES];
    int coord_count = 0;

    struct distance_pair *pairwise_distances = malloc(sizeof(struct distance_pair) * (MAX_COORDINATES * MAX_COORDINATES - MAX_COORDINATES));
    
    while(fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        coordinates[coord_count].x = atoi(strtok(line, ","));
        coordinates[coord_count].y = atoi(strtok(NULL, ","));
        coordinates[coord_count].z = atoi(strtok(NULL, ","));

        coordinates[coord_count].father_circuit_id = -1;
        coordinates[coord_count].circuit_counter = 1;

        coord_count++;
    }

    int pairwise_count = 0;

    int i; // We're gonna need it later!!

    // Build pairwise distance matrix
    for(i = 0; i < coord_count; i++) {
        for(int j = 0; j < coord_count; j++) {
            if (i == j) continue;

            pairwise_distances[pairwise_count].coord_index_a = i;
            pairwise_distances[pairwise_count].coord_index_b = j;
            pairwise_distances[pairwise_count].sq_distance = get_sq_distance(coordinates[i], coordinates[j]);

            pairwise_count++;
        }
    }

    qsort(pairwise_distances, coord_count * coord_count - coord_count, sizeof(struct distance_pair), compare_distance_pairs);

    int last_cluster_counter = 0;

    for (i = 0; (!LIMITLESS && i < NUM_CONNECTIONS) || (LIMITLESS && last_cluster_counter < coord_count); i++) {
        printf("Connection %d: Coord A [%d] <-> Coord B [%d] | Sq Distance: %ld\n", i + 1,
               pairwise_distances[2*i].coord_index_a,
               pairwise_distances[2*i].coord_index_b,
               pairwise_distances[2*i].sq_distance);

        // I want to merge one in another, so I need to find the father circuit id of both
        int index_a = pairwise_distances[2*i].coord_index_a;
        int index_b = pairwise_distances[2*i].coord_index_b;

        int father_a = coordinates[index_a].father_circuit_id;
        int father_b = coordinates[index_b].father_circuit_id;

        // Union-Find like structure to find the root
        while (father_a != -1) {
            index_a = father_a;
            father_a = coordinates[index_a].father_circuit_id;
        }

        while (father_b != -1) {
            index_b = father_b;
            father_b = coordinates[index_b].father_circuit_id;
        }

        if (index_a != index_b) {
            // Merge smaller cluster into larger one
            if (coordinates[index_a].circuit_counter >= coordinates[index_b].circuit_counter) {
                coordinates[index_b].father_circuit_id = index_a;
                coordinates[index_a].circuit_counter += coordinates[index_b].circuit_counter;
                coordinates[index_b].circuit_counter = 0;

                last_cluster_counter = coordinates[index_a].circuit_counter;
            } else {
                coordinates[index_a].father_circuit_id = index_b;
                coordinates[index_b].circuit_counter += coordinates[index_a].circuit_counter;
                coordinates[index_a].circuit_counter = 0;

                last_cluster_counter = coordinates[index_b].circuit_counter;
            }
        }
    }

    if (!LIMITLESS)
    {
        qsort(coordinates, coord_count, sizeof(struct coordinate), compare_cluster_counters);
        
        printf("Ordered cluster sizes:\n");
        for (int i = 0; i < coord_count; i++) {
            if (coordinates[i].circuit_counter == 0) continue;
            printf("Cluster %d size: %d\n", i + 1, coordinates[i].circuit_counter);
        }

        long problem_output = 1;

        for (int i = 0; i < NUM_LARGEST_TO_MULTIPLY; i++) {
            problem_output *= coordinates[i].circuit_counter;
        }

        printf("Problem output (product of %d largest clusters): %ld\n", NUM_LARGEST_TO_MULTIPLY, problem_output);
    } else {
        // i is the last connection, let's take coordinates

        struct coordinate lastCoordA = coordinates[pairwise_distances[2*(i - 1)].coord_index_a];
        struct coordinate lastCoordB = coordinates[pairwise_distances[2*(i - 1)].coord_index_b];

        printf("Last connected coordinates were:\n");
        printf("Coord A: (%d, %d, %d)\n", lastCoordA.x, lastCoordA.y, lastCoordA.z);
        printf("Coord B: (%d, %d, %d)\n", lastCoordB.x, lastCoordB.y, lastCoordB.z);    

        unsigned long problem_output = ((unsigned long) lastCoordA.x) * ((unsigned long) lastCoordB.x);

        printf("Output (product of both Xs coordinates): %lu\n", problem_output);
    }

    free(pairwise_distances);
    fclose(file);

    return 0;
}