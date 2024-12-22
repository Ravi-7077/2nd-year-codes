#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 1000
#define MAX_NAME 50
#define FILENAME "students.txt"

// Struct to store student details
typedef struct {
    int id;
    char name[MAX_NAME];
    int age;
    float grade;
} Student;

// Global array to store student records
Student students[MAX_STUDENTS];
int student_count = 0;

// Function prototypes
void add_student();
void view_students();
void update_student();
void delete_student();
void save_to_file();
void load_from_file();
void display_menu();

int main() {
    load_from_file(); // Load data from file at startup
    int choice;

    do {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_student();
                break;
            case 2:
                view_students();
                break;
            case 3:
                update_student();
                break;
            case 4:
                delete_student();
                break;
            case 5:
                save_to_file();
                printf("Data saved successfully. Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// Function to display the main menu
void display_menu() {
    printf("\n================ Student Management System ================\n");
    printf("1. Add Student Record\n");
    printf("2. View All Records\n");
    printf("3. Update Student Record\n");
    printf("4. Delete Student Record\n");
    printf("5. Exit\n");
    printf("===========================================================\n");
}

// Function to add a student record
void add_student() {
    if (student_count >= MAX_STUDENTS) {
        printf("Error: Student list is full.\n");
        return;
    }

    Student s;
    s.id = student_count + 1; // Generate unique student ID
    printf("Enter student name: ");
    getchar(); // Clear buffer
    fgets(s.name, MAX_NAME, stdin);
    strtok(s.name, "\n"); // Remove newline

    printf("Enter age: ");
    scanf("%d", &s.age);

    printf("Enter grade: ");
    scanf("%f", &s.grade);

    students[student_count++] = s;
    printf("Student added successfully! Student ID: %d\n", s.id);
}

// Function to view all student records
void view_students() {
    if (student_count == 0) {
        printf("No student records found.\n");
        return;
    }

    printf("\n================== All Student Records ==================\n");
    printf("%-5s %-20s %-5s %-10s\n", "ID", "Name", "Age", "Grade");
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < student_count; i++) {
        printf("%-5d %-20s %-5d %-10.2f\n", students[i].id, students[i].name, students[i].age, students[i].grade);
    }
    printf("=========================================================\n");
}

// Function to update a student record
void update_student() {
    int id;
    printf("Enter student ID to update: ");
    scanf("%d", &id);

    if (id < 1 || id > student_count) {
        printf("Error: Invalid student ID.\n");
        return;
    }

    Student *s = &students[id - 1];
    printf("Enter new name (current: %s): ", s->name);
    getchar(); // Clear buffer
    fgets(s->name, MAX_NAME, stdin);
    strtok(s->name, "\n");

    printf("Enter new age (current: %d): ", s->age);
    scanf("%d", &s->age);

    printf("Enter new grade (current: %.2f): ", s->grade);
    scanf("%f", &s->grade);

    printf("Student record updated successfully!\n");
}

// Function to delete a student record
void delete_student() {
    int id;
    printf("Enter student ID to delete: ");
    scanf("%d", &id);

    if (id < 1 || id > student_count) {
        printf("Error: Invalid student ID.\n");
        return;
    }

    for (int i = id - 1; i < student_count - 1; i++) {
        students[i] = students[i + 1];
    }
    student_count--;

    printf("Student record deleted successfully!\n");
}

// Function to save student records to a file
void save_to_file() {
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        printf("Error: Could not open file to save data.\n");
        return;
    }

    for (int i = 0; i < student_count; i++) {
        fprintf(file, "%d|%s|%d|%.2f\n", students[i].id, students[i].name, students[i].age, students[i].grade);
    }
    fclose(file);
    printf("Student records saved successfully to %s.\n", FILENAME);
}

// Function to load student records from a file
void load_from_file() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        printf("No previous student data found. Starting fresh.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Student s;
        sscanf(line, "%d|%[^|]|%d|%f", &s.id, s.name, &s.age, &s.grade);
        students[student_count++] = s;
    }
    fclose(file);
    printf("Student records loaded successfully from %s.\n", FILENAME);
}
