#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "director.h"

/* ============================================================
   INTERNAL HELPERS
   ============================================================ */

/* Flush leftover input from stdin */
static void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Dynamically loads users from FILE_USERS using malloc/realloc */
static struct user* loadUsersDynamic(int *userCount) {
    FILE *file = fopen(FILE_USERS, "r");
    *userCount = 0;
    
    if (file == NULL) return NULL;

    int capacity = 10;
    struct user *users = (struct user *)malloc(capacity * sizeof(struct user));
    if (users == NULL) {
        fclose(file);
        return NULL;
    }

    char line[200];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        struct user temp;
        
        int parsed = sscanf(line, "%9[^|]|%19[^|]|%19[^|]|%9[^|]|%14[^|]|%49[^\n]",
            temp.userID, temp.username, temp.password, 
            temp.role, temp.phone, temp.email);

        if (parsed == 6) {
            /* Expand array if capacity is reached */
            if (*userCount >= capacity) {
                capacity *= 2;
                users = (struct user *)realloc(users, capacity * sizeof(struct user));
            }
            users[*userCount] = temp;
            (*userCount)++;
        }
    }
    
    fclose(file);
    return users;
}

/* Saves dynamically allocated users array back to FILE_USERS */
static void saveUsersDynamic(struct user *users, int count) {
    FILE *file = fopen(FILE_USERS, "w");
    if (file == NULL) {
        printf("Error: Could not write to %s.\n", FILE_USERS);
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%s|%s|%s|%s|%s\n", 
                users[i].userID, users[i].username, users[i].password, 
                users[i].role, users[i].phone, users[i].email);
    }
    fclose(file);
}

/* Auto-generate an Admin ID based on dynamically loaded users */
static void generateAdminID(struct user users[], int count, char *out) {
    int max = 0;
    for (int i = 0; i < count; i++) {
        if (strncmp(users[i].userID, "A", 1) == 0) {
            int num = atoi(users[i].userID + 1);
            if (num > max) max = num;
        }
    }
    sprintf(out, "A%03d", max + 1);
}

/* Internal struct specific for sales report */
struct saleRecord {
    char year[5];
    char patientID[10];
    double amount;
};

/* ============================================================
   PUBLIC FUNCTIONS
   ============================================================ */

void directorMenu(struct user *loggedInUser) {
    int choice;

    while (1) {
        printf("\n=== Director Dashboard ===\n");
        printf("1. Manage Administrators (Add/Search/View)\n");
        printf("2. Staff Report (Search/View)\n");
        printf("3. Sales Report (Search/View Yearly Sales)\n");
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
                manageAdmins();
                break;
            case 2:
                staffReport();
                break;
            case 3:
                salesReport();
                break;
            case 4:
                printf("Logging out... Goodbye, %s!\n", loggedInUser->username);
                return; /* Returns to main.c, ending the director session */
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void manageAdmins() {
    int choice;
    while (1) {
        printf("\n--- Manage Administrators ---\n");
        printf("1. Add New Administrator\n");
        printf("2. Search and View Administrator\n");
        printf("3. Back to Dashboard\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            continue;
        }
        clearBuffer();

        if (choice == 1) addAdmin();
        else if (choice == 2) searchViewAdmin();
        else if (choice == 3) return;
        else printf("Invalid choice.\n");
    }
}

void addAdmin() {
    int count = 0;
    struct user *users = loadUsersDynamic(&count);

    struct user newAdmin;
    strcpy(newAdmin.role, "admin");

    printf("\n-- Add New Administrator --\n");
    
    /* Auto-generate Admin ID if we have loaded users */
    if (users != NULL) {
        generateAdminID(users, count, newAdmin.userID);
        printf("Generated User ID: %s\n", newAdmin.userID);
    } else {
        printf("Enter User ID: ");
        scanf("%9s", newAdmin.userID);
    }

    printf("Enter Username: ");
    scanf("%19s", newAdmin.username);
    
    /* Validate duplicate username */
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, newAdmin.username) == 0) {
            printf("Error: Username '%s' is already taken.\n", newAdmin.username);
            if (users != NULL) free(users);
            return;
        }
    }

    printf("Enter Password: ");
    scanf("%19s", newAdmin.password);
    printf("Enter Phone: ");
    scanf("%14s", newAdmin.phone);
    clearBuffer();
    printf("Enter Email: ");
    scanf("%49[^\n]", newAdmin.email);
    clearBuffer();

    /* Expand array dynamically to hold new admin */
    if (users == NULL) {
        users = (struct user *)malloc(sizeof(struct user));
    } else {
        users = (struct user *)realloc(users, (count + 1) * sizeof(struct user));
    }
    
    users[count] = newAdmin;
    count++;

    saveUsersDynamic(users, count);
    free(users);

    printf("Success: Administrator account created.\n");
}

