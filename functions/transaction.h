#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdbool.h>

#define MAX_DATA 100

struct Transaction {
    char trxCode[10];
    char bookName[100];
    int quantity;
    float totalPrice;
};

extern struct Transaction transactions[MAX_DATA];
extern int trxCount;

// Prototype function for transaction
void addTrx(const char *trxCode, const char *bookName, int quantity, float totalPrice);

void clearHistory();

void readDataTrx();

void viewHistoryTransaction();

void deleteHistoryTransaction();

int findMaxTrxNumber();

void buyBook();

#endif // TRANSACTION_H
