#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_DATA 100

struct Book {
	char code[20];
	char name[100];
	char type[20];
	float price;
	int stock;
};

struct Transaction {
	char trxCode[10];
	char bookName[100];
	int quantity;
    float totalPrice;
};

struct Book books[MAX_DATA]; 
int bookCount = 0;

struct Transaction transactions[MAX_DATA];
int trxCount = 0;

// Prototype Helper function 
// Start
bool backToMenu();

void toTitleCase(char *input);

void toUpperCase(char *str);

bool isBookCodeExist(const char *code);
// End
// Prototype Helper function 

// Prototype Function for book
// Start
void addBook(const char *code, const char *name, const char *type, float price, int stock);

void updateDataQtyBook();

void updateStockBook(const int idx, int stock);

void clearBooks();

void readDataBooks();

void viewBooks();

void insertDataBook();

void deleteBook();
// End
// Prototype function for book


// Prototype function for transaction
// Start
void addTrx(const char *trxCode, const char *bookName, int quantity, float totalPrice);

void clearHistory();

void readDataTrx();

void viewHistoryTransaction();

void deleteHistoryTransaction();

int findMaxTrxNumber();

void buyBook();
// End
// Prototype function for transaction


int main()
{
	while (true) {
		printf("================ Program Pencatatan Pembelian Buku ===============\n");
		int choice;
		printf("1. Input Data Buku\n");
        printf("2. Update Stock Buku\n");
		printf("3. View History\n");
		printf("4. View Buku\n");
		printf("5. Delete History\n");
		printf("6. Delete Buku\n");
		printf("7. Pembelian Buku\n");
		printf("8. Exit\n");
		printf("Pilih menu ? [1-8]\t: ");
		scanf("%d", &choice);
		switch (choice)
		{
			case 1:
				insertDataBook();
				break;
            case 2:
                updateDataQtyBook();
                break;
			case 3:
				viewHistoryTransaction();
				break;
			case 4:
				viewBooks();
				break;
			case 5:
				deleteHistoryTransaction();
                break;
			case 6:
				deleteBook();
				break;
			case 7:
				buyBook();
				break;
			case 8:
				return 0;
			default:
				break;
		}	

		if (!backToMenu()) {
            break;
        }
	}

	return 0;
}

// Helper Function
// Start

