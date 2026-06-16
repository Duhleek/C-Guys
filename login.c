#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "login.h"

/* Load users from FILE_USERS into the users array.
   Returns number of users loaded, or -1 on failure. */
int loadUsers(struct user users[], int maxUsers) {
    FILE *file = fopen(FILE_USERS, "r");
    if (file == NULL) {
        printf("Error: Could not open %s.\n", FILE_USERS);
        return -1;
    }

    int count = 0;
    char line[200];

    while (fgets(line, sizeof(line), file) != NULL && count < maxUsers) {

        /* Strip trailing newline */
        line[strcspn(line, "\n")] = '\0';

        /* Parse pipe-delimited fields:
           userID|username|password|role|phone|email */
        int parsed = sscanf(line, "%9[^|]|%19[^|]|%19[^|]|%9[^|]|%14[^|]|%49[^\n]",
            users[count].userID,
            users[count].username,
            users[count].password,
            users[count].role,
            users[count].phone,
            users[count].email);

        if (parsed == 6) {
            count++;
        }
    }

    fclose(file);
    return count;
}

/* Searches the loaded users array for a matching username + password.
   Returns a pointer to the matching user, or NULL if not found.
   Note: username and password are case-sensitive. */
struct user *authenticate(struct user users[], int count, const char *username, const char *password) {
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            return &users[i];
        }
    }
    return NULL;
}