#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "login.h"
#include "director.h"
#include "admin.h"
#include "nurse.h"

#define MAX_ATTEMPTS 3

int main() {
    struct user users[MAX_USERS];
    char inputUsername[20];
    char inputPassword[20];

    /* Load all users from file */
    int userCount = loadUsers(users, MAX_USERS);
    if (userCount < 0) {
        printf("Failed to load users. Exiting.\n");
        return 1;
    }

    printf("=== Hospital Management System ===\n\n");

    /* Login loop — up to MAX_ATTEMPTS tries */
    for (int i = 0; i < MAX_ATTEMPTS; i++) {
        printf("Login attempt %d of %d\n", i + 1, MAX_ATTEMPTS);
        printf("Username: ");
        scanf("%19s", inputUsername);
        printf("Password: ");
        scanf("%19s", inputPassword);

        struct user *loggedIn = authenticate(users, userCount, inputUsername, inputPassword);

        if (loggedIn != NULL) {
            printf("\nLogin successful! Welcome, %s (%s)\n\n", loggedIn->username, loggedIn->role);

            if (strcmp(loggedIn->role, "director") == 0) {
                directorMenu(loggedIn);
            } else if (strcmp(loggedIn->role, "admin") == 0) {
                adminMenu(loggedIn);
            } else if (strcmp(loggedIn->role, "doctor") == 0) {
                // doctorMenu(loggedIn);
            } else if (strcmp(loggedIn->role, "nurse") == 0) {
                 nurseMenu(loggedIn);
            } else if (strcmp(loggedIn->role, "patient") == 0) {
                // patientMenu(loggedIn);
            } else {
                printf("Error: Unknown role '%s'.\n", loggedIn->role);
            }

            return 0;
        }

        printf("Invalid username or password.");
        if (i < MAX_ATTEMPTS - 1) {
            printf(" Please try again.\n\n");
        } else {
            printf("\nAccount locked. Too many failed attempts.\n");
        }
    }

    return 1;
}