/*
 * Implementation of financial transactions.
 * Handles deposits, withdrawals, transfers, and viewing transaction history.
 */

#include <stdio.h>
#include <string.h>
#include "transaction.h"
#include "account.h"
#include "file.h"
#include "utility.h"

void depositMoney(void) {
    printf("=================================================\n");
    printf("                  DEPOSIT MONEY                  \n");
    printf("=================================================\n");
    
    int accNo = getIntInput("Enter Account Number: ");
    Account acc;

    if (!findAccount(accNo, &acc)) {
        printf("Account not found.\n");
        return;
    }

    printf("Account Holder: %s\n", acc.name);
    printf("Current Balance: $%.2f\n", acc.balance);

    double amount = getDoubleInput("Enter amount to deposit: $");
    if (amount <= 0) {
        printf("Invalid amount. Deposit must be greater than zero.\n");
        return;
    }

    acc.balance += amount;

    if (updateAccountInFile(&acc)) {
        Transaction t;
        getCurrentDate(t.date);
        getCurrentTime(t.time);
        t.accountNumber = acc.accountNumber;
        strcpy(t.transactionType, "Deposit");
        t.amount = amount;
        t.balanceAfter = acc.balance;
        
        recordTransaction(&t);

        printf("\nDeposit successful. New Balance: $%.2f\n", acc.balance);
    } else {
        printf("Failed to process deposit.\n");
    }
}

void withdrawMoney(void) {
    printf("=================================================\n");
    printf("                 WITHDRAW MONEY                  \n");
    printf("=================================================\n");
    
    int accNo = getIntInput("Enter Account Number: ");
    Account acc;

    if (!findAccount(accNo, &acc)) {
        printf("Account not found.\n");
        return;
    }

    printf("Account Holder: %s\n", acc.name);
    printf("Current Balance: $%.2f\n", acc.balance);

    double amount = getDoubleInput("Enter amount to withdraw: $");
    if (amount <= 0) {
        printf("Invalid amount. Withdrawal must be greater than zero.\n");
        return;
    }

    if (amount > acc.balance) {
        printf("Transaction failed. Insufficient funds!\n");
        return;
    }

    acc.balance -= amount;

    if (updateAccountInFile(&acc)) {
        Transaction t;
        getCurrentDate(t.date);
        getCurrentTime(t.time);
        t.accountNumber = acc.accountNumber;
        strcpy(t.transactionType, "Withdrawal");
        t.amount = amount;
        t.balanceAfter = acc.balance;
        
        recordTransaction(&t);

        printf("\nWithdrawal successful. New Balance: $%.2f\n", acc.balance);
    } else {
        printf("Failed to process withdrawal.\n");
    }
}

void transferMoney(void) {
    printf("=================================================\n");
    printf("                 TRANSFER MONEY                  \n");
    printf("=================================================\n");

    int srcAccNo = getIntInput("Enter Sender Account Number: ");
    int destAccNo = getIntInput("Enter Receiver Account Number: ");

    if (srcAccNo == destAccNo) {
        printf("Sender and receiver account numbers cannot be the same.\n");
        return;
    }

    Account srcAcc, destAcc;

    if (!findAccount(srcAccNo, &srcAcc)) {
        printf("Sender account not found.\n");
        return;
    }

    if (!findAccount(destAccNo, &destAcc)) {
        printf("Receiver account not found.\n");
        return;
    }

    printf("\nSender Name: %s (Balance: $%.2f)\n", srcAcc.name, srcAcc.balance);
    printf("Receiver Name: %s\n", destAcc.name);

    double amount = getDoubleInput("\nEnter amount to transfer: $");
    if (amount <= 0) {
        printf("Invalid amount. Transfer must be greater than zero.\n");
        return;
    }

    if (amount > srcAcc.balance) {
        printf("Transaction failed. Insufficient funds in sender's account.\n");
        return;
    }

    // Perform the monetary logic
    srcAcc.balance -= amount;
    destAcc.balance += amount;

    // Update both accounts
    if (updateAccountInFile(&srcAcc) && updateAccountInFile(&destAcc)) {
        // Record Sender Transaction
        Transaction tOut;
        getCurrentDate(tOut.date);
        getCurrentTime(tOut.time);
        tOut.accountNumber = srcAcc.accountNumber;
        strcpy(tOut.transactionType, "Transfer Out");
        tOut.amount = amount;
        tOut.balanceAfter = srcAcc.balance;
        recordTransaction(&tOut);

        // Record Receiver Transaction
        Transaction tIn;
        getCurrentDate(tIn.date);
        getCurrentTime(tIn.time);
        tIn.accountNumber = destAcc.accountNumber;
        strcpy(tIn.transactionType, "Transfer In");
        tIn.amount = amount;
        tIn.balanceAfter = destAcc.balance;
        recordTransaction(&tIn);

        printf("\nTransfer successful!\n");
        printf("Sender New Balance: $%.2f\n", srcAcc.balance);
    } else {
        printf("Transfer failed due to a file error.\n");
    }
}

void viewTransactions(void) {
    printf("=================================================\n");
    printf("              TRANSACTION HISTORY                \n");
    printf("=================================================\n");
    
    int accNo = getIntInput("Enter Account Number to view history: ");
    
    // Verify account exists before checking transactions
    Account dummy;
    if (!findAccount(accNo, &dummy)) {
        printf("Account not found.\n");
        return;
    }

    FILE *fp = fopen("transactions.dat", "rb");
    if (!fp) {
        printf("No transactions found or unable to open database.\n");
        return;
    }

    printf("History for Account: %d (%s)\n", accNo, dummy.name);
    printf("--------------------------------------------------------------------------------------\n");
    printf("%-12s | %-10s | %-15s | %-15s | %-15s\n", 
           "Date", "Time", "Type", "Amount", "Balance After");
    printf("--------------------------------------------------------------------------------------\n");

    Transaction t;
    int found = 0;
    while (fread(&t, sizeof(Transaction), 1, fp) == 1) {
        if (t.accountNumber == accNo) {
            printf("%-12s | %-10s | %-15s | $%-14.2f | $%-14.2f\n",
                   t.date, t.time, t.transactionType, t.amount, t.balanceAfter);
            found++;
        }
    }
    fclose(fp);

    if (found == 0) {
        printf("No transactions recorded for this account.\n");
    }
    printf("--------------------------------------------------------------------------------------\n");
}
