#include "list.h"

//INITIALIZE LIST
List* initialize_list(){
    List* list;
    list = (List*)malloc(sizeof(List));
    list->head = NULL;

    return list;
}

//INITIALIZE A LIST RECORD
listRecord* initialize_listRecord(){        
    listRecord* lRecord = (listRecord*)malloc(sizeof(listRecord));
    lRecord->vacc_between_dates = 0;
    lRecord->vaccinated_pop = 0;
    lRecord->notVaccinated_pop = 0;

    lRecord->vacc_0_20_between_dates = 0;
    lRecord->vaccinated_0_20 = 0;
    lRecord->not_vaccinated_0_20 = 0;

    lRecord->vacc_20_40_between_dates = 0;
    lRecord->vaccinated_20_40 = 0;
    lRecord->not_vaccinated_20_40 = 0;

    lRecord->vacc_40_60_between_dates = 0;
    lRecord->vaccinated_40_60 = 0;
    lRecord->not_vaccinated_40_60 = 0;

    lRecord->vacc_over_60_between_dates = 0;
    lRecord->vaccinated_over_60 = 0;
    lRecord->not_vaccinated_over_60 = 0;

    return lRecord;
}

//INITIALIZE A LIST NODE
listNode* initialize_listNode(char *name){
    //ALLOCATE PROPER MEMORY
    listNode* lNode = (listNode*)malloc(sizeof(listNode));
    lNode->name = (char*)malloc(strlen(name) + 1);
    strcpy(lNode->name, name);
    //INITIALIZE NODE'S RECORD
    lNode->record = initialize_listRecord();

    return lNode;
}

//INSERT A NODE TO LIST
listNode* insert_list(List** list, char* name){  
    //INITIALIZE THE NODE        
    listNode* lNode = initialize_listNode(name);

    //INSERT THE NODE TO THE BEGINNING OF THE LIST
    lNode->next = (*list)->head;
    (*list)->head = lNode;

    return lNode;
}

//SEARCH IF GIVEN KEY EXISTS IN LIST
listNode* search_list(List* list, char* key){
    listNode* current;
    current = list->head;
    //TRAVERSE THE LIST TO FIND IF GIVEN KEY EXISTS
    while(current != NULL){
        if(strcmp(current->name, key) == 0){
            return current;
        }
        else{
            current = current->next;
        }
    }
    return current;
}

//FREE A LIST NODE MEMORY
void destroy_listNode(listNode* lNode){
    free(lNode->record);
    free(lNode->name);
    free(lNode);
}

//FREE MEMORY ALLOCATED FROM LIST
void destroy_list(List* list){
    listNode* current;
    current = list->head;
    listNode* throwaway;
    //TRAVERSE THE LIST AND FREE EVERY NODE'S MEMORY
    while(current != NULL){
        throwaway = current;
        current = current->next;
        destroy_listNode(throwaway);

    }
    
    free(list);
}
