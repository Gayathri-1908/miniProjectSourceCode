/*
    Advanced Bank Management System
    Features Added:
    ----------------
    1. Add Account
    2. Delete Account
    3. Deposit Money
    4. Withdraw Money
    5. Transfer Money
    6. Search Account
    7. Display All Accounts
    8. Create Text File
    9. Update Account
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[20];
    char firstName[20];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);

void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

void displayAccounts(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void transferMoney(FILE *fPtr);
void searchAccount(FILE *fPtr);

int main(void)
{
    FILE *cfPtr;
    unsigned int choice;

    // open file
    cfPtr = fopen("credit.dat", "rb+");

    // create file if not exists
    if (cfPtr == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            puts("File could not be created.");
            exit(EXIT_FAILURE);
        }

        // initialize blank records
        struct clientData blankClient = {0, "", "", 0.0};

        for (int i = 0; i < MAX_ACCOUNTS; i++)
        {
            fwrite(&blankClient,
                   sizeof(struct clientData),
                   1,
                   cfPtr);
        }
    }

    while ((choice = enterChoice()) != 9)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            displayAccounts(cfPtr);
            break;

        case 6:
            depositMoney(cfPtr);
            break;

        case 7:
            withdrawMoney(cfPtr);
            break;

        case 8:
            transferMoney(cfPtr);
            break;

        case 10:
            searchAccount(cfPtr);
            break;

        default:
            puts("Invalid choice.");
        }
    }

    fclose(cfPtr);

    printf("\nProgram terminated.\n");

    return 0;
}

// menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n====================================\n");
    printf("       BANK MANAGEMENT SYSTEM\n");
    printf("====================================\n");

    printf("1  - Create text file\n");
    printf("2  - Update account\n");
    printf("3  - Add new account\n");
    printf("4  - Delete account\n");
    printf("5  - Display all accounts\n");
    printf("6  - Deposit money\n");
    printf("7  - Withdraw money\n");
    printf("8  - Transfer money\n");
    printf("9  - Exit\n");
    printf("10 - Search account\n");

    printf("\nEnter your choice: ");
    scanf("%u", &choice);

    return choice;
}

// create text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;

    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "%-10s%-20s%-20s%-10s\n",
                "Acct",
                "Last Name",
                "First Name",
                "Balance");

        while (fread(&client,
                     sizeof(struct clientData),
                     1,
                     readPtr) == 1)
        {
            if (client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-10u%-20s%-20s%10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.balance);
            }
        }

        fclose(writePtr);

        puts("accounts.txt created successfully.");
    }
}

// add new account
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > MAX_ACCOUNTS)
    {
        puts("Invalid account number.");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        puts("Account already exists.");
    }
    else
    {
        printf("Enter lastname firstname balance:\n");

        scanf("%19s%19s%lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        puts("Account created successfully.");
    }
}

// update account
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number to update: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        puts("Account does not exist.");
    }
    else
    {
        printf("Current Balance: %.2f\n",
               client.balance);

        printf("Enter amount (+/-): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        fseek(fPtr,
              sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        puts("Account updated.");
    }
}

// delete account
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        puts("Account does not exist.");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        puts("Account deleted.");
    }
}

// display all accounts
void displayAccounts(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    rewind(fPtr);

    printf("\n=====================================================\n");

    printf("%-10s%-20s%-20s%-10s\n",
           "Acct",
           "Last Name",
           "First Name",
           "Balance");

    printf("=====================================================\n");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%-10u%-20s%-20s%10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// deposit money
void depositMoney(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        puts("Account does not exist.");
        return;
    }

    printf("Enter deposit amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        puts("Invalid amount.");
        return;
    }

    client.balance += amount;

    fseek(fPtr,
          sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Deposit successful.\n");
    printf("New Balance: %.2f\n",
           client.balance);
}

// withdraw money
void withdrawMoney(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;
    double amount;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        puts("Account does not exist.");
        return;
    }

    printf("Enter withdrawal amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        puts("Invalid amount.");
        return;
    }

    if (amount > client.balance)
    {
        puts("Insufficient balance.");
        return;
    }

    client.balance -= amount;

    fseek(fPtr,
          sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Withdrawal successful.\n");
    printf("Remaining Balance: %.2f\n",
           client.balance);
}

// transfer money
void transferMoney(FILE *fPtr)
{
    struct clientData sender;
    struct clientData receiver;

    unsigned int fromAcc;
    unsigned int toAcc;

    double amount;

    printf("Enter sender account number: ");
    scanf("%u", &fromAcc);

    printf("Enter receiver account number: ");
    scanf("%u", &toAcc);

    printf("Enter transfer amount: ");
    scanf("%lf", &amount);

    // read sender
    fseek(fPtr,
          (fromAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&sender,
          sizeof(struct clientData),
          1,
          fPtr);

    // read receiver
    fseek(fPtr,
          (toAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&receiver,
          sizeof(struct clientData),
          1,
          fPtr);

    if (sender.acctNum == 0 || receiver.acctNum == 0)
    {
        puts("Invalid account.");
        return;
    }

    if (amount > sender.balance)
    {
        puts("Insufficient balance.");
        return;
    }

    sender.balance -= amount;
    receiver.balance += amount;

    // update sender
    fseek(fPtr,
          (fromAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&sender,
           sizeof(struct clientData),
           1,
           fPtr);

    // update receiver
    fseek(fPtr,
          (toAcc - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&receiver,
           sizeof(struct clientData),
           1,
           fPtr);

    puts("Transfer successful.");
}

// search account
void searchAccount(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;

    printf("Enter account number to search: ");
    scanf("%u", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        puts("Account not found.");
    }
    else
    {
        printf("\nAccount Found\n");
        printf("---------------------------\n");

        printf("Account Number : %u\n",
               client.acctNum);

        printf("Last Name      : %s\n",
               client.lastName);

        printf("First Name     : %s\n",
               client.firstName);

        printf("Balance        : %.2f\n",
               client.balance);
    }
}