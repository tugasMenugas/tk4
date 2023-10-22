#include "transaction.h"
#include "book.h"
#include <stdio.h>
#include <string.h>

struct Transaction transactions[MAX_DATA];
int trxCount = 0;


// Transaction Section Code
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

    // Check apakah file exist 
	if(fs == NULL){
		printf("failed open file!\n");
        return;
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
                
                // Check apakah file exist 
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

    // Check apakah file exist 
    if (file == NULL) {
        printf("Gagal membuka file untuk penulisan.\n");
        return 0;
    }

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

            // Check apakah file exist 
            if(fsBook == NULL) {
                printf("failed open file!\n");
                return;
            }

            for (int i = 0; i < bookCount; i++) {
                fprintf(fsBook, "%s,%s,%s,%.2f,%d\n", books[i].code, books[i].name, books[i].type, books[i].price, books[i].stock);
            }

            fclose(fsBook);

            FILE *fsTransaction = fopen("database\\transactionHistory.txt", "a");

            // Check apakah file exist 
            if(fsTransaction == NULL) {
                printf("failed open file!\n");
                return;
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
