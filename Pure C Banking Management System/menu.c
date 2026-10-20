/*
 * Implementation of the main menu navigation and routing.
 * Routes user choices to specific functional modules.
 */

#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "account.h"
#include "transaction.h"
#include "utility.h"

void showMenu(void) {
    int choice = 0;
    int running = 1;

    while (running) {
        clearScreen();
        printf("=================================================\n");
        printf("              BANK MANAGEMENT SYSTEM             \n");
        printf("=================================================\n");
        printf("1.  Create Account\n");
        printf("2.  Display Accounts\n");
        printf("3.  Search Account\n");
        printf("4.  Deposit Money\n");
        printf("5.  Withdraw Money\n");
        printf("6.  Transfer Money\n");
        printf("7.  Balance Inquiry\n");
        printf("8.  Update Account\n");
        printf("9.  Delete Account\n");
        printf("10. Transaction History\n");
        printf("11. Exit\n");
        printf("=================================================\n");
        
        choice = getIntInput("Enter your choice (1-11): ");

        printf("\n");
        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                displayAccounts();
                break;
            case 3:
                searchAccount();
                break;
            case 4:
                depositMoney();
                break;
            case 5:
                withdrawMoney();
                break;
            case 6:
                transferMoney();
                break;
            case 7:
                balanceInquiry();
                break;
            case 8:
                updateAccount();
                break;
            case 9:
                deleteAccount();
                break;
            case 10:
                viewTransactions();
                break;
            case 11:
                printf("Thank you for using the Bank Management System. Goodbye!\n");
                running = 0;
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 11.\n");
                break;
        }
        
        if (running) {
            pauseScreen();
        }
    }
}
