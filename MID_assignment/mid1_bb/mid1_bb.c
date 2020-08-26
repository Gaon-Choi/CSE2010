#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode_ *TreePtr;

typedef struct TreeNode_ {
    int value;
    struct TreeNode_ *left;
    struct TreeNode_ *right;
} TreeNode;

TreePtr make_node(int data) {
    TreePtr node = malloc(sizeof(TreeNode));
    node->value = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreePtr Insert(TreePtr T, int k) {
    // if the root doesn't have value in it
    if (T == NULL) {
        TreePtr temp = make_node(k);
        return temp;
    }
    if (k <= T->value)
        T->left = Insert(T->left, k);
    else if (k > T->value)
        T->right = Insert(T->right, k);
    return T;
}

TreePtr Find_max(TreePtr root) {
    if (root == NULL)
        return NULL;
    else if (root->right == NULL)
        return root;
    else
        return Find_max(root->right);
}

void deletion(TreePtr root) {
    while (root->left)
        root = root->left;
    free(root->left);
}

TreePtr Delete(TreePtr T, int k) {
    // if the data doesn't exist in the tree
    if (T == NULL)
        return T;
    // if the data is smaller
    if (k < T->value)
        T->left = Delete(T->left, k);
    // if the data is greater
    else if (k > T->value)
        T->right = Delete(T->right, k);
    // if the data is found
    else {
        deletion(T);
        // if the node has only one child
        if (T->left == NULL) {
            TreePtr temp = T->right;
            free(T);
            return temp;
        }
        else if (T->right == NULL) {
            TreePtr temp = T->left;
            free(T);
            return temp;
        }
        // if the node has two children
        // find the greatest value in the left subtree
        TreePtr temp = Find_max(T->left);
        T->value = temp->value;
        T->left = Delete(T->left, temp->value);
    }
    return T;
}

TreePtr Find(TreePtr T, int k) {
    TreePtr temp = T;
    if (T == NULL)
        return NULL;
    if (k < T->value) {
        temp = temp->left;
        return Find(temp, k);
    }
    else if (k > T->value) {
        temp = temp->right;
        return Find(temp, k);
    }
    return T;
}

void PrintInorder(FILE *output, TreePtr root) {
    // implemented recursively
    if (root == NULL)
        return;
    PrintInorder(output, root->left);
    fprintf(output, "%d ", root->value);
    PrintInorder(output, root->right);
}

int main(void) {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");

    TreePtr root = NULL;
    char opt;
    int data_;

    while (fscanf(input, "%c", &opt) == 1) {
        switch (opt) {
        case 'i':
            fscanf(input, "%d", &data_);
            root = Insert(root, data_);
            break;
        case 'd':
            fscanf(input, "%d", &data_);
            if(Find(root, data_))
                root = Delete(root, data_);
            else
                fprintf(output, "Deletion failed. %d does not exist.\n", data_);
            break;
        case 'f':
            fscanf(input, "%d", &data_);
            if(Find(root, data_))
                fprintf(output, "%d is in the tree.\n", data_);
            else
                fprintf(output, "%d is not in the tree.\n", data_);
            break;
        case 'p':
            PrintInorder(output, root);
            fprintf(output, "\n");
            break;
        default:
            break;
        }
    }

    fclose(input);
    fclose(output);

    return 0;
}