/*
 * Implementation of account operations.
 * Handles creation, displaying, searching, updating, and deleting of accounts.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "account.h"
#include "file.h"
#include "utility.h"

// Helper function to validate phone numbers
static int isValidPhone(const char *phone) {
    if (strlen(phone) == 0) return 0;
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit((unsigned char)phone[i]) && phone[i] != '+' && phone[i] != '-') {
            return 0;
        }
    }
    return 1;
}

void createAccount(void) {
    Account newAccount;
    printf("=================================================\n");
    printf("                CREATE NEW ACCOUNT               \n");
    printf("=================================================\n");

    newAccount.accountNumber = generateAccountNumber();
    printf("Auto-generated Account Number: %d\n", newAccount.accountNumber);

    do {
        getStringInput("Enter Customer Name: ", newAccount.name, sizeof(newAccount.name));
        if (strlen(newAccount.name) == 0) {
            printf("Name cannot be empty. Please try again.\n");
        }
    } while (strlen(newAccount.name) == 0);

    do {
        getStringInput("Enter Phone Number: ", newAccount.phone, sizeof(newAccount.phone));
        if (!isValidPhone(newAccount.phone)) {
            printf("Invalid phone number. Please use digits.\n");
        }
    } while (!isValidPhone(newAccount.phone));

    do {
        getStringInput("Enter Address: ", newAccount.address, sizeof(newAccount.address));
        if (strlen(newAccount.address) == 0) {
            printf("Address cannot be empty.\n");
        }
    } while (strlen(newAccount.address) == 0);

    while (1) {
        getStringInput("Enter Account Type (Savings/Current): ", newAccount.accountType, sizeof(newAccount.accountType));
        if (strcmp(newAccount.accountType, "Savings") == 0 || strcmp(newAccount.accountType, "Current") == 0) {
            break;
        }
        printf("Invalid type! Please enter 'Savings' or 'Current'.\n");
    }

    while (1) {
        newAccount.balance = getDoubleInput("Enter Initial Deposit Amount: $");
        if (newAccount.balance >= 0.0) {
            break;
        }
        printf("Initial deposit cannot be negative.\n");
    }

    getCurrentDate(newAccount.dateCreated);

    if (saveAccount(&newAccount)) {
        printf("\nAccount created successfully!\n");
    } else {
        printf("\nError saving account. Please check file permissions.\n");
    }
}

void displayAccounts(void) {
    printf("====================================================================================================\n");
    printf("%-12s | %-20s | %-15s | %-15s | %-12s | %-15s\n", 
           "Acc No.", "Name", "Phone", "Account Type", "Balance", "Date Created");
    printf("====================================================================================================\n");

    FILE *fp = fopen("accounts.dat", "rb");
    if (!fp) {
        printf("No accounts found in the database.\n");
        return;
    }

    Account acc;
    int count = 0;
    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        printf("%-12d | %-20.20s | %-15.15s | %-15.15s | $%-11.2f | %-15s\n",
               acc.accountNumber, acc.name, acc.phone, acc.accountType, acc.balance, acc.dateCreated);
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf("No accounts to display.\n");
    }
    printf("====================================================================================================\n");
    printf("Total Accounts: %d\n", count);
}

void searchAccount(void) {
    printf("=================================================\n");
    printf("                 SEARCH ACCOUNT                  \n");
    printf("=================================================\n");
    
    int accNo = getIntInput("Enter Account Number to search: ");
    Account acc;

    if (findAccount(accNo, &acc)) {
        printf("\n--- Account Details ---\n");
        printf("Account Number : %d\n", acc.accountNumber);
        printf("Customer Name  : %s\n", acc.name);
        printf("Phone Number   : %s\n", acc.phone);
        printf("Address        : %s\n", acc.address);
        printf("Account Type   : %s\n", acc.accountType);
        printf("Current Balance: $%.2f\n", acc.balance);
        printf("Date Created   : %s\n", acc.dateCreated);
        printf("-----------------------\n");
    } else {
        printf("\nAccount with Number %d not found.\n", accNo);
    }
}

void balanceInquiry(void) {
    printf("=================================================\n");
    printf("                BALANCE INQUIRY                  \n");
    printf("=================================================\n");
    
    int accNo = getIntInput("Enter Account Number: ");
    Account acc;

    if (findAccount(accNo, &acc)) {
        printf("\nAccount Holder : %s\n", acc.name);
        printf("Account Number : %d\n", acc.accountNumber);
        printf("Current Balance: $%.2f\n", acc.balance);
    } else {
        printf("\nAccount with Number %d not found.\n", accNo);
    }
}

void updateAccount(void) {
    printf("=================================================\n");
    printf("                 UPDATE ACCOUNT                  \n");
    printf("=================================================\n");
    
    int accNo = getIntInput("Enter Account Number to update: ");
    Account acc;

    if (!findAccount(accNo, &acc)) {
        printf("\nAccount not found.\n");
        return;
    }

    printf("\nCurrent Name: %s\n", acc.name);
    char newName[100];
    getStringInput("Enter new name (or press Enter to keep current): ", newName, sizeof(newName));
    if (strlen(newName) > 0) {
        strcpy(acc.name, newName);
    }

    printf("\nCurrent Phone: %s\n", acc.phone);
    char newPhone[20];
    getStringInput("Enter new phone (or press Enter to keep current): ", newPhone, sizeof(newPhone));
    if (strlen(newPhone) > 0 && isValidPhone(newPhone)) {
        strcpy(acc.phone, newPhone);
    }

    printf("\nCurrent Address: %s\n", acc.address);
    char newAddress[150];
    getStringInput("Enter new address (or press Enter to keep current): ", newAddress, sizeof(newAddress));
    if (strlen(newAddress) > 0) {
        strcpy(acc.address, newAddress);
    }

    printf("\nCurrent Account Type: %s\n", acc.accountType);
    char newType[20];
    while (1) {
        getStringInput("Enter new type 'Savings'/'Current' (or Enter to keep current): ", newType, sizeof(newType));
        if (strlen(newType) == 0) {
            break; // Keep current
        }
        if (strcmp(newType, "Savings") == 0 || strcmp(newType, "Current") == 0) {
            strcpy(acc.accountType, newType);
            break;
        }
        printf("Invalid type! Please enter 'Savings' or 'Current'.\n");
    }

    if (updateAccountInFile(&acc)) {
        printf("\nAccount updated successfully!\n");
    } else {
        printf("\nFailed to update account.\n");
    }
}

void deleteAccount(void) {
    printf("=================================================\n");
    printf("                 DELETE ACCOUNT                  \n");
    printf("=================================================\n");
    
    int accNo = getIntInput("Enter Account Number to delete: ");
    Account acc;

    if (!findAccount(accNo, &acc)) {
        printf("\nAccount not found.\n");
        return;
    }

    printf("\nAccount found for %s. Balance: $%.2f\n", acc.name, acc.balance);
    if (acc.balance > 0.0) {
        printf("Warning: This account has a positive balance.\n");
    }
    
    char confirm[10];
    getStringInput("Are you sure you want to delete? (yes/no): ", confirm, sizeof(confirm));
    
    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "y") == 0) {
        if (deleteAccountFromFile(accNo)) {
            printf("Account %d deleted successfully.\n", accNo);
        } else {
            printf("Failed to delete account. An error occurred.\n");
        }
    } else {
        printf("Deletion cancelled.\n");
    }
}
