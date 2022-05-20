#ifndef PRINT_RESULTS_H
#define PRINT_RESULTS_H

#include <stdbool.h>
#include "BST.h"
#include "list.h"


/**PRINTS RESULTS OF PROMPT COMMAND FUNCTIONS***/
void print_citizen_status(nodeptr* n, char* id);
void print_status(List* list, bool op);
void print_status_age(List* list, bool op);







#endif