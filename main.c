#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "login.h"

const int attempts = 3;

int main(){
    struct user users[MAX_USERS];

    char inputUsername[20];
    char inputPassword[20];
    int userCount = loadUsers(users, MAX_USERS);

    if (userCount < 0) {return 1;} // Exit if loading users failed

    printf("Loaded %d users from %s.\n", userCount, USERS_FILE);

    for (int i = 0; i < attempts; i++) {
        printf("Attempt %d of %d\n", i + 1, attempts);
        printf("Enter username: ");
        scanf("%19s", inputUsername);
        printf("Enter password: ");
        scanf("%19s", inputPassword);

        struct user *loggedIn = authenticate(users, userCount, inputUsername, inputPassword);

        if (loggedIn != NULL) {
            printf("Login successful! Welcome, %s %s\n",
                   loggedIn->role , loggedIn->username);

            if(strcmp(loggedIn->role, "director") == 0) {
                // directormenu(loggedIn);
            } else if(strcmp(loggedIn->role, "admin") == 0) {
                // adminmenu(loggedIn);
            } else if(strcmp(loggedIn->role, "patient") == 0) {
                // patientmenu(loggedIn);
            } else if(strcmp(loggedIn->role, "doctor") == 0) {
                // doctormenu(loggedIn);
            } else if(strcmp(loggedIn->role, "nurse") == 0) {
                // nursemenu(loggedIn);
            }
            return 0; // Exit after successful login
        } else {
            printf("Login failed. Invalid username or password.\n");
        }
    }
    return 0;
}
