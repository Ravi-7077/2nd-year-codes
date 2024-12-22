#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EXPENSES 1000
#define MAX_DESCRIPTION 100
#define MAX_CATEGORY 50
#define FILENAME "expenses.txt"

// Struct to store expense details
typedef struct {
    char category[MAX_CATEGORY];
    char description[MAX_DESCRIPTION];
    double amount;
} Expense;

// Global array to store expenses
Expense expenses[MAX_EXPENSES];
int expense_count = 0;

// Function prototypes
void add_expense();
void view_expenses();
void summary_report();
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
                add_expense();
                break;
            case 2:
                view_expenses();
                break;
            case 3:
                summary_report();
                break;
            case 4:
                save_to_file();
                break;
            case 5:
                printf("Exiting program. Goodbye!\n");
                save_to_file(); // Save data before exiting
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// Function to display the main menu
void display_menu() {
    printf("\n================ Expense Tracker ================\n");
    printf("1. Add Expense\n");
    printf("2. View Expenses\n");
    printf("3. Summary Report\n");
    printf("4. Save Expenses to File\n");
    printf("5. Exit\n");
    printf("================================================\n");
}

// Function to add an expense
void add_expense() {
    if (expense_count >= MAX_EXPENSES) {
        printf("Error: Expense list is full.\n");
        return;
    }

    Expense e;
    printf("Enter category: ");
    getchar(); // Clear newline from buffer
    fgets(e.category, MAX_CATEGORY, stdin);
    strtok(e.category, "\n"); // Remove trailing newline

    printf("Enter description: ");
    fgets(e.description, MAX_DESCRIPTION, stdin);
    strtok(e.description, "\n");

    printf("Enter amount: ");
    scanf("%lf", &e.amount);

    expenses[expense_count++] = e;
    printf("Expense added successfully!\n");
}

// Function to view all expenses
void view_expenses() {
    if (expense_count == 0) {
        printf("No expenses to display.\n");
        return;
    }

    printf("\n================ All Expenses ================\n");
    printf("%-20s %-30s %10s\n", "Category", "Description", "Amount");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < expense_count; i++) {
        printf("%-20s %-30s %10.2f\n", expenses[i].category, expenses[i].description, expenses[i].amount);
    }
    printf("===================================================\n");
}

// Function to display a summary report
void summary_report() {
    if (expense_count == 0) {
        printf("No expenses to summarize.\n");
        return;
    }

    typedef struct {
        char category[MAX_CATEGORY];
        double total;
    } CategorySummary;

    CategorySummary summaries[MAX_EXPENSES];
    int summary_count = 0;

    // Calculate totals for each category
    for (int i = 0; i < expense_count; i++) {
        int found = 0;
        for (int j = 0; j < summary_count; j++) {
            if (strcmp(expenses[i].category, summaries[j].category) == 0) {
                summaries[j].total += expenses[i].amount;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(summaries[summary_count].category, expenses[i].category);
            summaries[summary_count].total = expenses[i].amount;
            summary_count++;
        }
    }

    // Display the summary
    printf("\n================ Expense Summary ================\n");
    printf("%-20s %10s\n", "Category", "Total Amount");
    printf("------------------------------------------------\n");
    for (int i = 0; i < summary_count; i++) {
        printf("%-20s %10.2f\n", summaries[i].category, summaries[i].total);
    }
    printf("=================================================\n");
}

// Function to save expenses to a file
void save_to_file() {
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        printf("Error: Could not open file for saving.\n");
        return;
    }

    for (int i = 0; i < expense_count; i++) {
        fprintf(file, "%s|%s|%.2f\n", expenses[i].category, expenses[i].description, expenses[i].amount);
    }
    fclose(file);
    printf("Expenses saved successfully to %s.\n", FILENAME);
}

// Function to load expenses from a file
void load_from_file() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        printf("No previous expense data found. Starting fresh.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Expense e;
        char *token = strtok(line, "|");
        if (token) strcpy(e.category, token);

        token = strtok(NULL, "|");
        if (token) strcpy(e.description, token);

        token = strtok(NULL, "|");
        if (token) e.amount = atof(token);

        expenses[expense_count++] = e;
    }
    fclose(file);
    printf("Expenses loaded successfully from %s.\n", FILENAME);
}
