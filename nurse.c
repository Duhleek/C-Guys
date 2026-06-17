#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nurse.h"

/* ============================================================
   INTERNAL HELPERS
   ============================================================ */

/* Flush leftover input from stdin */
static void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Dynamically load schedules from FILE_SCHEDULES */
static struct schedule* loadSchedulesDynamic(int *count) {
    FILE *file = fopen(FILE_SCHEDULES, "r");
    *count = 0;
    if (file == NULL) return NULL;

    int capacity = 10;
    struct schedule *schedules = (struct schedule *)malloc(capacity * sizeof(struct schedule));
    if (schedules == NULL) { fclose(file); return NULL; }

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        struct schedule temp;
        
        if (sscanf(line, "%9[^|]|%9[^|]|%11[^|]|%5[^|]|%14[^\n]",
            temp.scheduleID, temp.doctorID, temp.date, temp.time, temp.status) == 5) {
            
            if (*count >= capacity) {
                capacity *= 2;
                schedules = (struct schedule *)realloc(schedules, capacity * sizeof(struct schedule));
            }
            schedules[*count] = temp;
            (*count)++;
        }
    }
    fclose(file);
    return schedules;
}

/* Dynamically load inventory from FILE_INVENTORY */
static struct inventory* loadInventoryDynamic(int *count) {
    FILE *file = fopen(FILE_INVENTORY, "r");
    *count = 0;
    if (file == NULL) return NULL;

    int capacity = 10;
    struct inventory *items = (struct inventory *)malloc(capacity * sizeof(struct inventory));
    if (items == NULL) { fclose(file); return NULL; }

    char line[200];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        struct inventory temp;
        
        if (sscanf(line, "%9[^|]|%49[^|]|%19[^|]|%d|%14[^|]|%11[^\n]",
            temp.itemID, temp.itemName, temp.category, &temp.quantity, temp.unit, temp.expiryDate) == 6) {
            
            if (*count >= capacity) {
                capacity *= 2;
                items = (struct inventory *)realloc(items, capacity * sizeof(struct inventory));
            }
            items[*count] = temp;
            (*count)++;
        }
    }
    fclose(file);
    return items;
}

static void saveInventoryDynamic(struct inventory *items, int count) {
    FILE *file = fopen(FILE_INVENTORY, "w");
    if (file == NULL) {
        printf("Error: Could not write to %s.\n", FILE_INVENTORY);
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%s|%s|%d|%s|%s\n", 
            items[i].itemID, items[i].itemName, items[i].category, 
            items[i].quantity, items[i].unit, items[i].expiryDate);
    }
    fclose(file);
}

/* Auto-generate Inventory ID (e.g., INV001) */
static void generateInventoryID(struct inventory items[], int count, char *out) {
    int max = 0;
    for (int i = 0; i < count; i++) {
        if (strncmp(items[i].itemID, "INV", 3) == 0) {
            int num = atoi(items[i].itemID + 3);
            if (num > max) max = num;
        }
    }
    sprintf(out, "INV%03d", max + 1);
}

/* Dynamically load nurse notes from FILE_NOTES */
static struct nurseNote* loadNotesDynamic(int *count) {
    FILE *file = fopen(FILE_NOTES, "r");
    *count = 0;
    if (file == NULL) return NULL;

    int capacity = 10;
    struct nurseNote *notes = (struct nurseNote *)malloc(capacity * sizeof(struct nurseNote));
    if (notes == NULL) { fclose(file); return NULL; }

    char line[500];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        struct nurseNote temp;
        
        if (sscanf(line, "%9[^|]|%9[^|]|%9[^|]|%11[^|]|%5[^|]|%f|%9[^|]|%199[^|]|%199[^\n]",
            temp.noteID, temp.patientID, temp.nurseID, temp.date, temp.time, 
            &temp.temperature, temp.bloodPressure, temp.symptoms, temp.note) == 9) {
            
            if (*count >= capacity) {
                capacity *= 2;
                notes = (struct nurseNote *)realloc(notes, capacity * sizeof(struct nurseNote));
            }
            notes[*count] = temp;
            (*count)++;
        }
    }
    fclose(file);
    return notes;
}

static void saveNotesDynamic(struct nurseNote *notes, int count) {
    FILE *file = fopen(FILE_NOTES, "w");
    if (file == NULL) {
        printf("Error: Could not write to %s.\n", FILE_NOTES);
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%s|%s|%s|%s|%.1f|%s|%s|%s\n", 
            notes[i].noteID, notes[i].patientID, notes[i].nurseID, notes[i].date, 
            notes[i].time, notes[i].temperature, notes[i].bloodPressure, 
            notes[i].symptoms, notes[i].note);
    }
    fclose(file);
}

