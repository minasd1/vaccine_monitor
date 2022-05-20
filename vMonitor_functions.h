#ifndef VMONITOR_FUNCTIONS_H
#define VMONITOR_FUNCTIONS_H


#include "list.h"
#include "BST.h"
#include "hashTable.h"
#include "skipList.h"
#include "error_handling.h"
#include "bitops.h"
#include "print_results.h"

/*------------FUNCTIONS THAT EXECUTE OUR APPLICATION'S OPERATIONS---------------*/
void vaccineStatusBloom(char* command, char* args, size_t BF_size, int hash_num);
void vaccineStatus(char* command, char* args);
void populationStatus(char* command, char* args);
void popStatusByAge(char* command, char* args);
void insertCitizenRecord(char* command, char* args, size_t BF_size, size_t sList_size);
void vaccinateNow(char* command, char* args, size_t BF_size, size_t sList_size);
void list_nonVaccinated_Persons(char* command, char* args);





#endif