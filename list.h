#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//USE FOR POPSTATUS, POPSTATUSBYAGE OPERATIONS
typedef struct listRecord{
    int vacc_between_dates;
    int vaccinated_pop;
    int notVaccinated_pop;

    int vacc_0_20_between_dates;
    int vaccinated_0_20;
    int not_vaccinated_0_20;

    int vacc_20_40_between_dates;
    int vaccinated_20_40;
    int not_vaccinated_20_40;

    int vacc_40_60_between_dates;
    int vaccinated_40_60;
    int not_vaccinated_40_60;

    int vacc_over_60_between_dates;
    int vaccinated_over_60;
    int not_vaccinated_over_60;

}listRecord;

typedef struct listNode{
    char* name;
    listRecord* record;
    struct listNode* next;

}listNode;

typedef struct List{
    listNode* head;

}List;

/*--------COUNTRY LIST FUNCTIONS----------*/
List* initialize_list();
listNode* initialize_listNode(char *name);
listRecord* initialize_listRecord();                
listNode* insert_list(List** cList, char* name);     
listNode* search_list(List* list, char* key);
void destroy_listNode(listNode* lNode);
void destroy_list(List* list);


#endif