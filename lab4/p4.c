#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct ThreadedTree_* Threaded_ptr;
typedef struct ThreadedTree_
{
    char data;
    short int left_thread;
    short int right_thread;
    struct ThreadedTree_* left_child;
    struct ThreadedTree_* right_child;
} ThreadedTree;

Threaded_ptr new_node(char data_)
{
    // create a new ThreadedNode
    Threaded_ptr node = (Threaded_ptr)malloc(sizeof(ThreadedTree));
    node->left_thread = TRUE;
    node->left_child = node;
    node->right_thread = TRUE;
    node->right_child = node;
    node->data = data_;
    return node;
}

Threaded_ptr new_root()
{
    // create a new threaded binary tree root
    Threaded_ptr root = new_node('\0');
    root->right_thread = FALSE;
    return root;
}

Threaded_ptr inserting_point(Threaded_ptr tree)
{
    Threaded_ptr tmp = tree->right_child;
    if (tmp->data == '\0'||tree->right_thread == FALSE)
    {
        while (tmp->left_thread == FALSE)
            tmp = tmp->left_child;
    }
    return tmp;
}

void insert_node(Threaded_ptr node, Threaded_ptr tree)
{
    if (tree->left_thread == TRUE)
    {
        node->left_child = tree->left_child;
        node->right_child = tree;
        tree->left_child = node;
        tree->left_thread = FALSE;
    }
    else if (tree->right_thread == TRUE)
    {
        node->right_child = tree->right_child;
        node->left_child = tree;
        tree->right_child = node;
        tree->right_thread = FALSE;
    }
}

Threaded_ptr InsertNode(Threaded_ptr node, Threaded_ptr tree)
{
    Threaded_ptr tmp = inserting_point(tree);

    while (tmp->left_thread == FALSE && tmp->right_thread == FALSE)
        tmp = inserting_point(tmp);
    insert_node(node, tmp);
    return node;
}

void traverse_inorder(FILE* output, Threaded_ptr tree)
{
    Threaded_ptr temp = tree;
    while (temp->left_child != tree)
        temp = temp->left_child;
    while (1)
    {
        fprintf(output, "%c ", temp->data);
        temp = inserting_point(temp);
        if (temp->left_child == tree)
            break;
    }
}

int main(void)
{
    // file I/O
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");

    // get the node info
    int size;
    char data, data_;
    fscanf(input, "%d", &size);
    
    // if the size exceeds the limit, quit
    if (size > 100)
        return 0;
    // delete the '\0'
    fscanf(input, "%c", &data_);

    // add a threaded binary tree
    Threaded_ptr head = new_root();
    Threaded_ptr last_thing = head;

    // insert nodes to the tree
    for (int i = 0; i < size; i++)
    {
        fscanf(input, "%c ", &data);
        Threaded_ptr node = new_node(data);
        last_thing = InsertNode(node, last_thing);
    }

    // traverse inorder
    traverse_inorder(output, head);
    fprintf(output, "\n");
    // file closing
    fclose(input);
    fclose(output);
    return 0;
}