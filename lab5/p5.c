#include <stdio.h>
#include <stdlib.h>
typedef struct TreeNode_* TreePtr;

typedef struct TreeNode_ {
    int value;
    struct TreeNode_* left;
    struct TreeNode_* right;
} TreeNode;

TreePtr make_node(int data) {
    TreePtr node = malloc(sizeof(TreeNode));
    node->value = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

TreePtr InsertNode(int data, FILE* output, TreePtr root) {
    // if the root doesn't have value in it
    if (root == NULL) {
        TreePtr temp = make_node(data);
        return temp;
    }
    if (data < root->value)
        root->left = InsertNode(data, output, root->left);
    else if (data > root->value)
        root->right = InsertNode(data, output, root->right);
    else
        fprintf(output, "%d already exists.\n", data);
    return root;
}

TreePtr Find_max(TreePtr root) {
    if (root == NULL)
        return NULL;
    else if (root->right == NULL)
        return root;
    else
        return Find_max(root->right);
}

TreePtr FindNode(int value, FILE* output, TreePtr root) {
    TreePtr temp = root;
    if (root == NULL) {
        fprintf(output, "%d is not in the tree.\n", value);
        return NULL;
    }

    if (value < root->value) {
        temp = temp->left;
        return FindNode(value, output, temp);
    }
    else if (value > root->value) {
        temp = temp->right;
        return FindNode(value, output, temp);
    }
    else {
        fprintf(output, "%d is in the tree.\n", value);
        return root;
    }
}

TreePtr DeleteNode(int data, FILE* output, TreePtr root) {
    // if the data doesn't exist in the tree
    if (root == NULL) {
        fprintf(output, "Deletion failed. %d does not exist.\n", data);
        return root;
    }
    // if the data is smaller
    if (data < root->value)
        root->left = DeleteNode(data, output, root->left);
    // if the data is greater
    else if (data > root->value)
        root->right = DeleteNode(data, output, root->right);
    // if the data is found
    else {
        // if the node has only one child
        if (root->left == NULL) {
            TreePtr temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            TreePtr temp = root->left;
            free(root);
            return temp;
        }
        // if the node has two children
        // find the greatest value in the left subtree
        TreePtr temp = Find_max(root->left);
        root->value = temp->value;
        root->left = DeleteNode(temp->value, output, root->left);
    }
    return root;
}

void delete_tree(TreePtr root) {
    if (root == NULL) return;
    delete_tree(root->left);
    delete_tree(root->right);
    free(root);
}

void PrintInorder(FILE* output, TreePtr root) {
    // implemented recursively
    if (root == NULL) return;
    PrintInorder(output, root->left);
    fprintf(output, "%d ", root->value);
    PrintInorder(output, root->right);
}

void PrintPreorder(FILE* output, TreePtr root) {
    // implemented recursively
    if (root == NULL) return;
    fprintf(output, "%d ", root->value);
    PrintPreorder(output, root->left);
    PrintPreorder(output, root->right);
}

void PrintPostorder(FILE* output, TreePtr root) {
    // implemented recursively
    if (root == NULL) return;
    PrintPostorder(output, root->left);
    PrintPostorder(output, root->right);
    fprintf(output, "%d ", root->value);
}

int main(void) {
    // File I/O
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");

    // create empty BST
    // Binary Search Tree
    TreePtr root = NULL;

    char select[3] = { 0, };
    int data_ = 0;
    while (fscanf(input, "%s", select) == 1) {
        switch (select[0]) {
            // Insertion
        case 'i':
            fscanf(input, "%d", &data_);
            root = InsertNode(data_, output, root);
            break;
            // Deletion
        case 'd':
            fscanf(input, "%d", &data_);
            root = DeleteNode(data_, output, root);
            break;
            // Find
        case 'f':
            fscanf(input, "%d", &data_);
            root = FindNode(data_, output, root);
            break;
            // Traverse
        case 'p': {
            fprintf(output, "%s - ", select);
            switch (select[1]) {
                // traverse inorder
            case 'i':
                PrintInorder(output, root);
                fprintf(output, "\n");
                break;
                // traverse preorder
            case 'r':
                PrintPreorder(output, root);
                fprintf(output, "\n");
                break;
                // traverse postorder
            case 'o':
                PrintPostorder(output, root);
                fprintf(output, "\n");
                break;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
    }

    // free the BST
    delete_tree(root);

    // file close
    fclose(input);
    fclose(output);
    return 0;
}