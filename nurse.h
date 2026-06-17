#ifndef NURSE_H
#define NURSE_H

#include "structs.h"

/* Main entry point for the Nurse role */
void nurseMenu(struct user *loggedInUser);

/* Sub-menus and functionalities */
void viewDoctorSchedule();

/* Inventory Management */
void manageInventory();
void addInventory();
void searchViewInventory();

/* Daily Notes Management */
void manageDailyNotes(struct user *loggedInUser);
void addDailyNote(struct user *loggedInUser);
void searchViewDailyNotes();

#endif /* NURSE_H */