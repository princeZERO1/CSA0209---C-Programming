/*
 * Header file for database and file management operations.
 * Defines prototypes for interacting with binary files securely.
 */

#ifndef FILE_H
#define FILE_H

#include "account.h"
#include "transaction.h"

// Core file operations for Account
int saveAccount(const Account *acc);
int findAccount(int accNo, Account *acc);
int updateAccountInFile(const Account *updatedAcc);
int deleteAccountFromFile(int accNo);
int generateAccountNumber(void);

// Core file operations for Transactions
void recordTransaction(const Transaction *t);

#endif /* FILE_H */
