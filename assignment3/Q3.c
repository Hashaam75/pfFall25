#include <stdio.h>
#include <string.h>

struct Employee {
    int id;
    char name[50];
    char designation[50];
    float salary;
};
void displayEmployees(struct Employee emp[], int n);
void findHighestSalary(struct Employee emp[], int n);
void searchEmployee(struct Employee emp[], int n);

int main() {
    int n;

    printf("Enter number of employees: ");
    scanf("%d", &n);

    struct Employee emp[n];

    printf("\n---- Enter Employee Records ----\n");
    for (int i = 0; i < n; i++) {
        printf("\nEmployee %d:\n", i + 1);
        printf("Enter ID: ");
        scanf("%d", &emp[i].id);

        printf("Enter Name: ");
        scanf("%s", emp[i].name);

        printf("Enter Designation: ");
        scanf("%s", emp[i].designation);

        printf("Enter Salary: ");
        scanf("%f", &emp[i].salary);
    }

    int choice;
    do {
        printf("\n\n---- MENU ----\n");
        printf("1. Display All Employees\n");
        printf("2. Find Employee With Highest Salary\n");
        printf("3. Search Employee (ID or Name)\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                displayEmployees(emp, n);
                break;

            case 2:
                findHighestSalary(emp, n);
                break;

            case 3:
                searchEmployee(emp, n);
                break;

            case 4:
                printf("Exiting program...\n");
                break;

            default:
                printf("Invalid option!\n");
        }

    } while(choice != 4);

    return 0;
}

void displayEmployees(struct Employee emp[], int n) {
    printf("\n\n---- Employee Records ----\n");
    printf("%-10s %-20s %-20s %-10s\n", "ID", "Name", "Designation", "Salary");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%-10d %-20s %-20s %-10.2f\n",
               emp[i].id, emp[i].name, emp[i].designation, emp[i].salary);
    }
}
void findHighestSalary(struct Employee emp[], int n) {
    int index = 0;
    for (int i = 1; i < n; i++) {
        if (emp[i].salary > emp[index].salary)
            index = i;
    }

    printf("\nEmployee With Highest Salary:\n");
    printf("ID: %d\n", emp[index].id);
    printf("Name: %s\n", emp[index].name);
    printf("Designation: %s\n", emp[index].designation);
    printf("Salary: %.2f\n", emp[index].salary);
}

void searchEmployee(struct Employee emp[], int n) {
    int option;
    printf("\nSearch by:\n1. ID\n2. Name\nEnter option: ");
    scanf("%d", &option);

    if (option == 1) {
        int id;
        printf("Enter Employee ID: ");
        scanf("%d", &id);

        for (int i = 0; i < n; i++) {
            if (emp[i].id == id) {
                printf("\nRecord Found:\n");
                printf("ID: %d\n", emp[i].id);
                printf("Name: %s\n", emp[i].name);
                printf("Designation: %s\n", emp[i].designation);
                printf("Salary: %.2f\n", emp[i].salary);
                return;
            }
        }
        printf("Employee not found!\n");
    }

    else if (option == 2) {
        char name[50];
        printf("Enter Employee Name: ");
        scanf("%s", name);

        for (int i = 0; i < n; i++) {
            if (strcmp(emp[i].name, name) == 0) { // case-sensitive match
                printf("\nRecord Found:\n");
                printf("ID: %d\n", emp[i].id);
                printf("Name: %s\n", emp[i].name);
                printf("Designation: %s\n", emp[i].designation);
                printf("Salary: %.2f\n", emp[i].salary);
                return;
            }
        }
        printf("Employee not found!\n");
    }

    else {
        printf("Invalid search option!\n");
    }
}
