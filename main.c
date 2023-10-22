#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#include "functions/book.h"
#include "functions/transaction.h"

#define MAX_DATA 100

// Prototype Helper function 
bool backToMenu();

// Main Function
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

// Function untuk menanyakan user kembali ke menu atau keluar
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