/*
 * Header file for transaction operations.
 * Defines the Transaction structure and prototypes for monetary operations.
 */

#ifndef TRANSACTION_H
#define TRANSACTION_H

// Structure to hold transaction data
typedef struct {
    char date[20];
    char time[20];
    int accountNumber;
    char transactionType[20]; // Deposit, Withdrawal, Transfer In, Transfer Out
    double amount;
    double balanceAfter;
} Transaction;

// Function prototypes for transaction operations
void depositMoney(void);
void withdrawMoney(void);
void transferMoney(void);
void viewTransactions(void);

#endif /* TRANSACTION_H */
