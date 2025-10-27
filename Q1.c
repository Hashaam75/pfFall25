#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 100

void addBook(int isbns[], char titles[][50], float prices[], int quantities[], int *count);
void processSale(int isbns[], char titles[][50], float prices[], int quantities[], int count);
void generateLowStockReport(int isbns[], char titles[][50], float prices[], int quantities[], int count);
int findBookByISBN(int isbns[], int count, int isbn);

int main() {
    int isbns[MAX_BOOKS];
    char titles[MAX_BOOKS][50];
    float prices[MAX_BOOKS];
    int quantities[MAX_BOOKS];
    int count = 0;  
    int choice;

    do {
        printf("\n=== LIBERTY BOOKS INVENTORY SYSTEM ===\n");
        printf("1. Add New Book\n");
        printf("2. Process a Sale\n");
        printf("3. Generate Low-Stock Report\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                addBook(isbns, titles, prices, quantities, &count);
                break;
            case 2:
                processSale(isbns, titles, prices, quantities, count);
                break;
            case 3:
                generateLowStockReport(isbns, titles, prices, quantities, count);
                break;
            case 4:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}

void addBook(int isbns[], char titles[][50], float prices[], int quantities[], int *count) {
    if (*count >= MAX_BOOKS) {
        printf("Inventory full! Can't add more books.\n");
        return;
    }

    int isbn;
    printf("Enter ISBN: ");
    scanf("%d", &isbn);
    getchar(); 

    
    if (findBookByISBN(isbns, *count, isbn) != -1) {
        printf("Error: A book with this ISBN already exists!\n");
        return;
    }

    isbns[*count] = isbn;
    printf("Enter Book Title: ");
    fgets(titles[*count], sizeof(titles[*count]), stdin);
    titles[*count][strcspn(titles[*count], "\n")] = '\0'; 

    printf("Enter Price: ");
    scanf("%f", &prices[*count]);

    printf("Enter Quantity: ");
    scanf("%d", &quantities[*count]);

    (*count)++;
    printf("Book added successfully!\n");
}

void processSale(int isbns[], char titles[][50], float prices[], int quantities[], int count) {
    if (count == 0) {
        printf("No books in inventory yet!\n");
        return;
    }

    int isbn, sold;
    printf("Enter ISBN of the book sold: ");
    scanf("%d", &isbn);

    int index = findBookByISBN(isbns, count, isbn);
    if (index == -1) {
        printf("Book not found!\n");
        return;
    }

    printf("Enter number of copies sold: ");
    scanf("%d", &sold);

    if (sold <= 0) {
        printf("Invalid number of copies.\n");
        return;
    }

    if (quantities[index] < sold) {
        printf("Error: Not enough stock! Available: %d\n", quantities[index]);
        return;
    }

    quantities[index] -= sold;
    printf("Sale processed successfully!\n");
    printf("Remaining stock of '%s': %d\n", titles[index], quantities[index]);
}

void generateLowStockReport(int isbns[], char titles[][50], float prices[], int quantities[], int count) {
    if (count == 0) {
        printf("No books in inventory.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (quantities[i] < 5) {
            printf("ISBN: %d | Title: %s | Price: %.2f | Quantity: %d\n",
                   isbns[i], titles[i], prices[i], quantities[i]);
            found = 1;
        }
    }

    if (!found)
        printf("All books have sufficient stock.\n");
}

int findBookByISBN(int isbns[], int count, int isbn) {
    for (int i = 0; i < count; i++) {
        if (isbns[i] == isbn)
            return i;
    }
    return -1;
}
