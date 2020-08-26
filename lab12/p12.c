#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// the trace of deletion, must be NEGATIVE!
#define DEL -1

typedef struct HashTable_ {
    int size;
    int *data;
    int (*hash_func)(struct HashTable_ *, int);
    int (*solution)(struct HashTable_ *, int, int);
} HashTable;

HashTable *build_hashtable(int size, int (*hash_func)(HashTable *, int), int (*solution)(HashTable *, int, int));
void free_hashtable(HashTable *hashtable);
int mod_func(HashTable *hashtable, int data);
int linear_probing(HashTable *hashtable, int data, int j);
int quadratic_probing(HashTable *hashtable, int data, int j);
int double_hashing(HashTable *hashtable, int data, int j);
int insert(HashTable *hashtable, int data);
int delete (HashTable *hashtable, int data);
int find(HashTable *hashtable, int data);
void print_data(HashTable *hashtable, FILE *output);

int main(void) {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    int num; // the number of test cases
    fscanf(input, "%d", &num);

    int (*hash_func_)(HashTable *, int) = mod_func;
    int (*probe_func_)(HashTable * hashtable, int, int);

    char solution[10];
    int data, size, i;
    char opt[2];
    int operation;

    HashTable *hashtable;

    for (i = 0; i < num; i++) {
        fscanf(input, "%s", solution);
        if (!strcmp(solution, "Linear"))
            probe_func_ = linear_probing;
        else if (!strcmp(solution, "Quadratic"))
            probe_func_ = quadratic_probing;
        else if (!strcmp(solution, "Double"))
            probe_func_ = double_hashing;
        fprintf(output, "%s\n", solution);

        fscanf(input, "%d", &size);
        hashtable = build_hashtable(size, hash_func_, probe_func_);

        while (fscanf(input, "%s", opt) == 1 && opt[0] != 'q') {
            switch (opt[0]) {
            case 'i':
                fscanf(input, "%d", &data);
                operation = insert(hashtable, data);
                if (operation)
                    fprintf(output, "Inserted %d\n", data);
                else
                    fprintf(output, "Already exists\n");
                break;
            case 'f':
                fscanf(input, "%d", &data);
                operation = find(hashtable, data);
                if (operation >= 0)
                    fprintf(output, "%d has index of %d\n", data, operation);
                else
                    fprintf(output, "Not found\n");
                break;
            case 'd':
                fscanf(input, " %d", &data);
                operation = delete (hashtable, data);
                if (operation)
                    fprintf(output, "Deleted %d\n", data);
                else
                    fprintf(output, "%d not exists\n", data);
                break;
            case 'p':
                print_data(hashtable, output);
                break;
            default:
                break;
            }
        }
        free_hashtable(hashtable);
        fprintf(output, "\n");
    }
    fclose(input);
    fclose(output);
    return 0;
}

HashTable *build_hashtable(int size, int (*hash_func)(HashTable *, int), int (*solution)(HashTable *, int, int)) {
    HashTable *hashtable = malloc(sizeof(HashTable));
    hashtable->size = size;
    hashtable->data = malloc(sizeof(int) * size);
    int i;
    for (i = 0; i < hashtable->size; i++) {
        hashtable->data[i] = 0;
    }

    hashtable->hash_func = hash_func;
    hashtable->solution = solution;

    return hashtable;
}

void free_hashtable(HashTable *hashtable) {
    if (hashtable == NULL)
        return;
    else {
        free(hashtable->data);
        free(hashtable);
    }
}

int mod_func(HashTable *hashtable, int data) {
    return data % (hashtable->size);
}

int linear_probing(HashTable *hashtable, int data, int j) {
    return (hashtable->hash_func(hashtable, data) + j) % hashtable->size;
}

int quadratic_probing(HashTable *hashtable, int data, int j) {
    return (hashtable->hash_func(hashtable, data) + (j * j)) % hashtable->size;
}

int double_hashing(HashTable *hashtable, int data, int j) {
    // the secondary function
    // = 7 - (data % 7)
    return (hashtable->hash_func(hashtable, data) + (j * (7 - (data % 7)))) % hashtable->size;
}

int insert(HashTable *hashtable, int data) {
    int i;
    // if the data already exists
    if (find(hashtable, data) >= 0) {
        return 0;
    }
    else {
        // find the empty slot for the new data
        for (i = 0;
             hashtable->data[hashtable->solution(hashtable, data, i)] > 0
             && hashtable->data[hashtable->solution(hashtable, data, i)] != data;
             i++);

        if (hashtable->data[hashtable->solution(hashtable, data, i)] <= 0) {
            // <= 0 : it means when it is blank(0)
            //               + when it is DEL(-1)
            hashtable->data[hashtable->solution(hashtable, data, i)] = data;
            return 1;
        }
    }
}

int find(HashTable *hashtable, int data) {
    int i;
    for (i = 0;
         hashtable->data[hashtable->solution(hashtable, data, i)] != 0 
         && hashtable->data[hashtable->solution(hashtable, data, i)] != data;
         ++i) {
        if (i > hashtable->size) {
            return -1;
        }
    }

    if (hashtable->data[hashtable->solution(hashtable, data, i)] == 0) {
        return -1;
    }
    return hashtable->solution(hashtable, data, i);
}

int delete (HashTable *hashtable, int data) {
    int exist = find(hashtable, data);
    if (exist == -1)
        return 0;
    else {
        hashtable->data[exist] = DEL;
        return 1;
    }
}

void print_data(HashTable *hashtable, FILE *output) {
    int i;
    for (i = 0; i < hashtable->size; i++) {
        fprintf(output, "%d ", (hashtable->data[i] >= 0) ? hashtable->data[i] : 0);
    }
    fprintf(output, "\n");
}