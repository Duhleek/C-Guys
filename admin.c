#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "admin.h"

/* ============================================================
   INTERNAL HELPERS
   ============================================================ */

/* Scans the users array for IDs matching the given prefix,
   finds the highest number suffix, and returns prefix + (max+1).
   e.g. prefix="DR", existing "DR001","DR002" → out="DR003" */
static void generateID(const char *prefix, struct user users[], int userCount, char *out) {
    int max = 0;
    int prefixLen = strlen(prefix);

    for (int i = 0; i < userCount; i++) {
        if (strncmp(users[i].userID, prefix, prefixLen) == 0) {
            int num = atoi(users[i].userID + prefixLen);
            if (num > max) max = num;
        }
    }
    sprintf(out, "%s%03d", prefix, max + 1);
}

/* Returns the ID prefix for a given role */
static const char *prefixForRole(const char *role) {
    if (strcmp(role, "director") == 0) return "D";
    if (strcmp(role, "admin")    == 0) return "A";
    if (strcmp(role, "doctor")   == 0) return "DR";
    if (strcmp(role, "nurse")    == 0) return "N";
    return "S"; /* fallback */
}

/* Flush leftover input from stdin */
static void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Rewrite users.txt from the in-memory array */
static void saveUsers(struct user users[], int userCount) {
    FILE *f = fopen(FILE_USERS, "w");
    if (f == NULL) { printf("Error: Could not write to %s.\n", FILE_USERS); return; }
    for (int i = 0; i < userCount; i++) {
        fprintf(f, "%s|%s|%s|%s|%s|%s\n",
            users[i].userID,
            users[i].username,
            users[i].password,
            users[i].role,
            users[i].phone,
            users[i].email);
    }
    fclose(f);
}

/* Rewrite staff.txt from the in-memory array */
static void saveStaff(struct staff staffList[], int staffCount) {
    FILE *f = fopen(FILE_STAFF, "w");
    if (f == NULL) { printf("Error: Could not write to %s.\n", FILE_STAFF); return; }
    for (int i = 0; i < staffCount; i++) {
        fprintf(f, "%s|%s|%c|%s|%s|%s|%s\n",
            staffList[i].staffID,
            staffList[i].name,
            staffList[i].gender,
            staffList[i].dob,
            staffList[i].phone,
            staffList[i].email,
            staffList[i].role);
    }
    fclose(f);
}

/* ============================================================
   PUBLIC FUNCTIONS
   ============================================================ */

/* List all staff in a simple table */
void listStaff(struct staff staffList[], int staffCount) {
    if (staffCount == 0) {
        printf("No staff records found.\n");
        return;
    }
    printf("\n%-10s %-25s %-10s\n", "ID", "Name", "Role");
    printf("----------------------------------------------\n");
    for (int i = 0; i < staffCount; i++) {
        printf("%-10s %-25s %-10s\n",
            staffList[i].staffID,
            staffList[i].name,
            staffList[i].role);
    }
    printf("\n");
}

