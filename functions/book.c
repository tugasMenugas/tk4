#include "book.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Book books[MAX_DATA];
int bookCount = 0;

bool isBookCodeExist(const char *code) {
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(code, books[i].code) == 0) {
            return true; 
        }
    }
    return false;
}

void addBook(const char *code, const char *name, const char *type, float price, int stock) {
	if(bookCount < MAX_DATA) {
		struct Book newBook;
		strcpy(newBook.code, code);
		strcpy(newBook.name, name);
		strcpy(newBook.type, type);
		newBook.price = price;
		newBook.stock = stock;
		books[bookCount++] = newBook;
	} else {
		printf("Jumlah buku yang dapat disimpan sudah tercapai!\n");
	}
}

void updateDataQtyBook() {
    int idx, newStock;
    printf("==================== Menu Update Stock Buku ===================\n");
    viewBooks();
    printf("Masukan nomor buku\t: ");
    scanf("%d", &idx);
    idx--;

    printf("Masukan jumlah stock\t: ");
    scanf("%d", &newStock);
    updateStockBook(idx, books[idx].stock + newStock);

    FILE *fsBook = fopen("database\\books.txt", "w");

    // Check apakah file exist 
    if(fsBook == NULL) {
        printf("failed open file!\n");
        return;
    }

    for (int i = 0; i < bookCount; i++) {
        fprintf(fsBook, "%s,%s,%s,%.2f,%d\n", books[i].code, books[i].name, books[i].type, books[i].price, books[i].stock);
    }

    fclose(fsBook);
    printf("================== Berhasil Menambahkan Stock =================\n");
}

void updateStockBook(const int idx, int stock){
	struct Book *book = &books[idx];
	book->stock = stock;
}

void clearBooks() {
    for (int i = 0; i < bookCount; i++) {    
        strcpy(books[i].code, "");
        strcpy(books[i].name, "");
        strcpy(books[i].type, "");
        books[i].price = 0.0;
		books[i].stock = 0;
    }
    
    bookCount = 0;
}

void readDataBooks() {
	clearBooks();
    int i = 0;
    struct Book book;
    FILE *fs = fopen("database\\books.txt", "r");

    // Check apakah file exist 
    if(fs == NULL) {
		printf("failed open file!\n");
        return;
	}

    while (!feof(fs)) {
        if (fscanf(fs, " %19[^,], %99[^,], %19[^,], %f, %d\n", book.code, book.name, book.type, &book.price, &book.stock) == 5) {
            addBook(book.code, book.name, book.type, book.price, book.stock);
        } else {
            int c;
            while ((c = fgetc(fs)) != '\n' && c != EOF);
        }
    }

    fclose(fs);
}

void viewBooks() {
    readDataBooks();
	printf("================================= Data Book =================================\n");
    printf("No.   %-8s %-17s %-7s %14s %14s\n", "Kode", "Nama", "Tipe", "Harga", "Stok");
    printf("=============================================================================\n");
    if(bookCount > 0) {
        for (int i = 0; i < bookCount; i++) {
            printf("%d.   %-8s %-18s %-8s %6s Rp.%.2f %8d\n", i + 1, books[i].code, books[i].name, books[i].type, " ", books[i].price, books[i].stock);
        }
    } else {
        printf("\t\t\t    No Data\n");
    }
    printf("=============================================================================\n");
}

void insertDataBook() {
    readDataBooks();
	struct Book book;
	FILE *fs = fopen("database\\books.txt", "a");

    // Check apakah file exist 
	if(fs == NULL) {
		printf("failed open file!\n");
        return;
	}
	printf("================== Menu Input Data Buku ==================\n");

    bool codeExists;
    do {
        codeExists = false;
        printf("Code\t: ");
        scanf("%s", book.code);
        toUpperCase(book.code);
        fflush(stdin);

        if (isBookCodeExist(book.code)) {
            printf("Kode buku sudah ada. Harap masukkan kode yang berbeda.\n");
            fclose(fs);
            codeExists = true;
        }

    } while (codeExists);

	printf("Name\t: ");
	scanf("%[^\n]s", book.name);
    toTitleCase(book.name);
	fflush(stdin);

	printf("Type\t: ");
	scanf("%s", book.type);
	fflush(stdin);

	printf("Price\t: ");
	scanf("%f", &book.price);
	fflush(stdin);

	printf("Stock\t: ");
	scanf("%d", &book.stock);

	fprintf(fs, "%s,%s,%s,%.2f,%d\n", book.code, book.name, book.type, book.price, book.stock);
	fclose(fs);

	printf("================ Successfully insert data ================\n");
}

void deleteBook() {
    printf("\n====================== Menu Delete Book =======================\n");
    int choice;
    printf("1. Delete All Data\n");
    printf("2. Delete One Data\n");
    printf("Choice ? [1-2]: ");
    scanf("%d", &choice);
    fflush(stdin);

    switch (choice) {
        case 1:
            FILE *fs = fopen("database\\books.txt", "w");

            // Check apakah file exist 
            if (fs == NULL) {
                printf("Gagal membuka file untuk penulisan.\n");
                return;
            }
            fclose(fs);
            printf("Semua data dalam file telah dihapus.\n");
            break;

        case 2:
            bool deleteAgain = true;
            do {
                viewBooks();
                int deletedIdx;
                if(bookCount > 0) {
                    printf("\nMasukkan nomor buku yang ingin dihapus\n");
                    printf("Nomor buku : ");
                    scanf("%d", &deletedIdx);  
                    deletedIdx--;
                } else {
                    break;
                }

                fs = fopen("database\\books.txt", "w");

                // Check apakah file exist 
                if (fs == NULL) {
                    printf("Gagal membuka file untuk penulisan.\n");
                    return;
                }

                int deleted = 0;

                for (int i = 0; i < bookCount; i++) {
                    if (i == deletedIdx) {
                        deleted = 1;
                    } else {
                        fprintf(fs, "%s,%s,%s,%.2f,%d\n", books[i].code, books[i].name, books[i].type, books[i].price, books[i].stock);
                    }
                }

                fclose(fs);

                if (deleted) {
                    printf("Buku dengan nama '%s' telah dihapus.\n", books[deletedIdx].name);
                } else {
                    printf("Buku dengan nama '%s' tidak ditemukan dalam database.\n", books[deletedIdx].name);
                }

                printf("Ingin menghapus lagi ? [y\\n] : ");
                char choice;
                scanf(" %c", &choice);

                if (choice != 'y' && choice != 'Y') {
                    deleteAgain = false;
                }

            } while (deleteAgain);
            
            break;

        default:
            printf("Pilihan tidak valid!\n");
            break;
    }
}

// function untuk merubah string menjadi Titlecase 
void toTitleCase(char *input) {
	int length = strlen(input);
    bool capitalizeNext = true;
	int i;
    for (i = 0; i < length; i++) {
    	// jika char tersebut adalah huruf maka akan char pertama akan diubah menjadi uppercase
    	// dan char selanjutkan akan diubah menjadi lowerCase
        if (isalpha(input[i])) {
            if (capitalizeNext) {
                input[i] = toupper(input[i]);
                capitalizeNext = false;
            } else {
                input[i] = tolower(input[i]);
            }
        } else {
            capitalizeNext = true;
        }
    }
}

void toUpperCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}