void searchViewAdmin() {
    char searchTerm[50];
    printf("\n-- Search Administrator --\n");
    printf("Enter Admin Name or ID to search (or 'ALL' to view all): ");
    scanf("%49s", searchTerm);
    clearBuffer();

    int count = 0;
    struct user *users = loadUsersDynamic(&count);
    if (users == NULL) {
        printf("Error: Could not load user data.\n");
        return;
    }

    int found = 0;
    printf("\n%-10s | %-15s | %-15s | %-25s\n", "ID", "Username", "Phone", "Email");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].role, "admin") == 0) {
            if (strcmp(searchTerm, "ALL") == 0 || 
                strcasecmp(users[i].userID, searchTerm) == 0 || 
                strcasecmp(users[i].username, searchTerm) == 0) {
                
                printf("%-10s | %-15s | %-15s | %-25s\n", 
                       users[i].userID, users[i].username, users[i].phone, users[i].email);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("No administrator found matching '%s'.\n", searchTerm);
    }

    free(users); /* Clean up memory */
}

void staffReport() {
    char searchTerm[50];
    printf("\n--- Staff Report ---\n");
    printf("Enter Staff Name/ID to search (or 'ALL' to view all staff): ");
    scanf("%49s", searchTerm);
    clearBuffer();

    int count = 0;
    struct user *users = loadUsersDynamic(&count);
    if (users == NULL) {
        printf("Error: Could not load user data.\n");
        return;
    }

    int found = 0;
    printf("\n%-10s | %-10s | %-15s | %-15s | %-20s\n", "Role", "ID", "Name", "Phone", "Email");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].role, "admin") == 0 || 
            strcmp(users[i].role, "doctor") == 0 || 
            strcmp(users[i].role, "nurse") == 0) {
            
            if (strcmp(searchTerm, "ALL") == 0 || 
                strcasecmp(users[i].userID, searchTerm) == 0 || 
                strcasecmp(users[i].username, searchTerm) == 0) {
                
                printf("%-10s | %-10s | %-15s | %-15s | %-20s\n", 
                       users[i].role, users[i].userID, users[i].username, users[i].phone, users[i].email);
                found = 1;
            }
        }
    }

    if (!found) {
        printf("No staff found matching '%s'.\n", searchTerm);
    }

    free(users); /* Clean up memory */
}

void salesReport() {
    char searchYear[5];
    printf("\n--- Yearly Sales Report ---\n");
    printf("Enter Year to view sales (e.g., 2024): ");
    scanf("%4s", searchYear);
    clearBuffer();

    FILE *file = fopen("sales.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open sales.txt. No admission billing data found.\n");
        return;
    }

    /* Dynamically allocate an array for sales records */
    int capacity = 10;
    int count = 0;
    struct saleRecord *sales = (struct saleRecord *)malloc(capacity * sizeof(struct saleRecord));

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        struct saleRecord s;
        if (sscanf(line, "%4[^|]|%9[^|]|%lf", s.year, s.patientID, &s.amount) == 3) {
            if (count >= capacity) {
                capacity *= 2;
                sales = (struct saleRecord *)realloc(sales, capacity * sizeof(struct saleRecord));
            }
            sales[count++] = s;
        }
    }
    fclose(file);

    double totalSales = 0.0;
    int found = 0;

    printf("\nSales for Year: %s\n", searchYear);
    printf("%-15s | %-15s\n", "Patient ID", "Amount (RM)");
    printf("----------------------------------\n");

    /* Read from the dynamically loaded array */
    for (int i = 0; i < count; i++) {
        if (strcmp(sales[i].year, searchYear) == 0) {
            printf("%-15s | %.2f\n", sales[i].patientID, sales[i].amount);
            totalSales += sales[i].amount;
            found = 1;
        }
    }

    if (found) {
        printf("----------------------------------\n");
        printf("Total Sales for %s: %.2f\n", searchYear, totalSales);
    } else {
        printf("No sales records found for the year %s.\n", searchYear);
    }

    free(sales); /* Clean up memory */
}