#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DB_FILENAME "members.dat"
typedef struct {
    int id;                     // unique student ID
    char fullName[100];         // full name
    char batch[30];             // CS, SE, Cyber Security, AI
    char membershipType[10];    // IEEE or ACM
    char registrationDate[11];  // YYYY-MM-DD (10 chars + '\0')
    char dob[11];               // YYYY-MM-DD
    char interest[10];          // IEEE / ACM / Both
} Student;
Student *students = NULL;
size_t studentCount = 0;
size_t studentCapacity = 0;

void ensureCapacity();
void pushStudentInMemory(const Student *s);
int findIndexByID(int id);
int checkDuplicateID(int id);
void readLine(char *buffer, size_t size);
void clearInputBuffer();

void loadDatabase(const char *filename);
int saveDatabase(const char *filename);
int addStudent(Student s, const char *filename);
int updateStudent(int studentID);                 
int deleteStudent(int studentID);                 

void registerStudent();
void displayAllStudents();
void showStudent(const Student *s);
void generateBatchReport();
void searchByID();
void searchByName();
void giveStressTestSuggestions();
void freeDatabase();

void ensureCapacity() {
    if (studentCapacity == 0) {
        studentCapacity = 8;
        students = malloc(studentCapacity * sizeof(Student));
        if (!students) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
    } else if (studentCount >= studentCapacity) {
        studentCapacity *= 2;
        Student *tmp = realloc(students, studentCapacity * sizeof(Student));
        if (!tmp) {
            fprintf(stderr, "Memory reallocation failed.\n");
            free(students);
            exit(EXIT_FAILURE);
        }
        students = tmp;
    }
}

void pushStudentInMemory(const Student *s) {
    ensureCapacity();
    students[studentCount++] = *s;
}

int findIndexByID(int id) {
    for (size_t i = 0; i < studentCount; ++i) {
        if (students[i].id == id) return (int)i;
    }
    return -1;
}

int checkDuplicateID(int id) {
    return findIndexByID(id) != -1;
}

