#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_FILE_PATH "input_test.txt"

#define MAX_LINE_LENGTH 256
#define MAX_NUM_COORDS 1000

struct coordinates {
    int x;
    int y;
};

bool is_vertex_in_polyline(struct coordinates* polyline, int vertices_count, struct coordinates vertex) {
    int crossing_count = 0;
    
    for (int i = 0; i < vertices_count; i++) {
        struct coordinates vertex_1, vertex_2;

        vertex_1 = polyline[i];
        vertex_2 = polyline[(i+1)%vertices_count];

        bool is_vertical = vertex_1.x == vertex_2.x;

        int min_x = vertex_1.x < vertex_2.x ? vertex_1.x : vertex_2.x;
        int max_x = vertex_1.x < vertex_2.x ? vertex_2.x : vertex_1.x;

        int min_y = vertex_1.y < vertex_2.y ? vertex_1.y : vertex_2.y;
        int max_y = vertex_1.y < vertex_2.y ? vertex_2.y : vertex_1.y;

        if (is_vertical && vertex.x == vertex_1.x) {
            if (vertex.y <= max_y && vertex.x >= min_y)
                return true;
        } else if (!is_vertical && vertex.y == vertex_1.y) {
            if (vertex.x <= max_x && vertex.x >= min_x)
                return true;
        }

        if (is_vertical) {
            if (vertex.x <= vertex_1.x && vertex.y >= min_y && vertex.y <= min_y)
                crossing_count++;
        }
    }

    return crossing_count % 2 != 0;
}

bool rect_intersects_polyline(struct coordinates angle_1, struct coordinates angle_2, struct coordinates* polyline, int vertices_count) {
    struct coordinates left_angle, right_angle;
    struct coordinates other_left_angle, other_right_angle;

    if (angle_1.x < angle_2.x) {
        left_angle = angle_1;
        right_angle = angle_2;
    } else {
        right_angle = angle_1;
        left_angle = angle_2;
    }

    other_left_angle.x = left_angle.x;
    other_left_angle.y = right_angle.y;

    other_right_angle.x = right_angle.x;
    other_right_angle.y = left_angle.y;

    printf("Current Rectangle: R(%d, %d) - L(%d, %d) - AR(%d, %d) - AL(%d, %d)\n",
                right_angle.x, right_angle.y,
                left_angle.x, left_angle.y,
                other_right_angle.x, other_right_angle.y,
                other_left_angle.x, other_left_angle.y);

    printf("Right is in polyline: %d\n", is_vertex_in_polyline(polyline, vertices_count, right_angle));
    printf("Left is in polyline: %d\n", is_vertex_in_polyline(polyline, vertices_count, left_angle));
    printf("Alt Right is in polyline: %d\n", is_vertex_in_polyline(polyline, vertices_count, other_right_angle));
    printf("Alt Left is in polyline: %d\n", is_vertex_in_polyline(polyline, vertices_count, other_left_angle));

    // La logica è:
    //  * se i 2 ALTRI angoli sono sul bordo o dentro (i 2 di ingresso sono chiaramente sul bordo)
    //  AND
    //  * se nessun lato del rettangolo interseca un lato della polilinea
    // allora il rettangolo è valido.



    // for (int i = 0; i < vertices_count; i++) {
    //     struct coordinates vertex_1, vertex_2;

    //     vertex_1 = polyline[i];
    //     vertex_2 = polyline[(i+1)%vertices_count];

    //     bool is_vertical = vertex_1.x == vertex_2.x;


    // }
}

int main() {
    FILE* file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    struct coordinates coords[MAX_NUM_COORDS];
    struct coordinates unsorted_coords[MAX_NUM_COORDS];

    int x_indices[MAX_NUM_COORDS];
    int indices_count = 0;

    unsigned long max_area = 1;

    int coord_count = 0;
    while(fgets(line, sizeof(line), file)) {
        coords[coord_count].x = atoi(strtok(line, ","));
        coords[coord_count].y = atoi(strtok(NULL, ","));

        unsorted_coords[coord_count].x = coords[coord_count].x;
        unsorted_coords[coord_count].y = coords[coord_count].y;

        coord_count++;
    }

    // Sort coordinates by x, then by y
    for (int i = 0; i < coord_count - 1; i++) {
        for (int j = i + 1; j < coord_count; j++) {
            if (coords[i].x > coords[j].x ||
               (coords[i].x == coords[j].x && coords[i].y > coords[j].y)) {
                struct coordinates temp = coords[i];
                coords[i] = coords[j];
                coords[j] = temp;
            }
        }
    }    

    x_indices[0] = 0;
    indices_count++;

    int last_x_coord = coords[0].x;

    // Indices construction
    for (int i = 1; i < coord_count; i++)
    {
        if (coords[i].x != last_x_coord){
            last_x_coord = coords[i].x;
            x_indices[indices_count] = i;
            indices_count++;
        }
    }

    printf("Sorted Coordinates:\n");
    for (int i = 0; i < coord_count; i++) {
        printf("(%d, %d)\n", coords[i].x, coords[i].y);
    }

    printf("Indices:\n");
    for (int i = 0; i < indices_count; i++) {
        printf("IDX: %d\n", x_indices[i]);
    }

    for (int i = 0; i < coord_count; i++) {
        for (int j = 0; j < indices_count - 1; j++) {
            if (x_indices[j] == coords[i].x)
                continue;

            int index_coord = x_indices[j];
            int next_index_coord = x_indices[j+1];

            // First coordinate of index
            struct coordinates leftest = coords[index_coord];
            struct coordinates rightest = coords[next_index_coord-1];

            struct coordinates current_coord = coords[i];

            long max_distance = abs(current_coord.y - rightest.y);
            bool is_left = false;

            if (abs(current_coord.y - leftest.y) > max_distance) {
                max_distance = abs(current_coord.y - leftest.y);
                is_left = true;
            }

            long new_area = max_distance + 1;
            new_area *= is_left ? 
                abs(current_coord.x - leftest.x + 1) : 
                abs(current_coord.x - rightest.x + 1);

            if (is_left)    
                rect_intersects_polyline(current_coord, leftest, unsorted_coords, coord_count);
            else
                rect_intersects_polyline(current_coord, rightest, unsorted_coords, coord_count);

            if (new_area > max_area) {
                max_area = new_area;
            }
        }
    }

    printf("Max Area found: %lu\n", max_area);

    return 0;
}