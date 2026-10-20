/*
 * Implementation of file operations.
 * Manages accounts.dat and transactions.dat binary files safely.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

#define ACCOUNTS_FILE "accounts.dat"
#define TRANSACTIONS_FILE "transactions.dat"
#define TEMP_FILE "temp.dat"

int saveAccount(const Account *acc) {
    FILE *fp = fopen(ACCOUNTS_FILE, "ab");
    if (!fp) return 0;
    
    size_t written = fwrite(acc, sizeof(Account), 1, fp);
    fclose(fp);
    
    return (written == 1);
}

int findAccount(int accNo, Account *acc) {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    if (!fp) return 0;

    int found = 0;
    while (fread(acc, sizeof(Account), 1, fp) == 1) {
        if (acc->accountNumber == accNo) {
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

int updateAccountInFile(const Account *updatedAcc) {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    FILE *temp = fopen(TEMP_FILE, "wb");
    
    if (!fp || !temp) {
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return 0;
    }

    Account acc;
    int success = 0;

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.accountNumber == updatedAcc->accountNumber) {
            // Write the updated account record instead of the old one
            fwrite(updatedAcc, sizeof(Account), 1, temp);
            success = 1;
        } else {
            // Write the existing record
            fwrite(&acc, sizeof(Account), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (success) {
        remove(ACCOUNTS_FILE);
        rename(TEMP_FILE, ACCOUNTS_FILE);
    } else {
        remove(TEMP_FILE);
    }

    return success;
}

int deleteAccountFromFile(int accNo) {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    FILE *temp = fopen(TEMP_FILE, "wb");
    
    if (!fp || !temp) {
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return 0;
    }

    Account acc;
    int found = 0;

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.accountNumber == accNo) {
            found = 1; // Skip writing this to temp file
        } else {
            fwrite(&acc, sizeof(Account), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(ACCOUNTS_FILE);
        rename(TEMP_FILE, ACCOUNTS_FILE);
    } else {
        remove(TEMP_FILE);
    }

    return found;
}

int generateAccountNumber(void) {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    if (!fp) {
        // File doesn't exist yet, start account numbers at 10001
        return 10001;
    }

    Account acc;
    int lastAccNo = 10000; // Base if file is empty

    // Read to the end of the file to get the last account number
    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        lastAccNo = acc.accountNumber;
    }
    
    fclose(fp);
    
    // Check if duplicate happens due to deletion, strictly increment
    return lastAccNo + 1;
}

void recordTransaction(const Transaction *t) {
    FILE *fp = fopen(TRANSACTIONS_FILE, "ab");
    if (fp) {
        fwrite(t, sizeof(Transaction), 1, fp);
        fclose(fp);
    }
}
