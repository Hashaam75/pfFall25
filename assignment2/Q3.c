#include <stdio.h>

#define ROWS 3
#define COLS 3

#define POWER 0
#define OVERLOAD 1
#define MAINTENANCE 2

void updateSector(int grid[ROWS][COLS]);
void querySector(int grid[ROWS][COLS]);
void diagnosticReport(int grid[ROWS][COLS]);
void showStatus(int status);

int main() {
    int grid[ROWS][COLS] = {0};
    int choice;

    do {
        printf("\n--- IESCO GRID CONTROL PANEL ---\n");
        printf("1. Update Sector Status\n");
        printf("2. Check Sector Status\n");
        printf("3. Run Diagnostic Report\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                updateSector(grid);
                break;
            case 2:
                querySector(grid);
                break;
            case 3:
                diagnosticReport(grid);
                break;
            case 4:
                printf("\nSystem shutting down...\n");
                break;
            default:
                printf("Invalid choice, try again.\n");
        }
    } while (choice != 4);

    return 0;
}

void updateSector(int grid[ROWS][COLS]) {
    int row, col, flag, action;

    printf("\nEnter sector coordinates (row col): ");
    scanf("%d %d", &row, &col);

    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        printf("Invalid coordinates.\n");
        return;
    }

    printf("Select status to modify:\n");
    printf("0 - Power\n");
    printf("1 - Overload\n");
    printf("2 - Maintenance\n");
    printf("Enter flag: ");
    scanf("%d", &flag);

    if (flag < 0 || flag > 2) {
        printf("Invalid option.\n");
        return;
    }

    printf("Enter action (1 = Set, 0 = Clear): ");
    scanf("%d", &action);

    if (action == 1)
        grid[row][col] |= (1 << flag);
    else
        grid[row][col] &= ~(1 << flag);

    printf("Sector (%d, %d) updated.\n", row, col);
}

void querySector(int grid[ROWS][COLS]) {
    int row, col;

    printf("\nEnter sector coordinates (row col): ");
    scanf("%d %d", &row, &col);

    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        printf("Invalid coordinates.\n");
        return;
    }

    int status = grid[row][col];
    printf("\nSector (%d, %d): Status Code = %d (Binary: ", row, col, status);
    for (int i = 2; i >= 0; i--) {
        printf("%d", (status >> i) & 1);
    }
    printf(")\n");

    showStatus(status);
}

void showStatus(int status) {
    printf("Power: %s\n", (status & (1 << POWER)) ? "ON" : "OFF");
    printf("Overload: %s\n", (status & (1 << OVERLOAD)) ? "YES" : "NO");
    printf("Maintenance: %s\n", (status & (1 << MAINTENANCE)) ? "Required" : "Not Required");
}

void diagnosticReport(int grid[ROWS][COLS]) {
    int overloaded = 0, maintenance = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int status = grid[i][j];
            if (status & (1 << OVERLOAD))
                overloaded++;
            if (status & (1 << MAINTENANCE))
                maintenance++;
        }
    }

    printf("\n--- SYSTEM DIAGNOSTIC REPORT ---\n");
    printf("Total sectors: %d\n", ROWS * COLS);
    printf("Overloaded: %d\n", overloaded);
    printf("Maintenance Required: %d\n", maintenance);
}
