#include <stdio.h>
#include <stdlib.h>

typedef struct Heap {
    int capacity;
    int size;
    int *elements;
} Heap;

Heap *heap_init(int capicity_) {
    Heap *heap = malloc(sizeof(Heap));
    heap->capacity = capicity_;
    heap->size = 0;
    heap->elements = malloc(sizeof(int) * (capicity_ + 1));
    return heap;
}

int exist_check(int data, Heap *heap) {
    int i;
    for (i = 1; i <= heap->size; i++) {
        if (data == heap->elements[i])
            return 1;
    }
    return 0;
}

void percolate_up(Heap *heap, int idx) {
    int i;
    int data = heap->elements[idx];
    for (i = heap->size; (i / 2) && heap->elements[i / 2] < data; i /= 2)
        heap->elements[i] = heap->elements[i / 2];
    heap->elements[i] = data;
}

void insert(int data, Heap *heap, FILE *output) {
    if (heap->capacity == heap->size)
        fprintf(output, "heap is full\n");
    else if (exist_check(data, heap))
        fprintf(output, "%d is already in the heap.\n", data);
    else {
        heap->elements[++heap->size] = data;
        percolate_up(heap, heap->size);
        fprintf(output, "insert %d\n", data);
    }
}

void find(int data, Heap *heap, FILE *output) {
    if (exist_check(data, heap))
        fprintf(output, "%d is in the heap.\n", data);
    else
        fprintf(output, "%d is not in the heap.\n", data);
}

void print(Heap *heap, FILE *output) {
    int i;
    for (i = 1; i <= heap->size; ++i)
        fprintf(output, "%d ", heap->elements[i]);
    fprintf(output, "\n");
}

void delete_heap(Heap *heap) {
    free(heap->elements);
    free(heap);
}

int main(void) {
    // FILE I/O
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    // initialize the heap with the capicity
    int capicity;
    fscanf(input, "%d", &capicity);
    Heap *heap = heap_init(capicity);

    char opt;
    int data;

    // heap operations
    while (fscanf(input, "%c", &opt) == 1) {
        switch (opt) {
        case 'i':
            fscanf(input, "%d", &data);
            insert(data, heap, output);
            break;
        case 'f':
            fscanf(input, "%d", &data);
            find(data, heap, output);
            break;
        case 'p':
            print(heap, output);
            break;
        default:
            break;
        }
    }

    // delete the heap
    delete_heap(heap);

    // File closing
    fclose(input);
    fclose(output);
    return 0;
}