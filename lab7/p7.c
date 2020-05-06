#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Maze_ {
    int size;
    int *ds;
    char **edge;
} Maze;

void maze_init(Maze *maze) {
    int i, j;
    int ds_size = maze->size * maze->size;
    maze->ds = malloc(sizeof(int) * (ds_size + 1));
    for (i = 1; i <= ds_size; i++)
        maze->ds[i] = 0;

    maze->edge = malloc(sizeof(char *) * (2 * maze->size + 1));
    for (i = 0; i < 2 * maze->size + 1; i++) {
        if (i % 2 == 0) {
            maze->edge[i] = malloc(sizeof(char) * maze->size);
            for (j = 0; j < maze->size; j++)
                maze->edge[i][j] = '-';
        }
        else {
            maze->edge[i] = malloc(sizeof(char) * maze->size + 1);
            for (j = 0; j < maze->size + 1; j++)
                maze->edge[i][j] = '|';
        }
    }
    maze->edge[1][0] = ' ';
    maze->edge[2 * maze->size - 1][maze->size] = ' ';
}

void delete_maze(Maze *maze) {
    int i;
    for (i = 0; i < 2 * maze->size + 1; i++)
        free(maze->edge[i]);
    free(maze->edge);
    free(maze->ds);
}

void print_maze(Maze *maze, FILE *output) {
    int i, j;
    for (i = 0; i < 2 * maze->size + 1; i++) {
        if (i % 2 == 0) {
            for (j = 0; j < 2 * maze->size + 1; j++)
                fprintf(output, "%c", (j % 2 == 0) ? '+' : maze->edge[i][j / 2]);
        }
        else {
            for (j = 0; j < 2 * maze->size + 1; j++)
                fprintf(output, "%c", (j % 2 == 0) ? maze->edge[i][j / 2] : ' ');
        }
        fprintf(output, "\n");
    }
}

int find(int *ds, int data) {
    if (ds[data] > 0)
        return ds[data] = find(ds, ds[data]);
    return data;
}

void union_ds(int *ds, int small, int large) {
    small = find(ds, small);
    large = find(ds, large);

    if (ds[small] > ds[large])
        ds[small] = large;
    else {
        if (ds[small] == ds[large])
            --ds[small];
        ds[large] = small;
    }
}

void remove_edge(Maze *maze, int ypos, int xpos) {
    int small, large;
    int i, j;
    int small_root, large_root;
    // "-": UP-DOWN
    if (ypos % 2 == 0) {
        small = (ypos / 2 - 1) * maze->size + xpos + 1;
        large = (ypos / 2) * maze->size + xpos + 1;
    }
    // "|": LEFT-RIGHT
    else {
        small = (ypos / 2) * maze->size + xpos;
        large = (ypos / 2) * maze->size + xpos + 1;
    }

    small_root = find(maze->ds, small);
    large_root = find(maze->ds, large);
    if (small_root == large_root)
        return;

    union_ds(maze->ds, small, large);
    maze->edge[ypos][xpos] = ' ';
}

int check_sets(Maze *maze) {
    int i;
    int count = 0;
    for (i = 1; i <= maze->size * maze->size; i++) {
        if (maze->ds[i] <= 0)
            count++;
        if (count > 1)
            return 1;
    }
    return 0;
}

int main(void) {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    Maze maze;
    int size;

    srand((unsigned int)time(NULL));
    fscanf(input, "%d", &(maze.size));

    maze_init(&maze);

    while (check_sets(&maze)) {
        int x, y;
        y = rand() % (2 * maze.size - 1) + 1;
        if (y % 2 == 1) {       // "|"
            x = rand() % (maze.size - 1);
            remove_edge(&maze, y, x + 1);
        }
        else {                  // "-"
            x = rand() % (maze.size);
            remove_edge(&maze, y, x);
        }
    }
    print_maze(&maze, output);
    delete_maze(&maze);

    fclose(input);
    fclose(output);
    return 0;
}