/* Auto-generate Note ID (e.g., NN001) */
static void generateNoteID(struct nurseNote notes[], int count, char *out) {
    int max = 0;
    for (int i = 0; i < count; i++) {
        if (strncmp(notes[i].noteID, "NN", 2) == 0) {
            int num = atoi(notes[i].noteID + 2);
            if (num > max) max = num;
        }
    }
    sprintf(out, "NN%03d", max + 1);
}

/* ============================================================
   PUBLIC FUNCTIONS
   ============================================================ */

void nurseMenu(struct user *loggedInUser) {
    int choice;

    while (1) {
        printf("\n=== Nurse Dashboard ===\n");
        printf("1. Doctor Scheduler (View Only)\n");
        printf("2. Medical & Supplies Inventory (Add/Search/View)\n");
        printf("3. Daily Notes (Add/Search/View)\n");
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
                viewDoctorSchedule();
                break;
            case 2:
                manageInventory();
                break;
            case 3:
                manageDailyNotes(loggedInUser);
                break;
            case 4:
                printf("Logging out... Goodbye, %s!\n", loggedInUser->username);
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

/* --- Scheduler (View Only) --- */
void viewDoctorSchedule() {
    printf("\n--- Doctor Schedules ---\n");
    int count = 0;
    struct schedule *schedules = loadSchedulesDynamic(&count);
    
    if (schedules == NULL || count == 0) {
        printf("No schedules found.\n");
        if (schedules) free(schedules);
        return;
    }

    printf("%-12s | %-10s | %-12s | %-6s | %-10s\n", "Schedule ID", "Doctor ID", "Date", "Time", "Status");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-12s | %-10s | %-12s | %-6s | %-10s\n", 
               schedules[i].scheduleID, schedules[i].doctorID, 
               schedules[i].date, schedules[i].time, schedules[i].status);
    }
    
    free(schedules);
}

/* --- Inventory Management --- */
void manageInventory() {
    int choice;
    while (1) {
        printf("\n--- Medical & Supplies Inventory ---\n");
        printf("1. Add New Stock Entry\n");
        printf("2. Search and View Inventory\n");
        printf("3. Back to Dashboard\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            continue;
        }
        clearBuffer();

        if (choice == 1) addInventory();
        else if (choice == 2) searchViewInventory();
        else if (choice == 3) return;
        else printf("Invalid choice.\n");
    }
}

void addInventory() {
    int count = 0;
    struct inventory *items = loadInventoryDynamic(&count);

    struct inventory newItem;

    printf("\n-- Add New Inventory Item --\n");
    
    if (items != NULL) {
        generateInventoryID(items, count, newItem.itemID);
        printf("Generated Item ID: %s\n", newItem.itemID);
    } else {
        printf("Enter Item ID (e.g., INV001): ");
        scanf("%9s", newItem.itemID);
        clearBuffer();
    }

    printf("Enter Item Name: ");
    scanf(" %49[^\n]", newItem.itemName);
    clearBuffer();

    printf("Enter Category (medication/supply): ");
    scanf("%19s", newItem.category);
    clearBuffer();

    printf("Enter Quantity: ");
    scanf("%d", &newItem.quantity);
    clearBuffer();

    printf("Enter Unit (e.g., tablets, boxes): ");
    scanf("%14s", newItem.unit);
    clearBuffer();

    printf("Enter Expiry Date (MM/YYYY) or N/A for supplies: ");
    scanf("%11s", newItem.expiryDate);
    clearBuffer();

    if (items == NULL) {
        items = (struct inventory *)malloc(sizeof(struct inventory));
    } else {
        items = (struct inventory *)realloc(items, (count + 1) * sizeof(struct inventory));
    }
    
    items[count] = newItem;
    count++;

    saveInventoryDynamic(items, count);
    free(items);

    printf("Success: Inventory item added.\n");
}

