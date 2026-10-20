/*
 * Header file for account management.
 * Defines the Account structure and prototypes for account operations.
 */

#ifndef ACCOUNT_H
#define ACCOUNT_H

// Structure to hold account data
typedef struct {
    int accountNumber;
    char name[100];
    char phone[20];
    char address[150];
    char accountType[20];
    double balance;
    char dateCreated[20];
} Account;

// Function prototypes for account operations
void createAccount(void);
void displayAccounts(void);
void searchAccount(void);
void balanceInquiry(void);
void updateAccount(void);
void deleteAccount(void);

#endif /* ACCOUNT_H */
