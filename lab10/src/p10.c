#include <stdio.h>
#include <stdlib.h>

typedef struct _Graph {
    int size;
    int* node;
    int** matrix;
}Graph;

typedef struct _Queue {
    int* key;
    int first;
    int rear;
    int qsize;
    int max_queue_size;
}Queue;

Graph* CreateGraph(int* nodes, int size);
void InsertEdge(Graph* graph, int a, int b);
void Topsort(Graph* G, FILE* output);
Queue* MakeNewQueue(int X);
void delete_queue(Queue* queue);
void Enqueue(Queue* Q, int X);
int Dequeue(Queue* Q);
void print_matrix(FILE* output, Graph* graph);
void delete_graph(Graph* graph);
int* sorted_order(int* arr, int n);
void swap(int* a, int* b);

int main(void) {
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt","w");

    char str[1001];
    int num, read_byte, idx = 0;
    char* ptr = str;
    int nodes[501];
    int* sorted_array;
    fgets(str, 1001, input);

    while(sscanf(ptr, "%d%n", &num, &read_byte) == 1) {
        nodes[idx++] = num;
        ptr += read_byte;
    }

    Graph* graph = CreateGraph(nodes, idx);
    sorted_array = malloc(sizeof(int) * graph->size);

    int u;  // initial  vertex
    int v;  // terminal vertex

    while(fscanf(input, "%d-%d", &u, &v) == 2) {
        InsertEdge(graph, u, v);
    }

    print_matrix(output, graph);
    Topsort(graph, output);

    delete_graph(graph);

    fclose(input);
    fclose(output);
    return 0;
}

Graph* CreateGraph(int* nodes, int size) {
    Graph* graph = malloc(sizeof(Graph));
    graph->size = size;
    graph->node = malloc(sizeof(int) * size);
    graph->matrix = malloc(sizeof(int*) * size);

    int i, j;

    for(i = 0; i < size; i++)
        graph->node[i] = nodes[i];

    for(i = 0; i < size; i++) {
        graph->matrix[i] = malloc(sizeof(int) * size);
        for(j = 0; j < size; j++)
            graph->matrix[i][j] = 0;
    }

    return graph;
}

void InsertEdge(Graph* graph, int a, int b) {
    int idx_1, idx_2;
    int i;
    for(i = 0; i < graph->size; i++) {
        if(graph->node[i] == a)
            idx_1 = i;
        if(graph->node[i] == b)
            idx_2 = i;
    }
    graph->matrix[idx_1][idx_2] = 1;
}

void Topsort(Graph* G, FILE* output) {
    int i, j, idx, del;
    int count = 0;
    int* sorted_arr = malloc(sizeof(int) * G->size);
    int* indegree = malloc(sizeof(int) * G->size);
    Queue* queue = MakeNewQueue(G->size);
    int* order = sorted_order(G->node, G->size);

    for(i = 0; i < G->size; i++) {
        idx = order[i];
        indegree[idx] = 0;
        for(j = 0; j < G->size; j++)
            indegree[idx] += G->matrix[j][idx];
        if(indegree[idx] == 0)
            Enqueue(queue, idx);
    }

    while(queue->qsize != 0) {
        del = Dequeue(queue);
        sorted_arr[count] = G->node[del];
        for(i = 0; i < G->size; i++) {
            idx = order[i];
            if(G->matrix[del][idx] != 0) {
                if(--indegree[idx] == 0)
                    Enqueue(queue, idx);    // indegree값이 0이면 enqueue
            }
        }
        count += 1;
    }

    fprintf(output, "TopSort Result: ");
    if(sorted_arr) {
        for(i = 0; i < G->size; i++)
            fprintf(output, "%d ", sorted_arr[i]);
    }
    else {
        fprintf(output, "This graph is not a DAG.\n");
    }
}

void swap(int* a, int* b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

Queue* MakeNewQueue(int X) {
    Queue* queue = malloc(sizeof(Queue));
    queue->key = malloc(sizeof(int) * X);
    queue->max_queue_size = X;
    queue->first = 1;
    queue->rear = 0;
    queue->qsize = 0;
    return queue;
}

void delete_queue(Queue* queue) {
    free(queue->key);
    free(queue);
}

void Enqueue(Queue* Q, int X) {
    if(Q->qsize == Q->max_queue_size)
        return;
    else {
        Q->rear += 1;
        Q->key[Q->rear] = X;
        Q->qsize++;
    }
}

int Dequeue(Queue* Q) {
    if(Q->qsize == 0)
        return -1;
    else {
        int delete_key = Q->key[Q->first];
        Q->first += 1;
        Q->qsize--;
        return delete_key;
    }
}

void print_matrix(FILE* output, Graph* graph) {
    fprintf(output, "Adjacency matrix\n");
    fprintf(output, "  ");

    int i, j;
    for(i = 0; i < graph->size; i++)
        fprintf(output, "%d ", graph->node[i]);
    fprintf(output, "\n");

    for(i = 0; i < graph->size; i++) {
        fprintf(output, "%d ", graph->node[i]);
        for(j = 0; j < graph->size; j++)
            fprintf(output, "%d ", graph->matrix[i][j]);
        fprintf(output, "\n");
    }
    fprintf(output, "\n\n");
}

void delete_graph(Graph* graph) {
    int i;
    free(graph->node);
    // delete the 2D matrix
    for(i = 0; i < graph->size; i++)
        free(graph->matrix[i]);
    free(graph->matrix);
    free(graph);    // Good bye, graph!
}

int* sorted_order(int* arr, int n) {
    // returns the array that contains indexes by which can access in sorted order of the value.
    int i, j;

    int* result = malloc(sizeof(int) * n);
    int* tmp = malloc(sizeof(int) * n);
    for(i = 0; i < n; i++) {
        result[i] = i;
        tmp[i] = arr[i];
    }
    
    for(i = 0; i < n - 1; i++) {
        for(j = 0; j < i; j++) {
            if(tmp[j] > tmp[j + 1]) {
                swap(&result[j], &result[j + 1]);
                swap(&tmp[j], &tmp[j + 1]);
            }
        }
    }
    free(tmp);
    return result;
}