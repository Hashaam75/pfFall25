#include <stdio.h>
#define MAX 5

void push(int stack[], int *top);
void pop(int stack[], int *top);
void peek(int stack[], int top);
void display(int stack[], int top);

int main() {
    int stack[MAX];
    int top = -1;
    int choice;

    do {
        printf("\n--- STACK OPERATIONS MENU ---\n");
        printf("1. PUSH\n");
        printf("2. POP\n");
        printf("3. PEEK\n");
        printf("4. DISPLAY\n");
        printf("5. EXIT\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                push(stack, &top);
                break;
            case 2:
                pop(stack, &top);
                break;
            case 3:
                peek(stack, top);
                break;
            case 4:
                display(stack, top);
                break;
            case 5:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while(choice != 5);

    return 0;
}

void push(int stack[], int *top) {
    if (*top == MAX - 1) {
        printf("Stack Overflow! Cannot push more elements.\n");
        return;
    }

    int value;
    printf("Enter value to push: ");
    scanf("%d", &value);
    (*top)++;
    stack[*top] = value;
    printf("%d pushed onto stack.\n", value);
}

void pop(int stack[], int *top) {
    if (*top == -1) {
        printf("Stack Underflow! No elements to pop.\n");
        return;
    }

    printf("%d popped from stack.\n", stack[*top]);
    (*top)--;
}

void peek(int stack[], int top) {
    if (top == -1) {
        printf("Stack is empty.\n");
    } else {
        printf("Top element: %d\n", stack[top]);
    }
}

void display(int stack[], int top) {
    if (top == -1) {
        printf("Stack is empty.\n");
        return;
    }

    printf("Stack elements (top to bottom):\n");
    for (int i = top; i >= 0; i--) {
        printf("%d\n", stack[i]);
    }
}
