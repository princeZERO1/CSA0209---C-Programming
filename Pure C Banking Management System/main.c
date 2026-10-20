/*
 * Banking Management System
 * Entry point of the application.
 * Handles the main initialization, login process, and transitions to the main menu.
 */

#include <stdio.h>
#include <stdlib.h>
#include "login.h"
#include "menu.h"
#include "utility.h"

int main(void) {
    // Clear screen for a fresh start
    clearScreen();
    
    printf("=================================================\n");
    printf("     WELCOME TO THE BANKING MANAGEMENT SYSTEM    \n");
    printf("=================================================\n");
    printf("Press Enter to continue to the login screen...\n");
    getchar();

    // Initiate the login process
    if (login()) {
        // Upon successful login, show the main system menu
        showMenu();
    } else {
        // Exit application if login attempts are exhausted
        printf("\n=================================================\n");
        printf("    SECURITY LOCKOUT: Maximum attempts reached.  \n");
        printf("    Exiting the Banking Management System...     \n");
        printf("=================================================\n");
    }

    return 0;
}
