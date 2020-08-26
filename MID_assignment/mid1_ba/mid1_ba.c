#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode_ *TreePtr;

typedef struct TreeNode_ {
    int value;
    struct TreeNode_ *list; // for the duplicated key

    struct TreeNode_ *left;
    struct TreeNode_ *right;
} TreeNode;

TreePtr make_node(int data) {
    TreePtr node = malloc(sizeof(TreeNode));
    node->value = data;
    node->list = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreePtr inserting_point(TreePtr list_) {
    while (list_->list)
        list_ = list_->list;
    return list_;
}

TreePtr Find_max(TreePtr root) {
    if (root == NULL)
        return NULL;
    else if (root->right == NULL)
        return root;
    else
        return Find_max(root->right);
}

TreePtr Insert(TreePtr T, int k) {
    // if the root doesn't have value in it
    if (T == NULL) {
        TreePtr temp = make_node(k);
        return temp;
    }
    if (k < T->value)
        T->left = Insert(T->left, k);
    else if (k > T->value)
        T->right = Insert(T->right, k);
    // when k == T->value
    else {
        if (T->list == NULL)
            T->list = make_node(k);
        else
            inserting_point(T)->list = make_node(k);
    }
    return T;
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
    else if (T->list != NULL) {
        TreePtr tmp = T;
        while (tmp->list->list != NULL)
            tmp = tmp->list;
        TreePtr s = tmp->list;
        free(s);
        tmp->list = NULL;
    }
    // if the node has only one child
    else if (T->left == NULL) {
        TreePtr temp = T->right;
        free(T);
        return temp;
    }
    else if (T->right == NULL) {
        TreePtr temp = T->left;
        free(T);
        return temp;
    }
    else {
        // if the node has two children
        // find the greatest value in the left subtree
        TreePtr temp = Find_max(T->left);
        T->value = temp->value;
        T->list = temp->list;
        temp->list = NULL;
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

void PrintInorder(TreePtr T, FILE *output) {
    // implemented recursively
    if (T == NULL)
        return;
    PrintInorder(T->left, output);
    fprintf(output, "%d ", T->value);
    PrintInorder(T->right, output);
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
            PrintInorder(root, output);
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