/* Create a new staff member — updates both arrays and both .txt files */
void createStaff(struct user users[], int *userCount,
                 struct staff staffList[], int *staffCount) {

    if (*userCount >= MAX_USERS || *staffCount >= MAX_STAFF) {
        printf("Error: Maximum capacity reached.\n");
        return;
    }

    struct user  newUser  = {0};
    struct staff newStaff = {0};

    /* --- Role --- */
    printf("\nSelect role:\n");
    printf("  1. Director\n");
    printf("  2. Admin\n");
    printf("  3. Doctor\n");
    printf("  4. Nurse\n");
    printf("Choice: ");

    int choice;
    scanf("%d", &choice); clearBuffer();

    switch (choice) {
        case 1: strcpy(newUser.role, "director"); break;
        case 2: strcpy(newUser.role, "admin");    break;
        case 3: strcpy(newUser.role, "doctor");   break;
        case 4: strcpy(newUser.role, "nurse");    break;
        default:
            printf("Invalid choice. Cancelled.\n");
            return;
    }
    strcpy(newStaff.role, newUser.role);

    /* --- Auto-generate ID --- */
    const char *prefix = prefixForRole(newUser.role);
    generateID(prefix, users, *userCount, newUser.userID);
    strcpy(newStaff.staffID, newUser.userID);
    printf("Generated ID: %s\n", newUser.userID);

    /* --- Username --- */
    printf("Username: ");
    scanf("%19s", newUser.username); clearBuffer();

    /* Check username is not already taken */
    for (int i = 0; i < *userCount; i++) {
        if (strcmp(users[i].username, newUser.username) == 0) {
            printf("Error: Username '%s' is already taken.\n", newUser.username);
            return;
        }
    }

    /* --- Password --- */
    printf("Password: ");
    scanf("%19s", newUser.password); clearBuffer();

    /* --- Full name --- */
    printf("Full name: ");
    fgets(newStaff.name, sizeof(newStaff.name), stdin);
    newStaff.name[strcspn(newStaff.name, "\n")] = '\0';

    /* --- Gender --- */
    printf("Gender (M/F): ");
    char g = getchar(); clearBuffer();
    newStaff.gender = (g == 'm') ? 'M' : (g == 'f') ? 'F' : g;

    /* --- Date of birth --- */
    printf("Date of birth (DD/MM/YYYY): ");
    scanf("%11s", newStaff.dob); clearBuffer();

    /* --- Phone --- */
    printf("Phone: ");
    scanf("%14s", newUser.phone); clearBuffer();
    strcpy(newStaff.phone, newUser.phone);

    /* --- Email --- */
    printf("Email: ");
    scanf("%49s", newUser.email); clearBuffer();
    strcpy(newStaff.email, newUser.email);

    /* --- Confirm --- */
    printf("\nConfirm creating user '%s' with ID '%s'? (y/n): ", newUser.username, newUser.userID);
    char confirm = getchar(); clearBuffer();
    if (confirm != 'y' && confirm != 'Y') {
        printf("Cancelled.\n");
        return;
    }

    /* --- Add to in-memory arrays --- */
    users[*userCount]       = newUser;
    staffList[*staffCount]  = newStaff;
    (*userCount)++;
    (*staffCount)++;

    /* --- Persist to files --- */
    saveUsers(users, *userCount);
    saveStaff(staffList, *staffCount);

    printf("Staff member '%s' (%s) created successfully.\n", newStaff.name, newStaff.staffID);
}

/* Delete a staff member by staffID — updates both arrays and both .txt files */
void deleteStaff(struct user users[], int *userCount,
                 struct staff staffList[], int *staffCount) {

    listStaff(staffList, *staffCount);
    if (*staffCount == 0) return;

    char targetID[10];
    printf("Enter staff ID to delete: ");
    scanf("%9s", targetID); clearBuffer();

    /* Find in staff array */
    int staffIdx = -1;
    for (int i = 0; i < *staffCount; i++) {
        if (strcmp(staffList[i].staffID, targetID) == 0) {
            staffIdx = i;
            break;
        }
    }
    if (staffIdx == -1) {
        printf("Error: Staff ID '%s' not found.\n", targetID);
        return;
    }

    /* Find matching user by userID */
    int userIdx = -1;
    for (int i = 0; i < *userCount; i++) {
        if (strcmp(users[i].userID, targetID) == 0) {
            userIdx = i;
            break;
        }
    }

    /* Confirm */
    printf("Are you sure you want to delete '%s' (%s)? (y/n): ",
           staffList[staffIdx].name, targetID);
    char confirm = getchar(); clearBuffer();
    if (confirm != 'y' && confirm != 'Y') {
        printf("Cancelled.\n");
        return;
    }

    /* Remove from staff array by shifting left */
    for (int i = staffIdx; i < *staffCount - 1; i++) {
        staffList[i] = staffList[i + 1];
    }
    (*staffCount)--;

    /* Remove from users array by shifting left */
    if (userIdx != -1) {
        for (int i = userIdx; i < *userCount - 1; i++) {
            users[i] = users[i + 1];
        }
        (*userCount)--;
    }

    /* Persist to files */
    saveUsers(users, *userCount);
    saveStaff(staffList, *staffCount);

    printf("Staff member '%s' deleted successfully.\n", targetID);

}

void adminMenu(struct user *loggedInUser) {
    int choice;
    
    /* Arrays to hold data for the admin functions */
    struct user users[MAX_USERS];
    struct staff staffList[MAX_STAFF];
    int userCount = 0;
    int staffCount = 0;

    /* TODO: You might want to load the existing users and staff from 
       your .txt files into the arrays above right here! */

    while (1) {
        printf("\n=== Admin Dashboard ===\n");
        printf("1. List Staff\n");
        printf("2. Create Staff\n");
        printf("3. Delete Staff\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        clearBuffer();

        switch (choice) {
            case 1:
                listStaff(staffList, staffCount);
                break;
            case 2:
                createStaff(users, &userCount, staffList, &staffCount);
                break;
            case 3:
                deleteStaff(users, &userCount, staffList, &staffCount);
                break;
            case 4:
                printf("Logging out... Goodbye, %s!\n", loggedInUser->username);
                return; /* Returns to main.c, ending the admin session */
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}