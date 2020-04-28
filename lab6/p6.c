#include <stdio.h>
#include <stdlib.h>

typedef struct AVLNode_* AVLTree;
typedef struct AVLNode_* position;
typedef struct AVLNode_ {
    int element;
    AVLTree left;
    AVLTree right;
    int height;
}AVLNode;

int max(int a, int b) {
    return a >= b ? a : b;
}

int height(position p) {
    if (p == NULL) return -1;
    else return p->height;
}

void traverse_inorder(AVLTree tree, FILE* output) {
    // implemented recursively
    if(tree==NULL) return;
    traverse_inorder(tree->left, output);
    fprintf(output, "%d(%d) ", tree->element, height(tree));
    traverse_inorder(tree->right, output);
}

position single_rotate_left(position pos) {     /* LL */
    position k1;

    k1 = pos->left;
    pos->left = k1->right;
    k1->right = pos;

    pos->height = max(height(pos->left), height(pos->right)) + 1;
    k1->height = max(height(k1->left), pos->height) + 1;
    return k1;
}

position single_rotate_right(position pos) {    /* RR */
    position k2;

    k2 = pos->right;
    pos->right = k2->left;
    k2->left = pos;

    pos->height = max(height(pos->left), height(pos->right)) + 1;
    k2->height = max(height(k2->right), pos->height) + 1;
    return k2;
}

position double_rotate_left(position pos) {     /* LR */
    pos->left = single_rotate_right(pos->left);
    return single_rotate_left(pos);
}

position double_rotate_right(position pos) {    /* RL */
    pos->right = single_rotate_left(pos->right);
    return single_rotate_right(pos);
}

AVLTree insert(int data, AVLTree tree, FILE* output, int* flag) {
    if (tree == NULL) {
        tree = malloc(sizeof(AVLNode));
        if (tree != NULL) {
            tree->element = data;
            tree->height = 0;
            tree->left = tree->right = NULL;
        }
    }
    else if (data < tree->element) {
        tree->left = insert(data, tree->left, output, flag);
        if (height(tree->left) - height(tree->right) == 2)
            if (data < tree->left->element)
                tree = single_rotate_left(tree);
            else
                tree = double_rotate_left(tree);
    }
    else if (data > tree->element) {
        tree->right = insert(data, tree->right, output, flag);
        if (height(tree->right) - height(tree->left) == 2)
            if (data > tree->right->element)
                tree = single_rotate_right(tree);
            else
                tree = double_rotate_right(tree);
    }
    else {
        fprintf(output, "%d already in the tree!\n", data);
        *flag = 0;
    }
    tree->height = max(height(tree->left), height(tree->right)) + 1;
    return tree;
}

void delete_tree(AVLTree root) {
    if (root == NULL)
        return;
    delete_tree(root->left);
    delete_tree(root->right);
    free(root);
}

int main(void) {
    // File I/O
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");
    int data;
    // AVL Tree
    AVLTree tree = NULL;

    // Insert data to the tree
    while (fscanf(input, "%d", &data) == 1) {
        int flag = 1;
        tree = insert(data, tree, output, &flag);
        if (flag) {
            traverse_inorder(tree, output);
            fprintf(output, "\n");
        }
    }

    // Delete the AVL Tree
    delete_tree(tree);
    
    // File Close
    fclose(input);
    fclose(output);
    return 0;
}