void readLine(char *buffer, size_t size) {
    if (!fgets(buffer, (int)size, stdin)) {
        // input error
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void loadDatabase(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("No existing database found. A new database will be created at '%s'.\n", filename);
        return;
    }

    Student tmp;
    while (fread(&tmp, sizeof(Student), 1, fp) == 1) {
        pushStudentInMemory(&tmp);
    }

    if (ferror(fp)) {
        fprintf(stderr, "Error reading from database file '%s'.\n", filename);
    }

    fclose(fp);
}

int saveDatabase(const char *filename) {
    FILE *fp = fopen(filename, "wb"); // overwrite
    if (!fp) {
        fprintf(stderr, "Failed to open '%s' for writing.\n", filename);
        return 0;
    }

    if (studentCount > 0) {
        size_t written = fwrite(students, sizeof(Student), studentCount, fp);
        if (written != studentCount) {
            fprintf(stderr, "Failed to write all records to '%s'.\n", filename);
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;
}
  
int addStudent(Student s, const char *filename) {
    if (checkDuplicateID(s.id)) {
        printf("Error: Student ID %d already exists. Aborting add.\n", s.id);
        return 0;
    }

    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        fprintf(stderr, "Failed to open '%s' for appending.\n", filename);
        return 0;
    }

    if (fwrite(&s, sizeof(Student), 1, fp) != 1) {
        fprintf(stderr, "Failed to append record to '%s'.\n", filename);
        fclose(fp);
        return 0;
    }
    fclose(fp);

    pushStudentInMemory(&s);
    return 1;
}

int updateStudent(int studentID) {
    int idx = findIndexByID(studentID);
    if (idx == -1) return 0;

    printf("Found student:\n");
    showStudent(&students[idx]);

    printf("\nWhat would you like to update?\n");
    printf("1. Batch\n2. Membership Type\nEnter choice: ");
    int ch;
    if (scanf("%d", &ch) != 1) { clearInputBuffer(); return 0; }
    clearInputBuffer();

    if (ch == 1) {
        printf("Enter new Batch (CS / SE / Cyber Security / AI): ");
        readLine(students[idx].batch, sizeof(students[idx].batch));
    } else if (ch == 2) {
        printf("Enter new Membership Type (IEEE / ACM): ");
        readLine(students[idx].membershipType, sizeof(students[idx].membershipType));
    } else {
        printf("Invalid choice.\n");
        return 0;
    }

    if (!saveDatabase(DB_FILENAME)) {
        fprintf(stderr, "Failed to save updated database.\n");
        return 0;
    }

    printf("Record updated and saved successfully.\n");
    return 1;
}

int deleteStudent(int studentID) {
    int idx = findIndexByID(studentID);
    if (idx == -1) return 0;

    printf("Deleting the following record:\n");
    showStudent(&students[idx]);
    printf("Are you sure? (y/n): ");
    char confirm = getchar(); clearInputBuffer();
    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled.\n");
        return 0;
    }

    for (size_t i = idx + 1; i < studentCount; ++i) {
        students[i - 1] = students[i];
    }
    studentCount--;

    if (!saveDatabase(DB_FILENAME)) {
        fprintf(stderr, "Failed to save database after deletion.\n");
        return 0;
    }

    printf("Record deleted and database saved.\n");
    return 1;
}


void showStudent(const Student *s) {
    printf("ID: %d\n", s->id);
    printf("Name: %s\n", s->fullName);
    printf("Batch: %s\n", s->batch);
    printf("Membership Type: %s\n", s->membershipType);
    printf("Registration Date: %s\n", s->registrationDate);
    printf("Date of Birth: %s\n", s->dob);
    printf("Interest: %s\n", s->interest);
}

void displayAllStudents() {
    if (studentCount == 0) {
        printf("\nNo student records to display.\n");
        return;
    }

    printf("\n---- All Student Registrations (%zu) ----\n", studentCount);
    printf("%-6s | %-25s | %-15s | %-8s | %-10s | %-10s | %-6s\n",
           "ID", "Name", "Batch", "MemType", "RegDate", "DOB", "Int");
    printf("--------------------------------------------------------------------------------------\n");
    for (size_t i = 0; i < studentCount; ++i) {
        printf("%-6d | %-25s | %-15s | %-8s | %-10s | %-10s | %-6s\n",
               students[i].id,
               students[i].fullName,
               students[i].batch,
               students[i].membershipType,
               students[i].registrationDate,
               students[i].dob,
               students[i].interest);
    }
}

void registerStudent() {
    Student s;
    printf("\n---- Register New Student ----\n");

    printf("Enter Student ID (integer): ");
    while (scanf("%d", &s.id) != 1) {
        printf("Invalid input. Enter an integer for Student ID: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    if (checkDuplicateID(s.id)) {
        printf("Error: ID %d already exists. Registration aborted.\n", s.id);
        return;
    }

    printf("Enter Full Name: ");
    readLine(s.fullName, sizeof(s.fullName));

    printf("Enter Batch (CS / SE / Cyber Security / AI): ");
    readLine(s.batch, sizeof(s.batch));

    printf("Enter Membership Type (IEEE / ACM): ");
    readLine(s.membershipType, sizeof(s.membershipType));

    printf("Enter Registration Date (YYYY-MM-DD): ");
    readLine(s.registrationDate, sizeof(s.registrationDate));

    printf("Enter Date of Birth (YYYY-MM-DD): ");
    readLine(s.dob, sizeof(s.dob));

    printf("Interest In (IEEE / ACM / Both): ");
    readLine(s.interest, sizeof(s.interest));

    if (addStudent(s, DB_FILENAME)) {
        printf("Student registered and saved successfully.\n");
    } else {
        printf("Failed to register student.\n");
    }
}

void generateBatchReport() {
    if (studentCount == 0) {
        printf("No records to report.\n");
        return;
    }

    char reqBatch[30], reqMem[10];
    printf("Enter Batch for report (CS / SE / Cyber Security / AI): ");
    readLine(reqBatch, sizeof(reqBatch));
    printf("Enter Membership filter (IEEE / ACM / Both / All): ");
    readLine(reqMem, sizeof(reqMem));

    printf("\nReport - Batch: %s | Membership: %s\n", reqBatch, reqMem);
    printf("%-6s | %-25s | %-15s | %-8s | %-10s\n", "ID", "Name", "Batch", "MemType", "RegDate");
    printf("--------------------------------------------------------------------------\n");

    int found = 0;
    for (size_t i = 0; i < studentCount; ++i) {
        int batchMatch = strcmp(students[i].batch, reqBatch) == 0;
        int memMatch = 0;
        if (strcmp(reqMem, "All") == 0) memMatch = 1;
        else if (strcmp(reqMem, "Both") == 0) memMatch = (strcmp(students[i].interest, "Both") == 0);
        else memMatch = (strcmp(students[i].membershipType, reqMem) == 0);

        if (batchMatch && memMatch) {
            printf("%-6d | %-25s | %-15s | %-8s | %-10s\n",
                   students[i].id,
                   students[i].fullName,
                   students[i].batch,
                   students[i].membershipType,
                   students[i].registrationDate);
            found = 1;
        }
    }

    if (!found) printf("No records matched the report criteria.\n");
}

void searchByID() {
    printf("Enter Student ID to search: ");
    int id;
    if (scanf("%d", &id) != 1) { clearInputBuffer(); printf("Invalid input.\n"); return; }
    clearInputBuffer();

    int idx = findIndexByID(id);
    if (idx == -1) {
        printf("Student with ID %d not found.\n", id);
    } else {
        printf("Record found:\n");
        showStudent(&students[idx]);
    }
}

void searchByName() {
    char name[100];
    printf("Enter full name (case-sensitive): ");
    readLine(name, sizeof(name));

    int found = 0;
    for (size_t i = 0; i < studentCount; ++i) {
        if (strcmp(students[i].fullName, name) == 0) {
            printf("\nRecord found:\n");
            showStudent(&students[i]);
            found = 1;
        }
    }
    if (!found) printf("No record matched the name '%s'.\n", name);
}

void freeDatabase() {
    if (students) free(students);
    students = NULL;
    studentCount = studentCapacity = 0;
}

void giveStressTestSuggestions() {
    printf("\n--- Stress Test Suggestions ---\n");
    printf("1. Use the 'Register New Student' option to add 20-30 records across batches.\n");
    printf("2. Use 'Delete Student' to remove 5-10 random records.\n");
    printf("3. Use 'Update Student' to update 5 records (batch/membership).\n");
    printf("4. After each operation, ensure 'View all registrations' shows correct data.\n");
    printf("5. Verify 'members.dat' file size grows and content is consistent (optional).\n");
    printf("6. To fully verify, exit and restart program; data should reload from file.\n\n");
}

int main() {
    loadDatabase(DB_FILENAME);

    int choice;
    do {
        printf("\n====== IEEE/ACM Membership System (FAST University) ======\n");
        printf("1. Register a new student\n");
        printf("2. Update membership type or batch (by ID)\n");
        printf("3. Delete a student registration (by ID)\n");
        printf("4. View all registrations\n");
        printf("5. Generate batch-wise report\n");
        printf("6. Search by ID\n");
        printf("7. Search by Name\n");
        printf("8. Stress Test Suggestions\n");
        printf("9. Exit (save & quit)\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number from 1 to 9.\n");
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                registerStudent();
                break;
            case 2: {
                printf("Enter Student ID to update: ");
                int id;
                if (scanf("%d", &id) != 1) { clearInputBuffer(); printf("Invalid input.\n"); break; }
                clearInputBuffer();
                if (!updateStudent(id)) printf("Update failed (ID not found or error).\n");
                break;
            }
            case 3: {
                printf("Enter Student ID to delete: ");
                int id;
                if (scanf("%d", &id) != 1) { clearInputBuffer(); printf("Invalid input.\n"); break; }
                clearInputBuffer();
                if (!deleteStudent(id)) printf("Deletion failed (ID not found or error).\n");
                break;
            }
            case 4:
                displayAllStudents();
                break;
            case 5:
                generateBatchReport();
                break;
            case 6:
                searchByID();
                break;
            case 7:
                searchByName();
                break;
            case 8:
                giveStressTestSuggestions();
                break;
            case 9:
                if (!saveDatabase(DB_FILENAME)) {
                    fprintf(stderr, "Warning: Could not save database before exit.\n");
                } else {
                    printf("Database saved to '%s'.\n", DB_FILENAME);
                }
                break;
            default:
                printf("Invalid choice. Please choose 1-9.\n");
        }

    } while (choice != 9);

    freeDatabase();
    printf("Goodbye.\n");
    return 0;
}