bool backToMenu() {
    printf("Back to menu ? :\n");
    printf("1. Menu\n");
    printf("2. Exit\n");
    printf("Choice ? [1-2] : ");
    int choice;
    scanf("%d", &choice);
    fflush(stdin);

    switch (choice) {
        case 1:
            return true; // Kembali ke menu
        default:
            return false; // Keluar dari program
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

bool isBookCodeExist(const char *code) {
    for (int i = 0; i < bookCount; i++) {
        if (strcmp(code, books[i].code) == 0) {
            return true; 
        }
    }
    return false;
}
// End
// Helper Function


// Book Section Code
// Start

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
    if(fsBook == NULL) {
        printf("failed open file!\n");
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

	if(fs == NULL) {
		printf("failed open file!\n");
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

// End
// Book Section Code

// Transaction Section Code
// Start

void addTrx(const char *trxCode, const char *bookName, int quantity, float totalPrice) {
	if(trxCount < MAX_DATA) {
		struct Transaction trx;
		strcpy(trx.trxCode, trxCode);
		strcpy(trx.bookName, bookName);
		trx.quantity = quantity;
        trx.totalPrice = totalPrice;

		transactions[trxCount++] = trx;
	} else {
		printf("Jumlah data yang dapat disimpan sudah tercapai!\n");
	}
}

void clearHistory() {
    for (int i = 0; i < trxCount; i++) {    
        strcpy(transactions[i].bookName, "");
        strcpy(transactions[i].trxCode, "");
        transactions[i].quantity = 0;
        transactions[i].totalPrice = 0.0;
    }
    
    trxCount = 0;
}

void readDataTrx() {
	clearHistory();
	int i = 0;
    struct Transaction trx;
	char date[11], time[9];
    FILE *fs = fopen("database\\transactionHistory.txt", "r");

	if(fs == NULL){
		printf("failed open file!\n");
	}

	while (!feof(fs)) {
		if (fscanf(fs, " %9[^,], %19[^,], %d, %f\n", trx.trxCode, trx.bookName, &trx.quantity, &trx.totalPrice) == 4) {
			addTrx(trx.trxCode, trx.bookName, trx.quantity, trx.totalPrice);
		} else {
			int c;
			while ((c = fgetc(fs)) != '\n' && c != EOF);
		}
	}

    fclose(fs);
}

void viewHistoryTransaction() {
    readDataTrx();
	printf("=========================== History ===========================\n");
    printf("No.   %-11s %-11s %8s %19s\n", "Kode Trx", "Nama Buku", "Jumlah", "Total Harga");
    printf("===============================================================\n");

    if(trxCount > 0) {
        for (int i = 0; i < trxCount; i++) {
            printf("%d.   %-12s %-12s %8d %6s Rp.%.2f\n", i + 1, transactions[i].trxCode, transactions[i].bookName, transactions[i].quantity, " ",transactions[i].totalPrice);
        }
    } else {
        printf("\t\t\t  No Data\n");
    }

    printf("===============================================================\n");
}

void deleteHistoryTransaction() {
	printf("\n================ Menu Delete History Transaction ==============\n");
    int choice;
    printf("1. Delete All Data\n");
    printf("2. Delete One Data\n");
    printf("Choice ? [1-2]: ");
    scanf("%d", &choice);
    fflush(stdin);

    switch (choice) {
        case 1:
            FILE *fs = fopen("database\\transactionHistory.txt", "w");
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
                viewHistoryTransaction();
                int deletedIdx;
                if(trxCount > 0) {
                    printf("\nMasukkan nomor transaksi yang ingin dihapus\n");
                    printf("Nomor transaksi ? [1-%d]\t: ", trxCount);
                    scanf("%d", &deletedIdx);  
                    deletedIdx--;
                } else {
                    break;
                }

                fs = fopen("database\\transactionHistory.txt", "w");
                if (fs == NULL) {
                    printf("Gagal membuka file untuk penulisan.\n");
                    return;
                }

                int deleted = 0;

                for (int i = 0; i < trxCount; i++) {
                    if (i == deletedIdx) {
                        deleted = 1;
                    } else {
                        fprintf(fs, "%s,%s,%d,%.2f\n", transactions[i].trxCode, transactions[i].bookName, transactions[i].quantity, transactions[i].totalPrice);
                    }
                }

                fclose(fs);

                if (deleted) {
                    printf("Transaksi dengan kode trx'%s' telah dihapus.\n", transactions[deletedIdx].trxCode);
                } else {
                    printf("Transaksi dengan kode trx '%s' tidak ditemukan dalam database.\n", transactions[deletedIdx].trxCode);
                } 

                
                printf("Ingin menghapus lagi ? [y\\n] : ");
                char choice;
                scanf(" %c", &choice);

                if (choice != 'y' && choice != 'Y') {
                    deleteAgain = false;
                }
            } while (deleteAgain);
            printf("========================================\n");
            break;

        default:
            printf("Pilihan tidak valid!\n");
            break;
    }
}

int findMaxTrxNumber() {
	FILE *file = fopen("database\\transactionHistory.txt", "r");

    int maxTrxNumber = 0;
    char line[100];  // Sesuaikan dengan panjang maksimum baris dalam file

    while (fgets(line, sizeof(line), file) != NULL) {
        int trxNumber;
        if (sscanf(line, "TRX%04d,", &trxNumber) == 1 && trxNumber > maxTrxNumber) {
            maxTrxNumber = trxNumber;
        }
    }
	fclose(file);
    return maxTrxNumber;
}

void buyBook() {
	int choice;
	int quantity;
	printf("============================ Menu Pembelian Buku ============================\n\n");
    bool buyAgain = true;
    do {
        viewBooks();
        printf("pilih buku ? [1-%d]\t: ", bookCount);
        scanf("%d", &choice);
        bool invalidQuantity = true;
        do {
            printf("Masukan Jumlah buku\t: ");
            scanf("%d", &quantity);

            if(quantity <= books[choice-1].stock) {
                invalidQuantity = false;
            } else {
                printf("Jumlah buku yang dimasukan melebihi stock yang ada.\n");
                printf("Silahkan input kembali!\n");
            }

        } while (invalidQuantity);
        

        choice--;
        if(choice >= 0 && choice < bookCount) {
            struct Transaction newTrx;
            char trxCode[10];
            int maxTrxNumber = findMaxTrxNumber();
            maxTrxNumber++;
            sprintf(newTrx.trxCode, "TRX%04d", maxTrxNumber);
            strcpy(newTrx.bookName, books[choice].name);
            newTrx.quantity = quantity;
            newTrx.totalPrice = quantity * books[choice].price;

            addTrx(newTrx.bookName, newTrx.trxCode, newTrx.quantity, newTrx.totalPrice);
            updateStockBook(choice, books[choice].stock - newTrx.quantity);

            FILE *fsBook = fopen("database\\books.txt", "w");
            if(fsBook == NULL) {
                printf("failed open file!\n");
            }

            for (int i = 0; i < bookCount; i++) {
                fprintf(fsBook, "%s,%s,%s,%.2f,%d\n", books[i].code, books[i].name, books[i].type, books[i].price, books[i].stock);
            }

            fclose(fsBook);

            FILE *fsTransaction = fopen("database\\transactionHistory.txt", "a");
            if(fsTransaction == NULL) {
                printf("failed open file!\n");
            }

            
            fprintf(fsTransaction, "%s,%s,%d,%.2f\n", newTrx.trxCode, newTrx.bookName, newTrx.quantity, newTrx.totalPrice);
            fclose(fsTransaction);

            printf("\nBuku yang dibeli\t: \n");
            printf("Nama\t\t\t: %s\n", newTrx.bookName);
            printf("Jumlah\t\t\t: %d\n", newTrx.quantity);
            printf("Total\t\t\t: Rp.%.2f\n\n", newTrx.totalPrice);

            printf("Ingin membeli lagi ? [y\\n] : ");
            char choice;
            scanf(" %c", &choice);

            if (choice != 'y' && choice != 'Y') {
                buyAgain = false;
            }
        }


    } while (buyAgain);

    printf("=========================== Berhasil membeli buku! ==========================\n");
    printf("=============================================================================\n");
}

// End
// Transaction Section Code