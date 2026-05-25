#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *filename = "users.txt";

struct user{
    char username[20];
    char password[20];
};

int main(){
    FILE *file = fopen(filename, "rb");
    struct user new_user;

    if (!file) {
        printf("Error opening file\n");
        return 1;
    }
    fclose(file);
    return 0;
}