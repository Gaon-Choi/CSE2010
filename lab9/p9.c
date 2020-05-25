#include <stdio.h>
#include <stdlib.h>
#define ORDER 3

typedef struct B_node {
    int n_keys;
    struct B_node *child[ORDER];
    int key[ORDER - 1];
} B_node;

B_node *root = NULL;

enum KeyStatus {
    Success,
    InsertIt
};

int searchPos(int key, int *key_arr, int n) {
    int pos = 0;
    while (pos < n && key > key_arr[pos])
        pos++;
    return pos;
}

enum KeyStatus ins(B_node *ptr, int key, int *upKey, B_node **newnode) {
    B_node *newPtr, *lastPtr;
    int pos, i, n, splitPos;
    int newKey, lastKey;
    enum KeyStatus value;
    if (ptr == NULL) {
        *newnode = NULL;
        *upKey = key;
        return InsertIt;
    }
    n = ptr->n_keys;
    pos = searchPos(key, ptr->key, n);
    value = ins(ptr->child[pos], key, &newKey, &newPtr);
    if (value != InsertIt)
        return value;
    if (n < ORDER - 1) {
        pos = searchPos(newKey, ptr->key, n);
        for (i = n; i > pos; i--) {
            ptr->key[i] = ptr->key[i - 1];
            ptr->child[i + 1] = ptr->child[i];
        }
        ptr->key[pos] = newKey;
        ptr->child[pos + 1] = newPtr;
        ++ptr->n_keys;
        return Success;
    }
    if (pos == ORDER - 1) {
        lastKey = newKey;
        lastPtr = newPtr;
    }
    else {
        lastKey = ptr->key[ORDER - 2];
        lastPtr = ptr->child[ORDER - 1];
        for (i = ORDER - 2; i > pos; i--) {
            ptr->key[i] = ptr->key[i - 1];
            ptr->child[i + 1] = ptr->child[i];
        }
        ptr->key[pos] = newKey;
        ptr->child[pos + 1] = newPtr;
    }
    splitPos = (ORDER - 1) / 2;
    (*upKey) = ptr->key[splitPos];

    (*newnode) = malloc(sizeof(B_node));
    ptr->n_keys = splitPos;
    (*newnode)->n_keys = ORDER - 1 - splitPos;
    for (i = 0; i < (*newnode)->n_keys; i++) {
        (*newnode)->child[i] = ptr->child[i + splitPos + 1];
        if (i < (*newnode)->n_keys - 1)
            (*newnode)->key[i] = ptr->key[i + splitPos + 1];
        else
            (*newnode)->key[i] = lastKey;
    }
    (*newnode)->child[(*newnode)->n_keys] = lastPtr;
    return InsertIt;
}

void insert(int key) {
    B_node *newnode;
    int upKey;
    enum KeyStatus value;
    value = ins(root, key, &upKey, &newnode);
    if (value == InsertIt) {
        B_node *uproot = root;
        root = malloc(sizeof(B_node));
        root->n_keys = 1;
        root->key[0] = upKey;
        root->child[0] = uproot;
        root->child[1] = newnode;
    }
}

void inorder(B_node *ptr, FILE *output) {
    if (ptr == NULL)
        return;
    int i;
    for (i = 0; i < ptr->n_keys; i++) {
        inorder(ptr->child[i], output);
        fprintf(output, "%d ", ptr->key[i]);
    }
    inorder(ptr->child[ptr->n_keys], output);
}

int main(void) {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    char opt;
    int data;

    while (fscanf(input, "%c", &opt) == 1) {
        switch (opt) {
        case 'i':
            fscanf(input, "%d", &data);
            insert(data);
            break;
        case 'p':
            inorder(root, output);
	    fprintf(output, "\n");
            break;
        default:
            break;
        }
    }

    return 0;
}
