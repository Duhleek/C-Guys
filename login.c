#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100
#define USERS_FILE "users.txt"

struct user{
    char userID[10];
    char username[20];
    char password[20];
    char role[10]; // "director", "admin", "patient", "doctor", "nurse"
};

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
struct user *authenticate(struct user users[], int count,
                          const char *username, const char *password) {
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

int main(){
    struct user users[MAX_USERS];
    char inputUsername[20];
    char inputPassword[20];

    int userCount = loadUsers(users, MAX_USERS);
    if (userCount < 0) {
        return 1; // Exit if loading users failed
    }
    printf("Loaded %d users from %s.\n", userCount, USERS_FILE);

    printf("Enter username: ");
    scanf("%19s", inputUsername);
    printf("Enter password: ");
    scanf("%19s", inputPassword);

    struct user *loggedIn = authenticate(users, userCount, inputUsername, inputPassword);

    if (loggedIn != NULL) {
        printf("Login successful! Welcome, %s. Your role is: %s\n",
               loggedIn->username, loggedIn->role);
    } else {
        printf("Login failed. Invalid username or password.\n");
    }
    return 0;
}