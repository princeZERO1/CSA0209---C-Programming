/*
 * Implementation of the login system.
 * Validates admin credentials and tracks login attempts.
 */

#include <stdio.h>
#include <string.h>
#include "login.h"
#include "utility.h"

int login(void) {
    const char correctUsername[] = "admin";
    const char correctPassword[] = "admin123";
    
    char enteredUsername[50];
    char enteredPassword[50];
    int attempts = 3;

    while (attempts > 0) {
        clearScreen();
        printf("=================================================\n");
        printf("              ADMINISTRATOR LOGIN                \n");
        printf("=================================================\n");
        
        getStringInput("Username: ", enteredUsername, sizeof(enteredUsername));
        getStringInput("Password: ", enteredPassword, sizeof(enteredPassword));

        if (strcmp(enteredUsername, correctUsername) == 0 && 
            strcmp(enteredPassword, correctPassword) == 0) {
            printf("\nLogin Successful! Welcome, %s.\n", enteredUsername);
            pauseScreen();
            return 1;
        } else {
            attempts--;
            printf("\nInvalid credentials.\n");
            if (attempts > 0) {
                printf("You have %d attempt(s) remaining.\n", attempts);
                pauseScreen();
            }
        }
    }
    return 0;
}
