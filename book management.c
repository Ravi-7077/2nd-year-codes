#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 1000
#define MAX_NAME 100
#define MAX_AUTHOR 100
#define FILENAME_BOOKS "books.txt"
#define FILENAME_ISSUED "issued_books.txt"

// Structs for book and issued book
typedef struct {
    int id;
    char name[MAX_NAME];
    char author[MAX_AUTHOR];
    int available; // 1 if available, 0 if issued
} Book;

typedef struct {
    int book_id;
    char issued_to[MAX_NAME];
} IssuedBook;

// Global arrays for books and issued books
Book books[MAX_BOOKS];
IssuedBook issued_books[MAX_BOOKS];
int book_count = 0;
int issued_count = 0;

// Function prototypes
void add_book();
void view_books();
void issue_book();
void return_book();
void save_to_file();
void load_from_file();
void display_menu();

int main() {
    load_from_file(); // Load data from files at startup
    int choice;

    do {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_book();
                break;
            case 2:
                view_books();
                break;
            case 3:
                issue_book();
                break;
            case 4:
                return_book();
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
    printf("\n============== Library Management System ==============\n");
    printf("1. Add Book\n");
    printf("2. View All Books\n");
    printf("3. Issue Book\n");
    printf("4. Return Book\n");
    printf("5. Exit\n");
    printf("=======================================================\n");
}

// Function to add a book
void add_book() {
    if (book_count >= MAX_BOOKS) {
        printf("Error: Book list is full.\n");
        return;
    }

    Book b;
    b.id = book_count + 1; // Generate unique book ID
    printf("Enter book name: ");
    getchar(); // Clear buffer
    fgets(b.name, MAX_NAME, stdin);
    strtok(b.name, "\n"); // Remove newline

    printf("Enter author name: ");
    fgets(b.author, MAX_AUTHOR, stdin);
    strtok(b.author, "\n");

    b.available = 1; // New books are available by default
    books[book_count++] = b;

    printf("Book added successfully! Book ID: %d\n", b.id);
}

// Function to view all books
void view_books() {
    if (book_count == 0) {
        printf("No books in the library.\n");
        return;
    }

    printf("\n================== All Books ==================\n");
    printf("%-5s %-30s %-20s %10s\n", "ID", "Name", "Author", "Status");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < book_count; i++) {
        printf("%-5d %-30s %-20s %10s\n", books[i].id, books[i].name, books[i].author,
               books[i].available ? "Available" : "Issued");
    }
    printf("=======================================================\n");
}

// Function to issue a book
void issue_book() {
    int book_id;
    char issued_to[MAX_NAME];

    printf("Enter book ID to issue: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > book_count || !books[book_id - 1].available) {
        printf("Error: Invalid book ID or book not available.\n");
        return;
    }

    printf("Enter name of the person issuing the book: ");
    getchar(); // Clear buffer
    fgets(issued_to, MAX_NAME, stdin);
    strtok(issued_to, "\n");

    books[book_id - 1].available = 0; // Mark book as issued
    IssuedBook ib = {book_id, ""};
    strcpy(ib.issued_to, issued_to);
    issued_books[issued_count++] = ib;

    printf("Book issued successfully to %s!\n", issued_to);
}

// Function to return a book
void return_book() {
    int book_id;
    printf("Enter book ID to return: ");
    scanf("%d", &book_id);

    if (book_id < 1 || book_id > book_count || books[book_id - 1].available) {
        printf("Error: Invalid book ID or book is not issued.\n");
        return;
    }

    books[book_id - 1].available = 1; // Mark book as available

    // Remove from issued_books array
    for (int i = 0; i < issued_count; i++) {
        if (issued_books[i].book_id == book_id) {
            for (int j = i; j < issued_count - 1; j++) {
                issued_books[j] = issued_books[j + 1];
            }
            issued_count--;
            break;
        }
    }

    printf("Book returned successfully!\n");
}

// Function to save data to files
void save_to_file() {
    FILE *file_books = fopen(FILENAME_BOOKS, "w");
    if (!file_books) {
        printf("Error: Could not open file to save books.\n");
        return;
    }

    for (int i = 0; i < book_count; i++) {
        fprintf(file_books, "%d|%s|%s|%d\n", books[i].id, books[i].name, books[i].author, books[i].available);
    }
    fclose(file_books);

    FILE *file_issued = fopen(FILENAME_ISSUED, "w");
    if (!file_issued) {
        printf("Error: Could not open file to save issued books.\n");
        return;
    }

    for (int i = 0; i < issued_count; i++) {
        fprintf(file_issued, "%d|%s\n", issued_books[i].book_id, issued_books[i].issued_to);
    }
    fclose(file_issued);

    printf("Data saved successfully!\n");
}

// Function to load data from files
void load_from_file() {
    FILE *file_books = fopen(FILENAME_BOOKS, "r");
    if (file_books) {
        char line[256];
        while (fgets(line, sizeof(line), file_books)) {
            Book b;
            sscanf(line, "%d|%[^|]|%[^|]|%d", &b.id, b.name, b.author, &b.available);
            books[book_count++] = b;
        }
        fclose(file_books);
        printf("Books loaded successfully from %s.\n", FILENAME_BOOKS);
    }

    FILE *file_issued = fopen(FILENAME_ISSUED, "r");
    if (file_issued) {
        char line[256];
        while (fgets(line, sizeof(line), file_issued)) {
            IssuedBook ib;
            sscanf(line, "%d|%[^\n]", &ib.book_id, ib.issued_to);
            issued_books[issued_count++] = ib;
        }
        fclose(file_issued);
        printf("Issued books loaded successfully from %s.\n", FILENAME_ISSUED);
    }
}
