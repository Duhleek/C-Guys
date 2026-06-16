#ifndef ADMIN_H
#define ADMIN_H
 
#include "structs.h"
 
/* Display all staff in the loaded array */
void listStaff(struct staff staffList[], int staffCount);
 
/* Prompt admin to fill in details, auto-generate IDs,
   then append to both users.txt and staff.txt */
void createStaff(struct user users[], int *userCount,
                 struct staff staffList[], int *staffCount);
 
/* Prompt admin to pick a staff by ID, then remove from
   both in-memory arrays and rewrite both .txt files */
void deleteStaff(struct user users[], int *userCount,
                 struct staff staffList[], int *staffCount);
 
#endif /* ADMIN_H */
 