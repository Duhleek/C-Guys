#include <stdio.h>
#include <string.h>
#include "structs.h"

struct user{
    char userID[10];
    char username[20];
    char password[20];
    char role[10]; // "director", "admin", "patient", "doctor", "nurse"
};

int loadUsers(struct user users[], int maxUsers); 
struct user *authenticate(struct user users[], int count, const char *username, const char *password);