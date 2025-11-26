#include <stdio.h>

#define MAX 100

typedef struct {
    int id;
    int popularity;
    int lastAccess;
    int valid;
} Book;

int findBook(Book shelf[], int capacity, int id);
int findLRU(Book shelf[], int capacity);
void addBook(Book shelf[], int capacity, int *timeCounter, int id, int popularity);
int accessBook(Book shelf[], int capacity, int *timeCounter, int id);

int main() {
    int capacity;
    printf("Enter shelf capacity: ");
    scanf("%d", &capacity);

    Book shelf[MAX] = {0};
    int timeCounter = 1;

    int choice;
    while (1) {
        printf("\n1. ADD book\n2. ACCESS book\n3. Exit\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int id, pop;
            printf("Enter Book ID and Popularity: ");
            scanf("%d %d", &id, &pop);
            addBook(shelf, capacity, &timeCounter, id, pop);
        }
        else if (choice == 2) {
            int id;
            printf("Enter Book ID to access: ");
            scanf("%d", &id);
            int result = accessBook(shelf, capacity, &timeCounter, id);
            if (result == -1)
                printf("Book not found! Returned: -1\n");
            else
                printf("Popularity Score = %d\n", result);
        }
        else if (choice == 3) {
            break;
        }
        else {
            printf("Invalid choice!\n");
        }
    }

    return 0;
}

int findBook(Book shelf[], int capacity, int id) {
    for (int i = 0; i < capacity; i++) {
        if (shelf[i].valid == 1 && shelf[i].id == id)
            return i;
    }
    return -1;
}

int findLRU(Book shelf[], int capacity) {
    int lruIndex = -1;
    int minAccess = 999999;

    for (int i = 0; i < capacity; i++) {
        if (shelf[i].valid == 1) {
            if (shelf[i].lastAccess < minAccess) {
                minAccess = shelf[i].lastAccess;
                lruIndex = i;
            }
        }
    }
    return lruIndex;
}

void addBook(Book shelf[], int capacity, int *timeCounter, int id, int popularity) {
    int idx = findBook(shelf, capacity, id);

    if (idx != -1) {
        shelf[idx].popularity = popularity;
        shelf[idx].lastAccess = (*timeCounter)++;
        printf("Book updated & moved to most recently used.\n");
        return;
    }

    for (int i = 0; i < capacity; i++) {
        if (shelf[i].valid == 0) {
            shelf[i].id = id;
            shelf[i].popularity = popularity;
            shelf[i].lastAccess = (*timeCounter)++;
            shelf[i].valid = 1;
            printf("Book added to empty slot.\n");
            return;
        }
    }

    int lruIndex = findLRU(shelf, capacity);
    printf("Shelf full! Removing LRU book with ID %d\n", shelf[lruIndex].id);

    shelf[lruIndex].id = id;
    shelf[lruIndex].popularity = popularity;
    shelf[lruIndex].lastAccess = (*timeCounter)++;
}

int accessBook(Book shelf[], int capacity, int *timeCounter, int id) {
    int idx = findBook(shelf, capacity, id);

    if (idx == -1)
        return -1;

    shelf[idx].lastAccess = (*timeCounter)++;

    return shelf[idx].popularity;
}
