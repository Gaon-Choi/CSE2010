#include <stdio.h>
#include <stdlib.h>

#define INF 99999;

typedef struct Node {
    int key;
    int idx;
    int dist;
    int heap_idx;
    struct Node* prev;
} Node;

typedef struct Heap {
    int capacity;
    int size;
    Node** arr;
    int (*cmp_func)(Node*, Node*);
} Heap;

Node* make_node(int key, int idx);
void InsertEdge(int** adjacency, int a, int b, int w);
int find_idx(Node** nodes, int size, int n);
void swap(Node** l, Node** r);
void delete(Heap* pq);
Node** dijkstra(int** adjacency, Node** nodes, int size, int start);
int all_visited(int* visited_, int start_idx, int size);
int is_empty(Heap* pq);

int main(void) {
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");

    char str[1001];
    Node* nodes[100];
    int num, read_byte, idx = 0;
    char* ptr = str;
    int i, j;

    fgets(str, 1001, input);

    while(sscanf(ptr, "%d%n", &num, &read_byte) == 1) {
        nodes[idx] = make_node(num, idx);
        idx += 1;
        ptr += read_byte;
    }

    int** adjacency = malloc(sizeof(int*) * idx);

    for(i = 0; i < idx; i++) {
        adjacency[i] = malloc(sizeof(int) * idx);
        for(j = 0; j < idx; j++)
            adjacency[i][j] = INF;
    }
    
    int u;  // initial vertex
    int v;  // terminal vertex
    int w;  // weight

    fgets(str, 1001, input);
    ptr = str;

    while(sscanf(ptr, "%d-%d-%d%n", &u, &v, &w, &read_byte) == 3) {
        InsertEdge(adjacency, find_idx(nodes, idx, u), find_idx(nodes, idx, v), w);
        ptr += read_byte;
    }

    int start;
    int end;
    fscanf(input, "%d %d", &start, &end);

    Node** prev = dijkstra(adjacency, nodes, idx, start);

    Node* now_node = nodes[find_idx(nodes, idx, end)];

    int* result = malloc(sizeof(int) * idx);
    for(i = 0; i < idx; i++) {
        result[i] = now_node->key;
        now_node = prev[now_node->idx];
        if(now_node == NULL) break;
    }

    if(result[i] != start)
        fprintf(output, "no path\n");
    else {
        for(;i >= 0; i--) {
            fprintf(output, "%d ", result[i]);
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}

Node* make_node(int key, int idx) {
    Node* result = malloc(sizeof(Node));
    result->key = key;
    result->prev = NULL;
    result->dist = INT_MAX;
    result->heap_idx = -1;
    result->idx = idx;
    return result;
}

void InsertEdge(int** adjacency, int a, int b, int w) {
    adjacency[a][b] = w;
}

int find_idx(Node** nodes, int size, int n) {
    int i;
    for(i = 0; i < size; i++) {
        if(nodes[i]->key == n)
            return i;
    }
    return -1;
}

void swap(Node** l, Node** r) {
    Node* temp = *l;
    *l = *r;
    *r = temp;

    int temp_i = (*l)->heap_idx;
    (*l)->heap_idx = (*r)->heap_idx;
    (*r)->heap_idx = temp_i;
}

void perc_up(Heap* heap, int idx)
{
    int i;
    Node* value = heap->arr[idx];
    for(i = idx; i/2 && !heap->cmp_func(heap->arr[i/2], value); i /= 2) {
        heap->arr[i] = heap->arr[i/2];
        heap->arr[i]->heap_idx = i;
    }
    heap->arr[i] = value;
    value->heap_idx = i;
}

// percolating elements in given index until heap satisfing partial ordering
void perc_down(Heap* heap, int index)
{
    int i;
    int len = heap->size;
    int temp;
    Node* value = heap->arr[index];
    for(i = index; 2*i <= len; i = temp) {
        temp = 2*i;
        if(temp != len && !heap->cmp_func(heap->arr[temp], heap->arr[temp + 1])) {
            ++temp;
        }
        if(heap->cmp_func(value, heap->arr[temp])) break;
        heap->arr[i] = heap->arr[temp];
        heap->arr[temp]->heap_idx = i;
    }
    heap->arr[i] = value;
    value->heap_idx = i;
}

void delete(Heap* pq) {
    if(is_empty(pq)) return;
    swap(&pq->arr[1], &pq->arr[pq->size--]);
    perc_down(pq, 1);
}

Heap* make_heap(Node** data_arr, int len, int(*cmp_func)(Node*,Node*))
{
    int i, j;
    Node* temp, value;
    Heap *heap = malloc(sizeof(Heap));

    heap->capacity = len;
    heap->size = len;
    heap->arr = malloc(sizeof(Node*) * (len + 1));
    heap->cmp_func = cmp_func;
    for(i = 0; i < len; ++i) {
        heap->arr[i+1] = data_arr[i];
        heap->arr[i+1]->heap_idx = i+1;
    }

    for(i = len / 2; i >= 1; --i) {
        perc_down(heap, i);
    }
    return heap;
}

int min(Node* left, Node* right) {
    return left->dist <= right->dist;
}

int is_empty(Heap* pq) {
    return pq->size <= 0;
}

Node* front(Heap* pq) {
    return pq->arr[1];
}

Node** dijkstra(int** adjacency, Node** nodes, int size, int start) {
    int i;
    nodes[find_idx(nodes, size, start)]->dist = 0;
    
    Heap* heap = make_heap(nodes, size, min);
    Node* now_node;
    while(!is_empty(heap)) {
        now_node = front(heap);
        delete(heap);
        if(now_node->dist == INT_MAX) break;
        for(i = 0; i < size; ++i) {
            if(adjacency[now_node->idx][i] < INT_MAX && nodes[i]->dist > now_node->dist + adjacency[now_node->idx][i]) {
                nodes[i]->dist = now_node->dist + adjacency[now_node->idx][i];
                nodes[i]->prev = now_node;
                perc_up(heap, nodes[i]->heap_idx);
            }
        }
    }

    Node** result = malloc(sizeof(Node*) * size);
    for(i = 0; i < size; ++i) {
        result[i] = nodes[i]->prev;
    }
    return result;
}

int all_visited(int* visited_, int start_idx, int size) {
    int i;
    for(i = start_idx; i < size; i++){
        if(visited_[i] == 0)
            return 0;
    }
    return 1;
}