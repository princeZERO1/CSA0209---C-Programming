/*
 * Header file for common utility functions.
 * Defines prototypes for UI helpers, input validation, and system time.
 */

#ifndef UTILITY_H
#define UTILITY_H

void clearScreen(void);
void pauseScreen(void);
void getStringInput(const char *prompt, char *buffer, int size);
int getIntInput(const char *prompt);
double getDoubleInput(const char *prompt);
void getCurrentDate(char *buffer);
void getCurrentTime(char *buffer);

#endif /* UTILITY_H */
