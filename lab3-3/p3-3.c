#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 100

typedef struct {
    int size;
    int list[MAX_SIZE];
}Stack;

Stack new_stack() {
    Stack stack;
    stack.size = 0;
    return stack;
}

int is_empty(Stack* stack) {
    return stack->size < 1;
}

int top(Stack* stack) {
    return stack->list[stack->size - 1];
}
void push(Stack* stack, int data_) {
    if (stack->size >= MAX_SIZE)
        return;
    stack->list[stack->size++] = data_;
}

void pop(Stack* stack, int* s) {
    if (stack->size <= 0)
        return;
    stack->size -= 1;
    if (s != NULL)
        *s = stack->list[stack->size];
}
int show_precedence(char oper) {
    if (oper == '(' || oper == ')')
        return 0;
    else if (oper == '+' || oper == '-')
        return 1;
    else if (oper == '%' || oper == '*' || oper == '/')
        return 2;
}

int operate(char oper, int a, int b) {
    switch (oper) {
    case '+':
        return a + b;
        break;
    case '-':
        return a - b;
        break;
    case '*':
        return a * b;
        break;
    case '/':
        return a / b;
        break;
    case '%':
        return a % b;
        break;
    default:
        break;
    }
}

void make_postfix(char* infix, char* postfix) {
    int i = -1;
    int j = 0;
    Stack operators = new_stack();
    int s;

    while (infix[++i] != '#') {
        // if it is digit
        if ((infix[i] >= '0') && (infix[i] <= '9'))
            postfix[j++] = infix[i];

        // if it is left parenthesis
        else if (infix[i] == '(')
            push(&operators, '(');

        // if it is right parenthesis
        else if (infix[i] == ')') {
            // pop all the operators until reaching the corresponding left parenthesis
            while (top(&operators) != '(') {
                postfix[j++] = top(&operators);
                pop(&operators, NULL);
            }
            pop(&operators, NULL); // pop the left parenthesis
        }
        else if (is_empty(&operators) || show_precedence(infix[i]) > show_precedence(top(&operators))) {
            push(&operators, infix[i]);
        }
        else {
            while (show_precedence(infix[i]) <= show_precedence(top(&operators))) {
                pop(&operators, &s);
                postfix[j++] = s;
            }
            push(&operators, infix[i]);
        }
    }
    while (!is_empty(&operators)) {
        pop(&operators, &s);
        postfix[j++] = s;
    }
}

int calculate_postfix(char* postfix) {
    int size = strlen(postfix);
    Stack cal_post = new_stack();
    for (int i = 0; i < size; i++) {
        if (postfix[i] >= '0' && postfix[i] <= '9')
            push(&cal_post, postfix[i] - '0');
        else {
            int a, b;
            pop(&cal_post, &a);
            pop(&cal_post, &b);
            push(&cal_post, operate(postfix[i], b, a));
        }
    }
    return top(&cal_post);
}

int main(void) {
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");
    char infix[MAX_SIZE + 1] = { 0, };
    char postfix[MAX_SIZE + 1] = { 0, };

    fscanf(input, "%s", infix);
    make_postfix(infix, postfix);

    int idx = -1;
    while (infix[++idx] != '#') {}
    infix[idx] = 0;
    fprintf(output, "Infix Form : %s\n", infix);
    fprintf(output, "Postfix Form : %s\n", postfix);
    fprintf(output, "Evaluation Result : %d\n", calculate_postfix(postfix));

    fclose(input);
    fclose(output);
    return 0;
}