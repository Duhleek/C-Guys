#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "structs.h"

/* Main entry point for the Director role */
void directorMenu(struct user *loggedInUser);

/* Sub-menus and functionalities */
void manageAdmins();
void addAdmin();
void searchViewAdmin();
void staffReport();
void salesReport();

#endif /* DIRECTOR_H */