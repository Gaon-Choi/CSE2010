#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXIMUM_SIZE 100

typedef struct Node_ {
    int data;
    struct Node_* next;
    int size;
} Node;

Node* make_node() {
    Node* new_node = malloc(sizeof(Node));
    new_node->data = 0;
    new_node->next = NULL;
    new_node->size = 0;
}

Node* get_end(Node* list) {
    Node* end = list->next; // assuming head
    while(end->next != NULL)
        end = end->next;
    return end;
}

void enqueue(Node* list, FILE* input, FILE* output) {
    int data_;
    fscanf(input, "%d", &data_);
    if(list->size == MAXIMUM_SIZE) {
        fprintf(output, "Full\n");
        return;
    }
    Node* node = make_node();
    node->data = data_;
    if(list->next == NULL) {
        list->next = node;
    }
    else {
        Node* second = list->next;
        list->next = node;
        node->next = second;
    }
    list->size++;
}
void dequeue(Node* list, FILE* output) {
    if(list->next == NULL) {
        fprintf(output, "Empty\n");
        return;
    }
    Node* prev = list;
    while(prev->next != get_end(list))
        prev = prev->next;
    list = get_end(list);
    prev->next = NULL;
    fprintf(output, "%d\n", list->data);
    free(list);
    list->size--;
}

int main(void) {
    int count;
    char opt[4];
    
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");
    Node* my_queue = make_node();

    fscanf(input, "%d", &count);
    while(fscanf(input, "%s", opt)==1) {
        if(!strcmp(opt, "enQ"))
            enqueue(my_queue, input, output);

        else if(!strcmp(opt, "deQ"))
            dequeue(my_queue, output);
    }

    fclose(input);
    fclose(output);

    return 0;
}