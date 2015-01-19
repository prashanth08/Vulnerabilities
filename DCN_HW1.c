#include <stdio.h>
#include <string.h>
 
/*
 * forward declarations for functions; see comments preceding
 * these for more details.
 */
int login();
void change_username(int index);
void transfer_money(int index);
void flush_to_disk();
 
/*
 * declare our bank record type.
 */
typedef struct {
        char name[16];
        int  balance;
} bank_entity_t;
 
/*
 * a preinitialized array of bank accounts.
 */
bank_entity_t bank_accounts[5] = {
        { .name="billy_gates", .balance=1000000 },
        { .name="stuart", .balance=10000 },
        { .name="zteo", .balance=1000 },
        { .name="hungry_ugrad", .balance=10 },
        { .name="av445_pb476", .balance=0 }
};
 
 
 
/*
 * executive entrypoint for our glorious Ivy League banking program.
 */
int main(int argc, char** argv) {
        int account_id;
        int choice;
        int done;
        char buf[16];
 
        while(1) {
                printf("Cornell University Primitive Payroll database v1.0.\n");
                printf("Enter 'exit' to quit.\n");
 
                // log in first
                account_id = login();
 
                // perform special processing for invalid user/exit instruction.
                if (account_id == -1) {
                        printf("error: no such user.\n\n");
                        continue;
                } else if (account_id == -2) {
                        flush_to_disk();
                        return 0;
                }
 
                // logged in, greet the user and present a menu
                done = 0;
                while (1) {
                        printf("Welcome ");
                        printf(bank_accounts[account_id].name);
                        printf(", your balance is $%d.\n", bank_accounts[account_id].balance);
 
                        printf("\nWhat would you like to do?\n");
                        printf("1. change your user id.\n");
                        printf("2. transfer funds.\n");
                        printf("3. logout.\n\n");
 
                        if (gets(buf) == NULL || sscanf(buf, "%d", &choice) != 1 ||
                                choice < 1 || choice > 3) continue;
 
                        switch (choice) {
                                case 1:
                                        change_username(account_id);
                                        break;
 
                                case 2:
                                        transfer_money(account_id);
                                        break;
 
                                default:
                                        printf("Your money is safe with us. Goodbye!\n\n");
                                        done = 1;
                        }
 
                        if (done) break;
                }
 
        }
 
        return 0;
}
 
/*
 * login()
 * this function presents a prompt to the user and asks for a username.
 *
 * return value: if username is valid, an integer corresponding to the user's
 *               bank account.
 *               -2, if the username is 'exit'.
 *               -1, otherwise.
 */
int login() {
        int counter;
        int account_id = -1;
        char buf[16];
 
        printf("enter your account name: ");
        fflush(stdout);
        gets(buf);
 
        // check for the special 'exit' username
        if (strcmp(buf, "exit") == 0) {
                return -2;
        }
 
        // look for matching account name
        for (counter = 0; counter < sizeof(bank_accounts)/sizeof(bank_entity_t);
                ++counter) {
 
                if (strncmp(bank_accounts[counter].name, buf, 16) == 0) {
                        account_id = counter;
                        break;
                }
        }
 
        return account_id;
}
 
/*
 * change_username()
 * this function presents a prompt to the user to ask for a new username.
 * checks are made to ensure that the given username is unique.
 *
 * input: index, the index of the bank accounts array for the current user
 */
void change_username(int index) {
        int counter;
        char buf[1024];
 
        printf("enter your new username: ");
        fflush(stdout);
        gets(buf);
 
        // look for duplicate account names
        for (counter = 0; counter < sizeof(bank_accounts) / sizeof(bank_entity_t);
                ++counter) {
                if (strncmp(bank_accounts[counter].name, buf, 16) == 0) {
                        printf("error -- username already exists! no changes made.\n\n");
                        return;
                }
        }
 
        strcpy(bank_accounts[index].name, buf); /*******VULNERABILITY*******/
                //   Buffer Overflow!! /
                /***************************/
        printf("username changed.\n\n");
}
 
/*
 * transfer_money()
 * this function presents two prompts that ask the user for a target account
 * and the amount of money to transfer. checks are made to ensure that the
 * target account exists and that there is sufficient balance in the current
 * account for the transfer.
 *
 * input: index, the index of the bank_accounts array for the current user
 */
void transfer_money(int index) {
 
        int amount;
        int counter;
        char buf[16];
        char buf2[16];
        int found = 0;
 
        printf("enter person to transfer to: ");
        fflush(stdout);
        gets(buf);
 
        printf("enter amount to transfer: ");
        fflush(stdout);
 
        if (gets(buf2) == NULL || sscanf(buf2, "%d", &amount) != 1 ||
                amount < 0 || amount > bank_accounts[index].balance) {
 
                printf("error -- invalid input. no changes made.\n\n");
                return;
        }
 
        // look for matching bank account
        for (counter = 0; counter < sizeof(bank_accounts)/sizeof(bank_entity_t);
                ++counter) {
 
                if (strncmp(bank_accounts[counter].name, buf, 16) == 0) {
                        bank_accounts[counter].balance += amount;
                        bank_accounts[index].balance -= amount;
                        found = 1;
                        break;
                }
        }
 
        if (found == 0) {
                printf("error -- unable to find recipient.\n\n");
        } else {
                printf("funds transferred.\n\n");
        }
}
 
/*
 * flush_to_disk()
 * this function commits all bank account info to the disk, where it should
 * be safe!
 */
void flush_to_disk() {
       
        // notes from junior engineer:
        // i dun haz teh codez for fopen() but i can ezily done dis wif bash
        // scriptingzz!!!
 
        char buf[1024];
        int counter;
 
        printf("writing bank data to disk.\n");
 
        for (counter = 0; counter < sizeof(bank_accounts)/sizeof(bank_entity_t);
                ++counter) {
 
                // my codez very secuer becoz no pssible bufer overflowz! haha u hackerzz!!
                snprintf(buf, sizeof(buf), "echo %d > %s.txt",
                        bank_accounts[counter].balance,
                        bank_accounts[counter].name); /*******VULNERABILITY*******/
                //   Format STring !! /
                /***************************/
 
                /*******VULNERABILITY*******/
                //   System vulnerability!! /
                /***************************/
                system(buf);    // TADA itz so ez y do pple bother use fopen?
        }
}