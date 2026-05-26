//If Login.h is not defined, define it and include the following code
#ifndef LOGIN_H
#define LOGIN_H

#define MAX_USERS 100
#define USERS_FILE "users.txt"

struct user{
    char userID[10];
    char username[20];
    char password[20];
    char role[10]; // "director", "admin", "patient", "doctor", "nurse"
};

int loadUsers(struct user users[], int maxUsers); 

struct user *authenticate(struct user users[], int count, const char *username, const char *password);

#endif