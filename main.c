#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "login.c"

int main() {
    struct user users[MAX_USERS];
    int userCount = loadUsers(users, MAX_USERS);
    if (userCount < 0) {
        return 1; // Exit if loading users failed
    }

    char username[20];
    char password[20];

    printf("Enter username: ");
    scanf("%19s", username);
    printf("Enter password: ");
    scanf("%19s", password);

    struct user *loggedInUser = authenticate(users, userCount, username, password);
    if (loggedInUser != NULL) {
        printf("Login successful! Welcome, %s. Your role is: %s\n",
               loggedInUser->username, loggedInUser->role);
        // Here you can add code to redirect to different interfaces based on the role
    } else {
        printf("Login failed! Invalid username or password.\n");
    }

    return 0;
}