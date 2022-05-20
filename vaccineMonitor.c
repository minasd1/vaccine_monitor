#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "citizenRecord.h"
#include "hash.h"
#include "BST.h"
#include "bitops.h"
#include "list.h"
#include "error_handling.h"
#include "hashTable.h"
#include "skipList.h"
#include "vMonitor_functions.h"

#define K 16 //NUMBER OF HASH FUNCTIONS


int main(int argc, char *argv[]){
    
    FILE* file = NULL;             //FILE WITH CITIZEN RECORDS
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    size_t bloomSize = 0;           //BLOOM FILTER SIZE
    root = NULL;
    tree = NULL;
    size_t skiplistSize = 0;        //SIZE OF VIRUSES SKIPLIST

    struct citizenRecord record;
    unsigned long BF_position;
    hashTableList* citizen_node;
    int recnum = 0;

    for(int i = 0; i < argc; i++){                  //READ COMMAND LINE ARGUMENTS
        if(strcmp(argv[i], "-c") == 0){
            i++;
            file = fopen(argv[i], "r");             //OPEN FILE TO READ FROM
            if(file == NULL){
                printf("Could not open file!\n");
                exit(1);
            }
        }
        else if(strcmp(argv[i], "-b") == 0){        //GET BLOOM FILTER SIZE
            i++;
            bloomSize = atoi(argv[i]);   
        }
    }

    //GET NUMBER OF RECORDS IN FILE
    while((read = getline(&line, &len, file)) != -1){
        recnum++;
    }
    free(line);
    line = NULL;                          
    
    float fsize = (float)recnum*1.2;
    htable.size = (int)fsize;       
    initialize_hashTable();

    skiplistSize = (log(recnum)/log(2));

    fseek(file, 0, SEEK_SET);     //START READING FROM TEXT FILE
    
    //FILE READ-RECORD MANIPULATION
    while ((read = getline(&line, &len, file)) != -1) {
        //TOKENIZE LINE STRINGS AND ASSIGN THEM TO PROPER RECORD MEMBERS
        record.citizenID = strtok(line, " ");
        record.firstName = strtok(NULL, " ");
        record.lastName = strtok(NULL, " ");
        record.country = strtok(NULL, " ");
        record.age = atoi(strtok(NULL, " "));
        record.virusName = strtok(NULL, " ");
        record.isvaccinated = strtok(NULL, " ");
        record.dateVaccinated = strtok(NULL, "\n");
        
        //ERROR, NOT VACCINATED CITIZEN MUST NOT HAVE A VACCINATION DATE
        if((strcmp(record.isvaccinated, "NO") == 0) && (record.dateVaccinated != NULL)){
            printf("ERROR IN RECORD %s %s %s %s %d %s %s %s\n", record.citizenID ,record.firstName, record.lastName, record.country, record.age, record.virusName, record.isvaccinated, record.dateVaccinated);
        }
        else{
            
            citizen_node = search_hashList(record.citizenID);
            if(citizen_node != NULL){
                printf("citizen id is %s\n", citizen_node->id);
            }
            if ((citizen_node != NULL) && ((strcmp(record.firstName, citizen_node->record->firstName) != 0) || (strcmp(record.lastName, citizen_node->record->lastName) != 0) || (strcmp(record.country, citizen_node->record->country->name) !=0) || (record.age != citizen_node->record->age))){
                   //INCONSISTENCY IN GIVEN CITIZEN DATA
                   printf("Inconsistent record!\n");
            }
            else{   //INSERT PROPER RECORD MEMBERS TO HASHTABLE  
                inserthashTable(record.citizenID, record.firstName, record.lastName, record.country, record.age);
                //INSERT THE VIRUS TO BST OR GET A POINTER TO ITS NODE
                tree = insertBST(&root, record.virusName, bloomSize, skiplistSize);
                if(strcmp(record.isvaccinated, "YES") == 0){
                    if(search_skipList(tree->notVaccinated, record.citizenID) == NULL){
                        //UPDATE BLOOM FILTER
                        for(int i = 0; i < 16; i++){
                            BF_position = hash_i(record.citizenID, i) % bloomSize;
                            setBit(tree->BF, BF_position);
                        }
                        //INSERT IN VACCINATED SKIPLIST
                        insertskipList(&(tree->vaccinated), record.citizenID, record.isvaccinated, record.dateVaccinated, skiplistSize);
                    }
                    else{
                        //CITIZEN ALREADY IN NON-VACCINATED LIST
                        printf("Inconsistent record!\n");
                    }
                }
                else{
                    if(search_skipList(tree->vaccinated, record.citizenID) == NULL){
                        insertskipList(&(tree->notVaccinated), record.citizenID, record.isvaccinated, record.dateVaccinated, skiplistSize);
                    }
                    else{
                        //CITIZEN ALREADY IN VACCINATED LIST
                        printf("Inconsistent record!\n");
                    }
                }
            }

        }
        
        free(line);
        line = NULL;
    }
    
    char* command = NULL;   //USER COMMAND TO INTERACT WITH DATABASE
    char* command_cp;
    char* operation;        
    char* arguments;

    while(1){
        //ASK FOR COMMAND AND READ GIVEN COMMAND
        printf("Please insert command: ");
        read = getline(&command, &len, stdin);
        command_cp = (char*)malloc(strlen(command) + 1);
        strcpy(command_cp, command);
        operation = strtok(command, " ");
        arguments = strtok(NULL, "\n");
        
        //DO THE PROPER OPERATION
        if(strcmp(operation, "/vaccineStatusBloom") == 0){
            vaccineStatusBloom(command_cp, arguments, bloomSize, K);
        }
        else if(strcmp(operation, "/vaccineStatus") == 0){
            vaccineStatus(command_cp, arguments);
        }
        else if(strcmp(operation, "/populationStatus") == 0){
            populationStatus(command_cp, arguments);
        }
        else if(strcmp(operation, "/popStatusByAge") == 0){
            popStatusByAge(command_cp, arguments);
        }
        else if(strcmp(operation, "/insertCitizenRecord") == 0){
            insertCitizenRecord(command_cp, arguments, bloomSize, skiplistSize);
        }
        else if(strcmp(operation, "/vaccinateNow") == 0){
            vaccinateNow(command_cp, arguments, bloomSize, skiplistSize);
        }
        else if(strcmp(operation, "/list-nonVaccinated-Persons") == 0){
            list_nonVaccinated_Persons(command_cp, arguments);
        }
        else if(strcmp(operation, "exit\n") == 0){  
            free(command_cp);
            command_cp = NULL;

            free(command);
            command = NULL;
            
            printf("Exiting...\n");
            break;
        }
        else{

            printf("Not a proper operation! Operation should be one of the following:\n"
            "/vaccineStatusBloom\n"
            "/vaccineStatus\n"
            "/populationStatus\n"
            "/popStatusByAge\n"
            "/insertCitizenRecord\n"
            "/vaccinateNow\n"
            "/list-nonVaccinated-Persons\n"
            "/exit\n\n");

            free(command_cp);
            command_cp = NULL;
        }
        
        
    }
    
    fclose(file);
    if(line)
        free(line);
    
    deallocate(&root);
    destroy_hashTable();
    

    return 0;
}