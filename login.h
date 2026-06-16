#ifndef LOGIN_H
#define LOGIN_H
#include "structs.h"


int loadUsers(struct user users[], int maxUsers); 
struct user *authenticate(struct user users[], int count, const char *username, const char *password);

#endif /* LOGIN_H */