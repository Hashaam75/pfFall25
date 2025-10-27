#include <stdio.h>
#include <string.h>

#define MAX_PRODUCTS 4
#define MAX_CART 10

int productCodes[MAX_PRODUCTS] = {1, 2, 3, 4};
int stock[MAX_PRODUCTS] = {50, 10, 20, 8};
float prices[MAX_PRODUCTS] = {100, 200, 300, 150};

int cartCodes[MAX_CART];
int cartQty[MAX_CART];
int cartCount = 0;

char customerName[50];
char customerCNIC[20];

void getCustomerInfo();
void displayInventory();
void addToCart();
void displayBill();
void showInvoice();
int findProductIndex(int code);
float calculateTotal(int applyDiscount);

int main() {
    int choice;

    printf("\n=== SUPERMARKET MANAGEMENT SYSTEM ===\n");

    do {
        printf("\nMain Menu:\n");
        printf("1. Enter Customer Information\n");
        printf("2. Display Inventory\n");
        printf("3. Add Item to Cart\n");
        printf("4. Display Total Bill\n");
        printf("5. Show Invoice\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                getCustomerInfo();
                break;
            case 2:
                displayInventory();
                break;
            case 3:
                addToCart();
                break;
            case 4:
                displayBill();
                break;
            case 5:
                showInvoice();
                break;
            case 6:
                printf("\nThank you for shopping with us. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while (choice != 6);

    return 0;
}

void getCustomerInfo() {
    printf("\nEnter Customer Name: ");
    fgets(customerName, sizeof(customerName), stdin);
    customerName[strcspn(customerName, "\n")] = '\0';

    printf("Enter CNIC Number: ");
    fgets(customerCNIC, sizeof(customerCNIC), stdin);
    customerCNIC[strcspn(customerCNIC, "\n")] = '\0';

    printf("\nCustomer information saved.\n");
}

void displayInventory() {
    printf("\n--- INVENTORY ---\n");
    printf("Code\tStock\tPrice (Rs)\n");
    printf("-----------------------------\n");
    for (int i = 0; i < MAX_PRODUCTS; i++)
        printf("%03d\t\t%d\t\t%.2f\n", productCodes[i], stock[i], prices[i]);
}

int findProductIndex(int code) {
    for (int i = 0; i < MAX_PRODUCTS; i++)
        if (productCodes[i] == code)
            return i;
    return -1;
}

void addToCart() {
    int code, qty;

    displayInventory();

    printf("\nEnter Product Code to Add: ");
    scanf("%d", &code);

    int index = findProductIndex(code);
    if (index == -1) {
        printf("Invalid product code.\n");
        return;
    }

    printf("Enter Quantity: ");
    scanf("%d", &qty);

    if (qty <= 0) {
        printf("Quantity must be greater than 0.\n");
        return;
    }

    if (qty > stock[index]) {
        printf("Only %d units available.\n", stock[index]);
        return;
    }

    stock[index] -= qty;
    cartCodes[cartCount] = code;
    cartQty[cartCount] = qty;
    cartCount++;

    printf("Item added to cart.\n");
}

float calculateTotal(int applyDiscount) {
    float total = 0;
    for (int i = 0; i < cartCount; i++) {
        int index = findProductIndex(cartCodes[i]);
        total += prices[index] * cartQty[i];
    }
    if (applyDiscount)
        total *= 0.75;
    return total;
}

void displayBill() {
    if (cartCount == 0) {
        printf("\nCart is empty.\n");
        return;
    }

    float total = calculateTotal(0);
    printf("\nTotal Bill (without discount): Rs. %.2f\n", total);

    char promo[20];
    printf("Do you have a promo code? (Enter code or 'no'): ");
    scanf("%s", promo);

    if (strcmp(promo, "Eid2025") == 0) {
        float discounted = calculateTotal(1);
        printf("Promo applied! Discounted Bill: Rs. %.2f\n", discounted);
    } else {
        printf("No discount applied.\n");
    }
}

void showInvoice() {
    if (cartCount == 0) {
        printf("\nCart is empty.\n");
        return;
    }

    printf("\n========== INVOICE ==========\n");
    printf("Customer: %s\n", customerName);
    printf("CNIC: %s\n", customerCNIC);
    printf("\nItems Purchased:\n");
    printf("Code\tQty\tPrice\tTotal\n");
    printf("---------------------------------\n");

    float total = 0;
    for (int i = 0; i < cartCount; i++) {
        int index = findProductIndex(cartCodes[i]);
        float subtotal = prices[index] * cartQty[i];
        total += subtotal;
        printf("%03d\t\t%d\t%.2f\t\t%.2f\n", cartCodes[i], cartQty[i], prices[index], subtotal);
    }

    printf("---------------------------------\n");
    printf("Total (no discount): Rs. %.2f\n", total);

    char promo[20];
    printf("\nEnter promo code if any (Eid2025) or 'no': ");
    scanf("%s", promo);

    if (strcmp(promo, "Eid2025") == 0) {
        float discounted = total * 0.75;
        printf("Discounted Total: Rs. %.2f\n", discounted);
    } else {
        printf("Final Total: Rs. %.2f\n", total);
    }

    printf("===============================\n");
}

