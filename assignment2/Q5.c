#include <stdio.h>
#include <string.h>

void encodeMessage(char message[]);
void decodeMessage(char message[]);
void toggleBits(char *ch);
void reverseString(char str[]);

int main() {
    char message[200];
    int choice;

    do {
        printf("\n--- TCS Secure Message Tool ---\n");
        printf("1. Encode Message\n");
        printf("2. Decode Message\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("\nEnter message to encode: ");
                fgets(message, sizeof(message), stdin);
                message[strcspn(message, "\n")] = '\0';
                encodeMessage(message);
                break;

            case 2:
                printf("\nEnter message to decode: ");
                fgets(message, sizeof(message), stdin);
                message[strcspn(message, "\n")] = '\0';
                decodeMessage(message);
                break;

            case 3:
                printf("\nExiting program...\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 3);

    return 0;
}

void toggleBits(char *ch) {
    *ch ^= (1 << 1);
    *ch ^= (1 << 4);
}

void reverseString(char str[]) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void encodeMessage(char message[]) {
    reverseString(message);
    for (int i = 0; i < strlen(message); i++) {
        toggleBits(&message[i]);
    }
    printf("\nEncoded Message: %s\n", message);
}

void decodeMessage(char message[]) {
    for (int i = 0; i < strlen(message); i++) {
        toggleBits(&message[i]);
    }
    reverseString(message);
    printf("\nDecoded Message: %s\n", message);
}