void searchViewInventory() {
    char searchTerm[50];
    printf("\n-- Search Inventory --\n");
    printf("Enter Item Name or ID to search (or 'ALL' to view all): ");
    scanf(" %49[^\n]", searchTerm);
    clearBuffer();

    int count = 0;
    struct inventory *items = loadInventoryDynamic(&count);
    if (items == NULL) {
        printf("Error: Could not load inventory data.\n");
        return;
    }

    int found = 0;
    printf("\n%-8s | %-20s | %-12s | %-6s | %-10s | %-10s\n", "ID", "Name", "Category", "Qty", "Unit", "Expiry");
    printf("------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (strcmp(searchTerm, "ALL") == 0 || 
            strcasecmp(items[i].itemID, searchTerm) == 0 || 
            strcasecmp(items[i].itemName, searchTerm) == 0) {
            
            printf("%-8s | %-20s | %-12s | %-6d | %-10s | %-10s\n", 
                   items[i].itemID, items[i].itemName, items[i].category, 
                   items[i].quantity, items[i].unit, items[i].expiryDate);
            found = 1;
        }
    }

    if (!found) {
        printf("No inventory found matching '%s'.\n", searchTerm);
    }
    free(items);
}

/* --- Daily Notes Management --- */
void manageDailyNotes(struct user *loggedInUser) {
    int choice;
    while (1) {
        printf("\n--- Daily Patient Notes ---\n");
        printf("1. Add New Daily Note\n");
        printf("2. Search and View Notes\n");
        printf("3. Back to Dashboard\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            continue;
        }
        clearBuffer();

        if (choice == 1) addDailyNote(loggedInUser);
        else if (choice == 2) searchViewDailyNotes();
        else if (choice == 3) return;
        else printf("Invalid choice.\n");
    }
}

void addDailyNote(struct user *loggedInUser) {
    int count = 0;
    struct nurseNote *notes = loadNotesDynamic(&count);

    struct nurseNote newNote;
    
    /* Assign the current logged-in nurse's ID */
    strcpy(newNote.nurseID, loggedInUser->userID);

    printf("\n-- Add Daily Patient Note --\n");
    
    if (notes != NULL) {
        generateNoteID(notes, count, newNote.noteID);
        printf("Generated Note ID: %s\n", newNote.noteID);
    } else {
        printf("Enter Note ID (e.g., NN001): ");
        scanf("%9s", newNote.noteID);
        clearBuffer();
    }

    printf("Enter Patient ID: ");
    scanf("%9s", newNote.patientID);
    clearBuffer();

    printf("Enter Date (DD/MM/YYYY): ");
    scanf("%11s", newNote.date);
    clearBuffer();

    printf("Enter Time (HH:MM): ");
    scanf("%5s", newNote.time);
    clearBuffer();

    printf("Enter Temperature (Celsius): ");
    scanf("%f", &newNote.temperature);
    clearBuffer();

    printf("Enter Blood Pressure (e.g., 120/80): ");
    scanf("%9s", newNote.bloodPressure);
    clearBuffer();

    printf("Enter Symptoms: ");
    scanf(" %199[^\n]", newNote.symptoms);
    clearBuffer();

    printf("Enter Note (Condition, reminders, etc.): ");
    scanf(" %199[^\n]", newNote.note);
    clearBuffer();

    if (notes == NULL) {
        notes = (struct nurseNote *)malloc(sizeof(struct nurseNote));
    } else {
        notes = (struct nurseNote *)realloc(notes, (count + 1) * sizeof(struct nurseNote));
    }
    
    notes[count] = newNote;
    count++;

    saveNotesDynamic(notes, count);
    free(notes);

    printf("Success: Daily note recorded for Patient %s.\n", newNote.patientID);
}

void searchViewDailyNotes() {
    char searchTerm[50];
    printf("\n-- Search Daily Notes --\n");
    printf("Enter Patient ID or Note ID to search (or 'ALL' to view all): ");
    scanf(" %49[^\n]", searchTerm);
    clearBuffer();

    int count = 0;
    struct nurseNote *notes = loadNotesDynamic(&count);
    if (notes == NULL) {
        printf("Error: Could not load daily notes data.\n");
        return;
    }

    int found = 0;
    printf("\n%-8s | %-10s | %-12s | %-6s | %-6s | %-8s | %-20s\n", 
           "Note ID", "Patient ID", "Date", "Time", "Temp", "BP", "Symptoms");
    printf("-------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (strcmp(searchTerm, "ALL") == 0 || 
            strcasecmp(notes[i].noteID, searchTerm) == 0 || 
            strcasecmp(notes[i].patientID, searchTerm) == 0) {
            
            printf("%-8s | %-10s | %-12s | %-6s | %-6.1f | %-8s | %-20s\n", 
                   notes[i].noteID, notes[i].patientID, notes[i].date, notes[i].time, 
                   notes[i].temperature, notes[i].bloodPressure, notes[i].symptoms);
            
            printf("  -> Detail Note: %s\n", notes[i].note);
            found = 1;
        }
    }

    if (!found) {
        printf("No notes found matching '%s'.\n", searchTerm);
    }
    free(notes);
}