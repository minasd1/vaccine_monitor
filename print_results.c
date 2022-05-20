#include "print_results.h"

//WE USE THIS FUNCTION IN /vaccineStatus citizenID operation
void print_citizen_status(nodeptr* n, char* id){
    if(*n == NULL){
        return;
    }
    print_citizen_status(&((*n)->left), id);
    print_citizen_status(&((*n)->right), id);
    //SEARCH FOR THE GIVEN CITIZEN ID IN EVERY VIRUS'S VACCINATED AND NOT VACCINATED SKIPLIST
    skipListNode* node = search_skipList((*n)->vaccinated, id);
    if(node != NULL){       //IF FOUND IN VACCINATED LIST, PRINT THE PROPER DATA
        printf("%s %s %s\n", (*n)->virusName, node->record->isvaccinated, node->record->dateVaccinated);
    }
    node = search_skipList((*n)->notVaccinated, id);
    if(node != NULL){       //SAME IF FOUND IN VIRUS'S NOT VACCINATED SKIPLIST
        printf("%s %s\n", (*n)->virusName, node->record->isvaccinated);
    }
}



//WE USE THIS FUNCTION TO PRINT THE RESULTS OF POPULATIONSTATUS OPERATION
void print_status(List* list, bool op){
    listNode* current = list->head;
    float vacc_percentage;
    int vacpop, notvacpop;
    //TRAVERSE THE COUNTRY LIST
    while(current != NULL){
        vacpop = current->record->vaccinated_pop;
        notvacpop = current->record->notVaccinated_pop;
        if(op == 0){ //NO DATE ARGUMENTS
            //CALCULATE PERCENTAGE OF VACCINATED POPULATION
            vacc_percentage = (float)vacpop/(vacpop + notvacpop) * 100;
            if(vacc_percentage != vacc_percentage){vacc_percentage = 0;}    //IF NAN ASSIGN TO 0
            printf("%s %d %.2f%%\n", current->name, current->record->vaccinated_pop, vacc_percentage);
        }
        else{       //DATE ARGUMENTS
            int vac_in_dates = current->record->vacc_between_dates;
            vacc_percentage = (float)vac_in_dates/(vacpop + notvacpop) * 100;
            if(vacc_percentage != vacc_percentage){vacc_percentage = 0;}
            printf("%s %d %.2f%%\n\n", current->name, current->record->vacc_between_dates, vacc_percentage);
        }

        current = current->next;
    }
}

//WE USE THIS FUNCTION TO PRINT THE RESULTS OF POPSTATUSBYAGE OPERATION
void print_status_age(List* list, bool op){
    listNode* current = list->head;
    float vacc_percent_0_20, vacc_percent_20_40, vacc_percent_40_60, vacc_percent_over_60;

    int vac_0_20, vac_20_40, vac_40_60, vac_over_60;
    int not_vac_0_20, not_vac_20_40, not_vac_40_60, not_vac_over_60;
    //TRAVERSE THE COUNTRY LIST
    while(current != NULL){
        vac_0_20 = current->record->vaccinated_0_20;
        not_vac_0_20 = current->record->not_vaccinated_0_20;

        vac_20_40 = current->record->vaccinated_20_40;
        not_vac_20_40 = current->record->not_vaccinated_20_40;

        vac_40_60 = current->record->vaccinated_40_60;
        not_vac_40_60 = current->record->not_vaccinated_40_60;

        vac_over_60 = current->record->vaccinated_over_60;
        not_vac_over_60 = current->record->not_vaccinated_over_60;

        if(op == 0){ //NO DATE ARGUMENTS
            //CALCULATE VACCINATED PERCENTAGE IN EVERY AGE GROUP
            vacc_percent_0_20 = (float)vac_0_20/(vac_0_20 + not_vac_0_20) * 100;
            if(vacc_percent_0_20 != vacc_percent_0_20){vacc_percent_0_20 = 0;}//IF NAN ASSIGN TO 0

            vacc_percent_20_40 = (float)vac_20_40/(vac_20_40 + not_vac_20_40) * 100;
            if(vacc_percent_20_40 != vacc_percent_20_40){vacc_percent_20_40 = 0;}

            vacc_percent_40_60 = (float)vac_40_60/(vac_40_60 + not_vac_40_60) * 100;
            if(vacc_percent_40_60 != vacc_percent_40_60){vacc_percent_40_60 = 0;}

            vacc_percent_over_60 = (float)vac_over_60/(vac_over_60 + not_vac_over_60) * 100;
            if(vacc_percent_over_60 != vacc_percent_over_60){vacc_percent_over_60 = 0;}
            //PRINT THE RESULTS
            printf("%s\n0-20 %d %.2f%%\n", current->name, current->record->vaccinated_0_20, vacc_percent_0_20);
            printf("20-40 %d %.2f%%\n", current->record->vaccinated_20_40, vacc_percent_20_40);
            printf("40-60 %d %.2f%%\n", current->record->vaccinated_40_60, vacc_percent_40_60);
            printf("60+ %d %.2f%%\n\n", current->record->vaccinated_over_60, vacc_percent_over_60);
        }
        else{       //DATE ARGUMENTS
            int vac_0_20_in_dates, vac_20_40_in_dates, vac_40_60_in_dates, vac_over_60_in_dates;
            //CALCULATE THE VACCINATED BETWEEN DATES POPULATION FOR EVERY AGE GROUP
            vac_0_20_in_dates = current->record->vacc_0_20_between_dates;
            vacc_percent_0_20 = (float)vac_0_20_in_dates/(vac_0_20 + not_vac_0_20) * 100;
            if(vacc_percent_0_20 != vacc_percent_0_20){vacc_percent_0_20 = 0;}

            vac_20_40_in_dates = current->record->vacc_20_40_between_dates;
            vacc_percent_20_40 = (float)vac_20_40_in_dates/(vac_20_40 + not_vac_20_40) * 100;
            if(vacc_percent_20_40 != vacc_percent_20_40){vacc_percent_20_40 = 0;}

            vac_40_60_in_dates = current->record->vacc_40_60_between_dates;
            vacc_percent_40_60 = (float)vac_40_60_in_dates/(vac_40_60 + not_vac_40_60) * 100;
            if(vacc_percent_40_60 != vacc_percent_40_60){vacc_percent_40_60 = 0;}

            vac_over_60_in_dates = current->record->vacc_over_60_between_dates;
            vacc_percent_over_60 = (float)vac_over_60_in_dates/(vac_over_60 + not_vac_over_60) * 100;
            if(vacc_percent_over_60 != vacc_percent_over_60){vacc_percent_over_60 = 0;}
            //PRINT THE RESULTS
            printf("%s\n0-20 %d %.2f%%\n", current->name, current->record->vacc_0_20_between_dates, vacc_percent_0_20);
            printf("20-40 %d %.2f%%\n", current->record->vacc_20_40_between_dates, vacc_percent_20_40);
            printf("40-60 %d %.2f%%\n", current->record->vacc_40_60_between_dates, vacc_percent_40_60);
            printf("60+ %d %.2f%%\n\n", current->record->vacc_over_60_between_dates, vacc_percent_over_60);
        }

        current = current->next;
    }
}