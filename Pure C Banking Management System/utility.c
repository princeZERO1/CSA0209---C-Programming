/*
 * Implementation of utility functions.
 * Handles robust user input safely (preventing buffer overflow and infinite loops).
 * Also handles cross-platform screen clearing and timestamp generation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utility.h"

// Clear the screen appropriately for the operating system
void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Pause screen waiting for user action
void pauseScreen(void) {
    printf("\nPress Enter to continue...");
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Safely get string input preventing buffer overflows and clearing stdin trailing
void getStringInput(const char *prompt, char *buffer, int size) {
    if (prompt != NULL) {
        printf("%s", prompt);
    }
    
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            // Clear input buffer if truncated
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    } else {
        buffer[0] = '\0';
    }
}

// Safely get an integer, rejecting purely alphabetical inputs
int getIntInput(const char *prompt) {
    char buffer[50];
    int value = 0;
    while (1) {
        getStringInput(prompt, buffer, sizeof(buffer));
        // sscanf parses the string; if it successfully parses an int, break.
        if (sscanf(buffer, "%d", &value) == 1) {
            break;
        } else {
            printf("Invalid input. Please enter a valid integer.\n");
        }
    }
    return value;
}

// Safely get a double, rejecting purely alphabetical inputs
double getDoubleInput(const char *prompt) {
    char buffer[50];
    double value = 0.0;
    while (1) {
        getStringInput(prompt, buffer, sizeof(buffer));
        if (sscanf(buffer, "%lf", &value) == 1) {
            break;
        } else {
            printf("Invalid input. Please enter a valid numerical amount.\n");
        }
    }
    return value;
}

// Get the current system date as DD-MM-YYYY
void getCurrentDate(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(buffer, "%02d-%02d-%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

// Get the current system time as HH:MM:SS
void getCurrentTime(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(buffer, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
}
