#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "login.h"

// Load users from the file into the users array, return the number of users loaded, -1 if it fails
int loadUsers(struct user users[], int maxUsers) {
    FILE *file = fopen(USERS_FILE, "r");
    if(file == NULL){
        printf("Error opening user file.\n");
        return -1;
    }
    int count = 0;
    char line[100];

    while (fgets(line, sizeof(line), file) != NULL && count < MAX_USERS) {

        /* Strip trailing newline */
        line[strcspn(line, "\n")] = '\0';
 
        /* Parse pipe-delimited fields */
        int parsed = sscanf(line, "%9[^|]|%19[^|]|%19[^|]|%9[^\n]",
            users[count].userID,
            users[count].username,
            users[count].password,
            users[count].role);
 
        if (parsed == 4) {
            count++;
        }
    }
 
    fclose(file);
    return count;
}

/* Searches the loaded users array for a matching username + password.
   Returns a pointer to the matching User, or NULL if not found.
   Assumption: username and password are case-sensitive. */
struct user *authenticate(struct user users[], int count, const char *username, const char *password) {
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            return &users[i];
        }
    }
    return NULL;
}
