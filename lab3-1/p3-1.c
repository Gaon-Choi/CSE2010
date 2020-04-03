#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXIMUM_SIZE 100

typedef struct Node_ {
    int data;
    struct Node_ *next;
    int size;
} Node;

Node* make_node() {
    Node* new_node = malloc(sizeof(Node));
    new_node->data = 0;
    new_node->next = NULL;
    new_node->size = 0;
}

void push(Node* list, FILE* input, FILE* output) {
    if (list->size == MAXIMUM_SIZE) {
        fprintf(output, "Full\n");
        return;
    }
    int data_;
    fscanf(input, "%d", &data_);
    Node* node = make_node();
    node->data = data_;
    if (list->next == NULL)
        list->next = node;
    else {
        Node* second = list->next;
        list->next = node;
        node->next = second;
    }
    list->size += 1;
}

void pop(Node* list, FILE* input, FILE* output) {
    if (list->next == NULL)
        fprintf(output, "Empty\n");
    else {
        Node* del_node = list->next;
        fprintf(output, "%d\n", del_node->data);
        list->next = del_node->next;
        free(del_node);
        list->size -= 1;
    }
}
int main(void) {
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");
    int count = 0;

    Node* my_stack = make_node();
    fscanf(input, "%d", &count);
    char opt[5];
    while (fscanf(input, "%s", opt) == 1) {
        if (!strcmp(opt, "push"))
            push(my_stack, input, output);
        else if (!strcmp(opt, "pop"))
            pop(my_stack, input, output);
    }

    while (my_stack != NULL) {
        Node* node = my_stack->next;
        free(my_stack);
        my_stack = node;
    }

    fclose(input);
    fclose(output);
    return 0;
}