#ifndef BOOK_H
#define BOOK_H

#include <stdbool.h>

#define MAX_DATA 100

struct Book {
    char code[20];   // Unique identifier for the book
    char name[100];  // Name/title of the book
    char type[20];   // Category or genre of the book
    float price;     // Price of the book
    int stock;       // Available stock/quantity of the book
};

extern struct Book books[MAX_DATA];
extern int bookCount;

bool isBookCodeExist(const char *code);
void addBook(const char *code, const char *name, const char *type, float price, int stock);

void updateDataQtyBook();

void updateStockBook(const int idx, int stock);

void clearBooks();

void readDataBooks();

void viewBooks();

void insertDataBook();

void deleteBook();

// Book Helpers
void toTitleCase(char *input);

void toUpperCase(char *str);

#endif // BOOK_H