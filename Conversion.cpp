#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

typedef struct Node {
    char data;
    struct Node* next;
} Node;

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void push(Node** top, char data) {
    Node* newNode = createNode(data);
    newNode->next = *top;
    *top = newNode;
}

char pop(Node** top) {
    if (*top == NULL) return '\0';
    Node* temp = *top;
    char data = temp->data;
    *top = (*top)->next;
    free(temp);
    return data;
}

char peek(Node* top) {
    if (top == NULL) return '\0';
    return top->data;
}

int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

int precedence(char ch) {
    if (ch == '+' || ch == '-') return 1;
    if (ch == '*' || ch == '/') return 2;
    return 0;
}

void infixToPostfix(char* infix, char* postfix) {
    Node* stack = NULL;
    int k = 0;
    for (int i = 0; infix[i]; ++i) {
        if (isdigit(infix[i]) || isalpha(infix[i])) {
            postfix[k++] = infix[i];
        } else if (infix[i] == '(') {
            push(&stack, infix[i]);
        } else if (infix[i] == ')') {
            while (stack && peek(stack) != '(') {
                postfix[k++] = pop(&stack);
            }
            pop(&stack);
        } else if (isOperator(infix[i])) {
            while (stack && precedence(peek(stack)) >= precedence(infix[i])) {
                postfix[k++] = pop(&stack);
            }
            push(&stack, infix[i]);
        }
    }
    while (stack) {
        postfix[k++] = pop(&stack);
    }
    postfix[k] = '\0';
}

void postfixToInfix(char* postfix, char* infix) {
    Node* stack = NULL;
    char result[MAX][MAX];
    int k = 0;
    for (int i = 0; postfix[i]; ++i) {
        if (isdigit(postfix[i]) || isalpha(postfix[i])) {
            result[k][0] = postfix[i];
            result[k][1] = '\0';
            push(&stack, k++);
        } else if (isOperator(postfix[i])) {
            int val2 = pop(&stack);
            int val1 = pop(&stack);
            snprintf(result[k], MAX, "(%s%c%s)", result[val1], postfix[i], result[val2]);
            push(&stack, k++);
        }
    }
    strcpy(infix, result[pop(&stack)]);
}

void displayMenu() {
    printf("\nConversion\n");
    printf("---------------------------\n");
    printf("1. Convert Infix to Postfix\n");
    printf("2. Convert Postfix to Infix\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    char infix[MAX], postfix[MAX], convertedInfix[MAX];
    int choice;

    while (1) {
        displayMenu();
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                printf("Enter infix expression: ");
                fgets(infix, MAX, stdin);
                infix[strcspn(infix, "\n")] = '\0';

                infixToPostfix(infix, postfix);
                printf("Postfix expression: %s\n", postfix);
                break;
            case 2:
                printf("Enter postfix expression: ");
                fgets(postfix, MAX, stdin);
                postfix[strcspn(postfix, "\n")] = '\0';

                postfixToInfix(postfix, convertedInfix);
                printf("Infix expression from postfix: %s\n", convertedInfix);
                break;
            case 3:
                printf("Bye\n");
                exit(0);
        }
    }

    return 0;
}

