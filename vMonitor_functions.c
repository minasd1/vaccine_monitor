#include "vMonitor_functions.h"

void vaccineStatusBloom(char* command, char* args, size_t BF_size, int hash_num){
    char* id = NULL;
    char* virusName;
    int BF_position;
    int num_of_spaces = get_num_of_spaces(command); //WE USE THEM TO SPECIFY WHAT MUST BE DONE

    if(num_of_spaces == 2){
        id = strtok(args, " ");     
        //CHECK IF GIVEN ID HAS THE PROPER FORM
        if((is_number(id)) && (strlen(id) <= 5)){
            virusName = strtok(NULL, "\n");
            //CHECK IF VIRUSNAME IS GIVEN
            if(virusName != NULL){
                nodeptr virus_node = searchBST(root, virusName);
                //SEARCH BST FOR THE GIVEN VIRUS
                if(virus_node != NULL){
                    unsigned short int count = 0;
                    //CHECK THE BLOOMFILTER TO DECIDE WHETHER GIVEN ID IS OF A NOT VACCINATED OR MAYBE
                    //VACCINATED PERSON
                    for(int i = 0; i < hash_num; i++){
                        BF_position = hash_i(id, i)% BF_size;
                        if(testBit(virus_node->BF, BF_position) == false){
                            printf("NOT VACCINATED\n");
                            break;
                        }
                        else{
                            count++;
                        } 
                    }
                            
                    if(count == hash_num){printf("MAYBE\n");}
                }
                else{   //ERROR INPUT MANIPULATION
                    printf("Given virus does not exist in database!\n");
                }
            }
            else{
                printf("Error: Command must be like following:\n"
                "/vaccineStatusBloom citizenID virusname\n");
            }
        }
        else{
            printf("Error: Given id should be a number with 5 digits maximum!\n");
        }
    }
    else{
        printf("Error: Command must be like following:\n"
        "/vaccineStatusBloom citizenID virusname\n");
    }
    //break;
    free(command);
    command = NULL;
}

void vaccineStatus(char* command, char* args){
    char* id = NULL;
    char* virusName;
    int num_of_spaces = get_num_of_spaces(command);
            
    if(num_of_spaces == 1){

        // /vaccineStatus citizenID
        id = strtok(args, "\n");
        if(id != NULL){
            hashTableList* htable_node = search_hashList(id);
            if(htable_node != NULL){            //RECORD WITH GIVEN ID EXISTS
                //CALL THE APPROPRIATE FUNCTION TO GET CITIZEN VACCINATION STATUS
                print_citizen_status(&root, id);
            }
            else{   //ERROR INPUT MANIPULATION
                printf("Given id does not exist in database!\n");
            }
        }
        else{
            printf("Error: Command must be like following:\n"
            "/vaccineStatus citizenID [virusName]\n");
        }
    }
    else if(num_of_spaces == 2){                        // /vaccineStatus citizenID virusName
        id = strtok(args, " ");
        hashTableList* htable_node = search_hashList(id);
        if(htable_node != NULL){                        //RECORD WITH GIVEN ID EXISTS
            virusName = strtok(NULL, "\n");
            if(virusName != NULL){                      //IF A VIRUSNAME IS GIVEN
                nodeptr virus_node = searchBST(root, virusName);
                 //AND IT EXISTS IN OUR DATABASE
                 if(virus_node != NULL){
                    //SEARCH THE VIRUS'S VACCINATED SKIPLIST TO DECIDE WHETHER THE CITIZEN IS VACCINATED        
                    skipListNode* sNode = search_skipList(virus_node->vaccinated, id);
                    if(sNode == NULL){
                        printf("NOT VACCINATED\n");
                    }
                    else{   //GET VACCINATION DATE
                        printf("VACCINATED ON %s\n", sNode->record->dateVaccinated);
                    }
                            
                }       //ERROR INPUT MANIPULATION
                else{
                    printf("Given virus does not exist in database!\n");
                }
            }
            else{
                printf("Error: Command must be like following:\n"
                "/vaccineStatus citizenID [virusName]\n");
            }
        }
        else{
            printf("Given id does not exist in database!\n");
        }
    }
    else{
        printf("Error: Command must be like following:\n"
        "/vaccineStatus citizenID [virusName]\n");
    }
    free(command);
    command = NULL;
    
}


void populationStatus(char* command, char* args){
    char* virusName;
    char* country;
    char* date1;
    char* date2;

    int num_of_spaces = get_num_of_spaces(command);
    List* country_list = initialize_list(); //KEEP A LINKED LIST FOR COUNTRIES

    if(num_of_spaces == 1){             // /populationStatus virusname
                
        virusName = strtok(args, "\n");
        if(virusName != NULL){
            //SEARCH IF GIVEN VIRUS EXISTS IN BST
            nodeptr virus_node = searchBST(root, virusName);
            if(virus_node != NULL){
                        
                skipListNode* current = virus_node->vaccinated->header->forward[0];
                listNode* country_node;
                //TRAVERSE THE VIRUS'S VACCINATED SKIPLIST
                while(current != virus_node->vaccinated->final){ 
                    //MAKE LIST WITH COUNTRIES
                    country_node = search_list(country_list, current->hRecord->country->name);
                    if(country_node == NULL){  //NEW COUNTRY, INSERT IN COUNTRY LIST
                        country_node = insert_list(&country_list, current->hRecord->country->name);
                    }
                    //UPDATE VACCINATED POPULATION AND CONTINUE SKIPLIST TRAVERSAL       
                    country_node->record->vaccinated_pop++;
                    current = current->forward[0];
                }

                current = virus_node->notVaccinated->header->forward[0];
                //DO THE SAME FOR THE NOT VACCINATED SKIP LIST
                while(current != virus_node->notVaccinated->final){

                    country_node = search_list(country_list, current->hRecord->country->name);
                    if(country_node == NULL){
                        country_node = insert_list(&country_list, current->hRecord->country->name);
                    }

                    country_node->record->notVaccinated_pop++;
                    current = current->forward[0];
                }
                //PRINT THE RESULTS
                print_status(country_list, 0);
                        
            }       //ERROR HANDLING
            else{
                printf("Given virus does not exist in database!\n");
            }
        }
        else{
            printf("Error: Command must be like following:\n"
            "/populationStatus [country] virusName date1 date2\n");
        }
                
    }
    else if(num_of_spaces == 2){        //populationStatus country virusname
        country = strtok(args, " ");
        if(is_string(country)){
            virusName = strtok(NULL, "\n");
            if(virusName != NULL){
                nodeptr virus_node = searchBST(root, virusName);
                if(virus_node != NULL){
                    skipListNode* current = virus_node->vaccinated->header->forward[0];
                    //THIS TIME, WE ONLY INSERT ONE COUNTRY IN COUNTRY LIST
                    listNode* country_node = insert_list(&country_list, country);
                    //TRAVERSE VIRUS'S VACCINATED SKIPLIST
                    while(current != virus_node->vaccinated->final){
                        
                        if(strcmp(country, current->hRecord->country->name) == 0){
                            country_node->record->vaccinated_pop++;
                        }
                        current = current->forward[0];
                    }
                    //DO THE SAME WITH VIRUS'S NOT VACCINATED SKIPLIST
                    current = virus_node->notVaccinated->header->forward[0];
                    while(current != virus_node->notVaccinated->final){
                        if(strcmp(country, current->hRecord->country->name) == 0){
                            country_node->record->notVaccinated_pop++;
                        }
                        current = current->forward[0];
                    }
                    //PRINT THE RESULTS
                    print_status(country_list, 0);
                }       //ERROR MANIPULATION
                else{
                    printf("Given virus does not exist in database!\n");
                }
            }
            else{
                printf("Error: Command must be like following:\n"
                "/populationStatus [country] virusName date1 date2\n");
            }
        }
        else{
            printf("Error: Command must be like following:\n"
            "/populationStatus [country] virusName date1 date2\n");
        }
    }
    else if(num_of_spaces == 3){    //populationStatus virusName date1 date2
        virusName = strtok(args, " ");
        nodeptr virus_node = searchBST(root, virusName);
        if(virus_node != NULL){                 //CHECK FOR A PROPER ARGUMENTS INPUT
            date1 = strtok(NULL, " ");
            if(date1 != NULL){
                date2 = strtok(NULL, "\n");
                if(date2 != NULL){
                    if(valid_date(date1) && valid_date(date2)){ //CHECK THAT DATES HAVE VALID FORM
                        if(date2_greater_date1(date1, date2)){  //CHECK IF DATE2 IS GREATER THAN DATE1
                            skipListNode* current = virus_node->vaccinated->header->forward[0];
                            listNode* country_node;
                            //TRAVERSE VIRUS'S VACCINATED SKIPLIST
                            while(current != virus_node->vaccinated->final){ 
                                //MAKE LIST WITH COUNTRIES
                                country_node = search_list(country_list, current->hRecord->country->name);
                                if(country_node == NULL){  
                                    //NEW COUNTRY, INSERT IN COUNTRY LIST
                                    country_node = insert_list(&country_list, current->hRecord->country->name);
                                }
                                        
                                if(date2_greater_date1(date1, current->record->dateVaccinated) && date2_greater_date1(current->record->dateVaccinated, date2)){
                                    //UPDATE VACCINATED BETWEEN GIVEN DATES POPULATION
                                    country_node->record->vacc_between_dates++;
                                }
                                //AND THE VACCINATED POPULATION
                                country_node->record->vaccinated_pop++;
                                //CONTINUE VACCINATED SKIPLIST TRAVERSAL
                                current = current->forward[0];
                            }
                            //DO THE SAME FOR NON VACCINATED SKIPLIST
                            current = virus_node->notVaccinated->header->forward[0];
                            while(current != virus_node->notVaccinated->final){ 
                                //MAKE LIST WITH COUNTRIES
                                country_node = search_list(country_list, current->hRecord->country->name);
                                if(country_node == NULL){
                                    country_node = insert_list(&country_list, current->hRecord->country->name);
                                }
                                country_node->record->notVaccinated_pop++;
                                current = current->forward[0];
                            }
                            //PRINT THE RESULTS
                            print_status(country_list, 1);
                                        
                        }       //HANDLE THE ERRORS
                        else{
                            printf("Error: Date2 must be greater than date1!\n");
                        }
                    }
                    else{
                        printf("Invalid date!\n");
                    }
                            
                }
                else{
                    printf("Error: Command must be like following:\n"
                    "/populationStatus [country] virusName date1 date2\n");
                }
                       
            }
            else{
                printf("Error: Command must be like following:\n"
                "/populationStatus [country] virusName date1 date2\n");
            }
        }
        else{
            printf("Given virus does not exist in database!\n");
        }
                

    }
    else if(num_of_spaces == 4){    //popstatus country virusname date1 date2
        country = strtok(args, " ");
        if(is_string(country)){                     //CHECK IF GIVEN COUNTRY ARGUMENT IS A STRING
            virusName = strtok(NULL, " ");          //CHECK IF A VIRUSNAME IS GIVEN
            if(virusName != NULL){
                nodeptr virus_node = searchBST(root, virusName);
                if(virus_node != NULL){             //CHECK IF VIRUS EXISTS IN DATABASE
                    date1 = strtok(NULL, " ");      
                    date2 = strtok(NULL, "\n");
                    if(date2 != NULL){
                        if(valid_date(date1) && valid_date(date2)){ //CHECK IF GIVEN DATES HAVE VALID FORM
                            if(date2_greater_date1(date1, date2)){  //CHECK IF DATE2 IS GREATER THAN DATE1
                                skipListNode* current = virus_node->vaccinated->header->forward[0];
                                //THIS TIME, WE ONLY INSERT ONE COUNTRY IN COUNTRY LIST
                                listNode* country_node = insert_list(&country_list, country);
                                //TRAVERSE VIRUS'S VACCINATED SKIPLIST
                                while(current != virus_node->vaccinated->final){
                                    if(strcmp(country, current->hRecord->country->name) == 0){
                                        if(date2_greater_date1(date1, current->record->dateVaccinated) && date2_greater_date1(current->record->dateVaccinated, date2)){
                                            //UPDATE VACCINATED BETWEEN DATES POPULATION
                                            country_node->record->vacc_between_dates++;
                                        }
                                        //AND VACCINATED POPULATION
                                        country_node->record->vaccinated_pop++;
                                    }
                                    //CONTINUE VIRUS'S VACCINATED SKIPLIST TRAVERSAL
                                    current = current->forward[0];
                                }
                                //SAME WITH NOT VACCINATED SKIPLIST
                                current = virus_node->notVaccinated->header->forward[0];
                                while(current != virus_node->notVaccinated->final){
                                    if(strcmp(country, current->hRecord->country->name) == 0){
                                        country_node->record->notVaccinated_pop++;
                                    }
                                    current = current->forward[0];
                                }
                                //PRINT THE RESULTS
                                print_status(country_list, 1);
            
                            }       //HANDLE INPUT ERRORS
                            else{
                                printf("Error: Date2 must be greater than date1!\n");
                            }
                        }
                        else{
                            printf("Invalid date!\n");
                        }
                    }
                    else{
                        printf("Error: Please insert second date argument\n");
                    }
                }
            }
            else{
                printf("Given virus does not exist in database!\n");
            }
        }
        else{
            printf("Error: Command must be like following:\n"
            "/populationStatus [country] virusName date1 date2\n");
        }
    }
    else{
        printf("Error: Command must be like following:\n"
        "/populationStatus [country] virusName date1 date2\n");
    }
    destroy_list(country_list);

    free(command);
    command = NULL;    
}



void popStatusByAge(char* command, char* args){
    char* virusName;
    char* country;
    char* date1;
    char* date2;

    int num_of_spaces = get_num_of_spaces(command);
    List* country_list = initialize_list(); //KEEP A LINKED LIST FOR COUNTRIES

    if(num_of_spaces == 1){ //  /popStatusByAge virusName
                
        virusName = strtok(args, "\n");
        if(virusName != NULL){              //CHECK IF VIRUSNAME ARGUMENT IS GIVEN
            nodeptr virus_node = searchBST(root, virusName);
            if(virus_node != NULL){         //AND IF IT EXISTS IN DATABASE
                        
                skipListNode* current = virus_node->vaccinated->header->forward[0];
                listNode* country_node;
                //TRAVERSE VIRUS'S VACCINATED SKIPLIST
                while(current != virus_node->vaccinated->final){ 
                    //MAKE LIST WITH COUNTRIES
                    country_node = search_list(country_list, current->hRecord->country->name);
                    if(country_node == NULL){  
                        //NEW COUNTRY, INSERT IN COUNTRY LIST
                        country_node = insert_list(&country_list, current->hRecord->country->name);
                    }
                    //UPDATE THE PROPER AGE GROUP VACCINATED POPULATION
                    //REGARDING TO CURRENT CITIZEN RECORD'S AGE
                    if(current->hRecord->age >= 0 && current->hRecord->age < 20){
                        country_node->record->vaccinated_0_20++;
                    }
                    else if(current->hRecord->age >= 20 && current->hRecord->age < 40){
                        country_node->record->vaccinated_20_40++;
                    }
                    else if(current->hRecord->age >= 40 && current->hRecord->age < 60){
                        country_node->record->vaccinated_40_60++;
                    }
                    else{
                        country_node->record->vaccinated_over_60++;
                    }
                    
                    current = current->forward[0];
                }

                current = virus_node->notVaccinated->header->forward[0];
                //SAME WITH VIRUS'S NON VACCINATED SKIPLIST
                while(current != virus_node->notVaccinated->final){ 

                    country_node = search_list(country_list, current->hRecord->country->name);
                    if(country_node == NULL){
                        country_node = insert_list(&country_list, current->hRecord->country->name);
                    }

                    if(current->hRecord->age >= 0 && current->hRecord->age < 20){
                        country_node->record->not_vaccinated_0_20++;
                    }
                    else if(current->hRecord->age >= 20 && current->hRecord->age < 40){
                        country_node->record->not_vaccinated_20_40++;
                    }
                    else if(current->hRecord->age >= 40 && current->hRecord->age < 60){
                        country_node->record->not_vaccinated_40_60++;
                    }
                    else{
                        country_node->record->not_vaccinated_over_60++;
                    }
                            
                    current = current->forward[0];
                }
                //PRINT THE RESULTS
                print_status_age(country_list, 0);
                           
            }       //HANDLE INPUT ERRORS
            else{
                printf("Given virus does not exist in database!\n");
            }
        }
        else{
            printf("Error: Command must be like following:\n"
            "/popStatusByAge [country] virusName date1 date2\n");
        }
                
    }
    else if(num_of_spaces == 2){    // /popstatusByAge country virusname
        country = strtok(args, " ");
        if(is_string(country)){             //CHECK IF GIVEN COUNTRY ARGUMENT IS A STRING
            virusName = strtok(NULL, "\n");
            if(virusName != NULL){          //CHECK IF A VIRUSNAME IS GIVEN
                nodeptr virus_node = searchBST(root, virusName);
                if(virus_node != NULL){     //CHECK IF GIVEN VIRUS EXISTS IN DATABASE
                    skipListNode* current = virus_node->vaccinated->header->forward[0];
                    //ONLY ONE NODE IN COUNTRY LIST
                    listNode* country_node = insert_list(&country_list, country);
                    //TRAVERSE VIRUS'S VACCINATED SKIPLIST
                    while(current != virus_node->vaccinated->final){
                        if(strcmp(country, current->hRecord->country->name) == 0){
                            //UPDATE PROPER AGE GROUP VACCINATED POPULATION
                            if(current->hRecord->age >= 0 && current->hRecord->age < 20){
                                country_node->record->vaccinated_0_20++;
                            }
                            else if(current->hRecord->age >= 20 && current->hRecord->age < 40){
                                country_node->record->vaccinated_20_40++;
                            }
                            else if(current->hRecord->age >= 40 && current->hRecord->age < 60){
                                country_node->record->vaccinated_40_60++;
                            }
                            else{
                                country_node->record->vaccinated_over_60++;
                            }

                        }
                        //CONTINUE VIRUS'S VACCINATED SKIPLIST TRAVERSAL
                        current = current->forward[0];
                    }
                    //SAME WITH VIRUS'S NOT VACCINATED SKIPLIST
                    current = virus_node->notVaccinated->header->forward[0];
                    while(current != virus_node->notVaccinated->final){
                        if(strcmp(country, current->hRecord->country->name) == 0){
                            if(current->hRecord->age >= 0 && current->hRecord->age < 20){
                                country_node->record->not_vaccinated_0_20++;
                            }
                            else if(current->hRecord->age >= 20 && current->hRecord->age < 40){
                                country_node->record->not_vaccinated_20_40++;
                            }
                            else if(current->hRecord->age >= 40 && current->hRecord->age < 60){
                                country_node->record->not_vaccinated_40_60++;
                            }
                            else{
                                country_node->record->not_vaccinated_over_60++;
                            }

                        }
                        current = current->forward[0];
                    }
                    //PRINT RESULTS
                    print_status_age(country_list, 0);
                }       //HANDLE INPUT ERRORS
                else{
                    printf("Error: Command must be like following:\n"
                    "/popStatusByAge [country] virusName date1 date2\n");
                }
            }
            else{
                printf("Error: Command must be like following:\n"
                "/popStatusByAge [country] virusName date1 date2\n");
            }
        }
        else{
            printf("Error: Command must be like following:\n"
            "/popStatusByAge [country] virusName date1 date2\n");
        }
    }
    else if(num_of_spaces == 3){    // /popStatusByAge virusName date1 date2
                
        virusName = strtok(args, " ");                      //CHECK FOR PROPER ARGUMENT INPUT
        nodeptr virus_node = searchBST(root, virusName);
        if(virus_node != NULL){
            date1 = strtok(NULL, " ");
            if(date1 != NULL){
                date2 = strtok(NULL, "\n");
                if(date2 != NULL){
                    if(valid_date(date1) && valid_date(date2)){ //CHECK IF GIVEN DATES HAVE A VALID FORM
                        if(date2_greater_date1(date1, date2)){
                            skipListNode* current = virus_node->vaccinated->header->forward[0];
                            listNode* country_node;
                            //TRAVERSE VIRUS'S VACCINATED SKIPLIST
                            while(current != virus_node->vaccinated->final){ 
                                //MAKE LIST WITH COUNTRIES
                                country_node = search_list(country_list, current->hRecord->country->name);
                                if(country_node == NULL){  
                                    //NEW COUNTRY, INSERT IN LIST
                                    country_node = insert_list(&country_list, current->hRecord->country->name);
                                }
                                bool result = ((date2_greater_date1(date1, current->record->dateVaccinated)) && (date2_greater_date1(current->record->dateVaccinated, date2)));
                                //UPDATE PROPER AGE GROUP VACCINATED BETWEEN DATES AND VACCINATED POPULATION       
                                if(current->hRecord->age >= 0 && current->hRecord->age < 20){
                                    if(result){ 
                                        country_node->record->vacc_0_20_between_dates++;
                                    }
                                    country_node->record->vaccinated_0_20++;
                                }
                                else if(current->hRecord->age >= 20 && current->hRecord->age < 40){
                                    if(result){
                                        country_node->record->vacc_20_40_between_dates++;
                                    }
                                    country_node->record->vaccinated_20_40++;
                                }
                                else if(current->hRecord->age >= 40 && current->hRecord->age < 60){
                                    if(result){
                                        country_node->record->vacc_40_60_between_dates++;
                                    }
                                    country_node->record->vaccinated_40_60++;
                                }
                                else{
                                    if(result){
                                        country_node->record->vacc_over_60_between_dates++;
                                    }
                                    country_node->record->vaccinated_over_60++;
                                }
                                
                                //CONTINUE TRAVERSAL        
                                current = current->forward[0];
                            }

                            current = virus_node->notVaccinated->header->forward[0];
                            //SAME WITH NOT VACCINATED SKIPLIST
                            while(current != virus_node->notVaccinated->final){
                                country_node = search_list(country_list, current->hRecord->country->name);
                                if(country_node == NULL){
                                    country_node = insert_list(&country_list, current->hRecord->country->name);
                                }

                                if(current->hRecord->age >= 0 && current->hRecord->age < 20){
                                    country_node->record->not_vaccinated_0_20++;
                                }
                                else if(current->hRecord->age >= 20 && current->hRecord->age < 40){
                                    country_node->record->not_vaccinated_20_40++;
                                }
                                else if(current->hRecord->age >= 40 && current->hRecord->age < 60){
                                    country_node->record->not_vaccinated_40_60++;
                                }
                                else{
                                    country_node->record->not_vaccinated_over_60++;
                                }
                                country_node->record->notVaccinated_pop++; //VGALTO META

                                current = current->forward[0];
                            }
                            //PRINT RESULTS
                            print_status_age(country_list, 1);
                                        
                        }       //HANDLE INPUT ERRORS
                        else{
                            printf("Error: Date1 must be greater than date2!\n");
                        }
                    }
                    else{
                        printf("Invalid date!\n");
                    }
                            
                }
                else{
                    printf("Error: Command must be like following:\n"
                    "/popStatusByAge [country] virusName date1 date2\n");
                }
                       
            }
            else{
                printf("Error: Command must be like following:\n"
                "/popStatusByAge [country] virusName date1 date2\n");
            }
        }
        else{
            printf("Given virus does not exist in database!\n");
        }

    }
    else if(num_of_spaces == 4){    // /popStatusByAge country virusName date1 date2
        country = strtok(args, " ");        
        if(is_string(country)){             //CHECK FOR APPROPRIATE ARGUMENTS INPUT
            virusName = strtok(NULL, " ");
            if(virusName != NULL){
                nodeptr virus_node = searchBST(root, virusName);
                if(virus_node != NULL){
                    date1 = strtok(NULL, " ");
                    date2 = strtok(NULL, "\n");
                    if(date2 != NULL){
                        if(valid_date(date1) && valid_date(date2)){
                            if(date2_greater_date1(date1, date2)){
                                skipListNode* current = virus_node->vaccinated->header->forward[0];
                                //INSERT ONLY ONE COUNTRY IN COUNTRY LIST
                                listNode* country_node = insert_list(&country_list, country);
                                //TRAVERSE VIRUS'S NON VACCINATED SKIPLIST
                                while(current != virus_node->vaccinated->final){
                                    if(strcmp(country, current->hRecord->country->name) == 0){
                                        bool result = ((date2_greater_date1(date1, current->record->dateVaccinated)) && (date2_greater_date1(current->record->dateVaccinated, date2)));
                                        //UPDATE PROPER AGE GROUP VACCINATED BETWEEN DATES AND VACCINATED POPULATION
                                        if(current->hRecord->age >= 0 && current->hRecord->age < 20){
                                            if(result){ //BETWEEN DATES
                                                country_node->record->vacc_0_20_between_dates++;
                                            }
                                            country_node->record->vaccinated_0_20++;
                                        }
                                        else if(current->hRecord->age >= 20 && current->hRecord->age < 40){
                                            if(result){
                                                country_node->record->vacc_20_40_between_dates++;
                                            }
                                            country_node->record->vaccinated_20_40++;
                                        }
                                        else if(current->hRecord->age >= 40 && current->hRecord->age < 60){
                                            if(result){
                                                country_node->record->vacc_40_60_between_dates++;
                                            }
                                            country_node->record->vaccinated_40_60++;
                                        }
                                        else{
                                            if(result){
                                                country_node->record->vacc_over_60_between_dates++;
                                            }
                                            country_node->record->vaccinated_over_60++;
                                        }
                                    }
                                    current = current->forward[0];
                                }

                                current = virus_node->notVaccinated->header->forward[0];
                                //SAME FOR NOT VACCINATED SKIP LIST
                                while(current != virus_node->notVaccinated->final){
                                    if(strcmp(country, current->hRecord->country->name) == 0){
                                        if(current->hRecord->age >= 0 && current->hRecord->age < 20){
                                            country_node->record->not_vaccinated_0_20++;
                                        }
                                        else if(current->hRecord->age >= 20 && current->hRecord->age < 40){
                                            country_node->record->not_vaccinated_20_40++;
                                        }
                                        else if(current->hRecord->age >= 40 && current->hRecord->age < 60){
                                            country_node->record->not_vaccinated_40_60++;
                                        }
                                        else{
                                            country_node->record->not_vaccinated_over_60++;
                                        }
                                                
                                    }
                                    current = current->forward[0];

                                }
                                //PRINT THE RESULTS
                                print_status_age(country_list, 1);
                        
                            }       //HANDLE INPUT ERRORS
                            else{
                                printf("Error: Date1 must be greater than date2!\n");
                            }
                        }
                        else{
                            printf("Invalid date!\n");
                        }
                    }
                    else{
                        printf("Error: Command must be like following:\n"
                        "/populationStatus [country] virusName date1 date2\n");
                    }
                }else{
                    printf("Error: Given virus does not exist in database!");
                }
            }
            else{
                printf("Error: Command must be like following:\n"
                "/populationStatus [country] virusName date1 date2\n");
            }
        }
        else{
            printf("Error: Command must be like following:\n"
            "/populationStatus [country] virusName date1 date2\n");
        }
    }
    else{
        printf("Error: Command must be like following:\n"
        "/popStatusByAge [country] virusName date1 date2\n");
    }
    destroy_list(country_list);

    free(command);
    command = NULL;   
}



void insertCitizenRecord(char* command, char* args, size_t BF_size, size_t sList_size){
    char* id = NULL;
    char* firstName;
    char* lastName;
    char* country;
    char* age;
    char* virusName;
    char* isVaccinated;
    char* date;

    unsigned long BF_position;
    hashTableList* citizen_node;

    int num_of_spaces = get_num_of_spaces(command);
            
    if(num_of_spaces == 7 || num_of_spaces == 8){
        id = strtok(args, " ");                         //CHECK FOR PROPER ARGUMENT INPUT
        if(id != NULL){
            if((strlen(id) <= 5) && (is_number(id))){
                firstName = strtok(NULL, " ");
                if(firstName != NULL){
                    if(is_string(firstName)){
                        lastName = strtok(NULL, " ");
                        if(lastName != NULL){
                            if(is_string(lastName)){
                                country = strtok(NULL, " ");
                                if(country != NULL){
                                    if(is_string(country)){
                                        age = strtok(NULL, " ");
                                        if(age != NULL){
                                            int age_i = atoi(age);
                                            if(age_i <= 120 && is_number(age)){
                                                virusName = strtok(NULL, " ");
                                                if(virusName != NULL){
                                                    if(virus_valid_name(virusName)){
                                                        if(num_of_spaces == 7){
                                                            //INSERT A NOT VACCINATED RECORD
                                                            isVaccinated = strtok(NULL, "\n");
                                                            if(isVaccinated != NULL){
                                                                if(strcmp(isVaccinated, "NO") == 0){
                                                                    citizen_node = search_hashList(id);
                                                                    //CHECK FOR CITIZEN RECORD INCONSISTENCY
                                                                    if ((citizen_node != NULL) && ((strcmp(firstName, citizen_node->record->firstName) != 0) || (strcmp(lastName, citizen_node->record->lastName) != 0) || (strcmp(country, citizen_node->record->country->name) !=0) || (age_i != citizen_node->record->age))){
                                                                        printf("Inconsistent record!\n");
                                                                    }
                                                                    else if(citizen_node == NULL){    
                                                                        //NEW CITIZEN ID - INSERT GIVEN VIRUS TO BST(OR GET A POINTER TO ITS NODE IF EXISTS)
                                                                        nodeptr virus_node = insertBST(&root, virusName, BF_size, sList_size);
                                                                        //UPDATE THE CITIZEN HASH TABLE
                                                                        inserthashTable(id, firstName, lastName, country, age_i);
                                                                        //INSERT TO NOT VACCINATED SKIP LIST
                                                                        insertskipList(&(virus_node->notVaccinated), id, isVaccinated, NULL, sList_size);
                                                                        printf("Insertion successful!\n");
                                                                                
                                                                    }
                                                                    else{       //CITIZEN ID ALREADY EXISTS IN DATABASE
                                                                        nodeptr virus_node = searchBST(root, virusName);
                                                                        if(virus_node == NULL){
                                                                            //NEW VIRUS, INSERT TO VIRUSES BST
                                                                            nodeptr virus_node = insertBST(&root, virusName, BF_size, sList_size);
                                                                            //UPDATE THE CITIZEN HASH TABLE
                                                                            inserthashTable(id, firstName, lastName, country, age_i);
                                                                            //INSERT TO NOT VACCINATED SKIPLIST
                                                                            insertskipList(&(virus_node->notVaccinated), id, isVaccinated, NULL, sList_size);
                                                                            printf("Insertion successful!\n");
                                                                        }
                                                                        else{
                                                                        //VIRUS ALREADY EXISTS IN DATABASE, SO AS THE CITIZEN ID
                                                                        //SEARCH IF ALREADY VACCINATED
                                                                            skipListNode* isVaccinated_node = search_skipList(virus_node->vaccinated, id);
                                                                            if(isVaccinated_node == NULL){  //IF NOT IN VACCINATED SKIPLIST
                                                                                //AND ALSO NOT IN NON VACCINATED SKIPLIST
                                                                                if(search_skipList(virus_node->notVaccinated, id) == NULL){
                                                                                    //UPDATE PROPER DATA STRUCTURES
                                                                                    inserthashTable(id, firstName, lastName, country, age_i);
                                                                                    insertskipList(&(virus_node->notVaccinated), id, isVaccinated, NULL, sList_size);
                                                                                    printf("Insertion successful!\n");
                                                                                }
                                                                                else{
                                                                                    //ALREADY EXISTS IN NOT VACCINATED SKIPLIST
                                                                                    printf("Error: Record already exists!\n");
                                                                                }
                                                                            }       
                                                                            else{
                                                                                printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %s\n", citizen_node->id, isVaccinated_node->record->dateVaccinated);
                                                                            }
                                                                        }
                                                                    }
                                                                            

                                                                }       //HANDLE INPUT ERRORS
                                                                else if(strcmp(isVaccinated, "YES") == 0){
                                                                    printf("Error: Please insert date of vaccination!\n");
                                                                }
                                                                else{
                                                                    printf("Error: Isvaccinated argument should be YES or NO!\n");
                                                                }
                                                            }
                                                            else{
                                                                printf("Error: Command must be like following:\n"
                                                                "/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]\n");
                                                            }
                                                        }
                                                        else{
                                                            //INSERT A VACCINATED RECORD
                                                            isVaccinated = strtok(NULL, " ");
                                                            if(isVaccinated != NULL){                   //CHECK FOR PROPER INPUT
                                                                if(strcmp(isVaccinated, "YES") == 0){
                                                                    date = strtok(NULL, "\n");
                                                                    if(date != NULL){
                                                                        char* date_cp = (char*)malloc(strlen(date) + 1);
                                                                        strcpy(date_cp, date);
                                                                        if(valid_date(date)){
                                                                            citizen_node = search_hashList(id); //IF CITIZEN ID EXISTS
                                                                            //CHECK FOR INPUT INCONSISTENCIES
                                                                            if ((citizen_node != NULL) && ((strcmp(firstName, citizen_node->record->firstName) != 0) || (strcmp(lastName, citizen_node->record->lastName) != 0) || (strcmp(country, citizen_node->record->country->name) !=0) || (age_i != citizen_node->record->age))){
                                                                                printf("Inconsistent record!\n");
                                                                            }
                                                                            else if(citizen_node == NULL){    
                                                                                //NEW CITIZEN ID - UPDATE VIRUS'S BST WITH GIVEN VIRUS
                                                                                //OR GET A POINTER TO VIRUSNODE IF ALREADY EXISTS
                                                                                nodeptr virus_node = insertBST(&root, virusName, BF_size, sList_size);
                                                                                //UPDATE CITIZEN HASH TABLE
                                                                                inserthashTable(id, firstName, lastName, country, age_i);
                                                                                for(int i = 0; i < 16; i++){    
                                                                                    //UPDATE VIRUS'S BLOOM FILTER
                                                                                    BF_position = hash_i(id, i) % BF_size;
                                                                                    setBit(virus_node->BF, BF_position);
                                                                                }
                                                                                //INSERT TO VACCINATED SKIPLIST
                                                                                insertskipList(&(virus_node->vaccinated), id, isVaccinated, date_cp, sList_size);
                                                                                printf("Insertion successful!\n");
                                                                                        
                                                                            }
                                                                            else{       //CITIZEN ID ALREADY EXISTS IN DATABASE
                                                                                nodeptr virus_node = searchBST(root, virusName);
                                                                                if(virus_node == NULL){
                                                                                    //NEW VIRUS - INSERT TO VIRUSES BST
                                                                                    nodeptr virus_node = insertBST(&root, virusName, BF_size, sList_size);
                                                                                    for(int i = 0; i < 16; i++){    
                                                                                        //UPDATE VIRUS'S BLOOM FILTER
                                                                                        BF_position = hash_i(id, i) % BF_size;
                                                                                        setBit(virus_node->BF, BF_position);
                                                                                    }
                                                                                    //INSERT TO VIRUS'S VACCINATED SKIPLIST
                                                                                    insertskipList(&(virus_node->vaccinated), id, isVaccinated, date_cp, sList_size);
                                                                                    printf("Insertion successful!\n");
                                                                                }
                                                                                else{   //VIRUS ALREADY EXISTS
                                                                                    skipListNode* isVaccinated_node = search_skipList(virus_node->vaccinated, id);
                                                                                    if(isVaccinated_node == NULL){//NOT ALREADY VACCINATED
                                                                                
                                                                                        for(int i = 0; i < 16; i++){    
                                                                                            //UPDATE VIRUS'S BLOOM FILTER
                                                                                            BF_position = hash_i(id, i) % BF_size;
                                                                                            setBit(virus_node->BF, BF_position);
                                                                                        }
                                                                                        if(search_skipList(virus_node->notVaccinated, id) != NULL){
                                                                                            //REMOVE FROM NOT VACCINATED, ADD TO VACCINATED
                                                                                            delete_skipListNode(virus_node->notVaccinated, id, sList_size);
                                                                                        }
                                                                                        //UPDATE VIRUS'S VACCINATED SKIPLIST
                                                                                        insertskipList(&(virus_node->vaccinated), id, isVaccinated, date_cp, sList_size);
                                                                                        printf("Insertion successful!\n");
                                                                                    }
                                                                                    else{
                                                                                        printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %s\n", citizen_node->id, isVaccinated_node->record->dateVaccinated);
                                                                                    }
                                                                                }
                                                                            }
                                                                        }               //HANDLE INPUT ERRORS
                                                                        else{
                                                                            printf("Error: Invalid date!\n");
                                                                        }
                                                                        free(date_cp);
                                                                        date_cp = NULL;
                                                                    }
                                                                    else{
                                                                        printf("Please insert date of vaccination!\n");
                                                                    }
                                                                }
                                                                else if(strcmp(isVaccinated, "NO") == 0)
                                                                {
                                                                    printf("Error: Date argument must not be inserted!\n");
                                                                }
                                                                else{
                                                                    printf("Error: Isvaccinated field must be YES OR NO\n");
                                                                }
                                                                    
                                                            }
                                                            else{
                                                                printf("Error: Command must be like following:\n"
                                                                "/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]\n");
                                                            }
                                                        }
                                                                
                                                    }
                                                    else{
                                                        printf("Error: Given virus name should only consist of"
                                                        "alphabet letters, numbers and -\n");
                                                    }
                                                }
                                                else{
                                                    printf("Error: Command must be like following:\n"
                                                    "/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]\n");
                                                }
                                            }
                                            else{
                                                printf("Error: Given age should be a number between 0 and 120!\n");
                                            }
                                        }
                                        else{
                                            printf("Error: Command must be like following:\n"
                                            "/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]\n");
                                        }
                                                
                                    }
                                    else{
                                        printf("Error: Given country should only contain alphabet letters!\n");
                                    }
                                }
                                else{
                                    printf("Error: Command must be like following:\n"
                                    "/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]\n");
                                }
                            }
                            else{
                                printf("Error: Given lastname should only contain alphabet letters!\n");
                            }
                                    
                        }
                        else{
                            printf("Error: Command must be like following:\n"
                            "/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]\n");
                        }
                    }
                    else{
                        printf("Error: Given firstname should only contain alphabet letters!\n");
                    }
                }
                else{
                    printf("Error: Command must be like following:\n"
                    "/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]\n");
                }
            }
            else{
                printf("Error: Given id should be a number with 5 digits maximum!\n");
            }
        }
        else{
            printf("Error: Command must be like following:\n"
            "/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]\n");
        }
    }
    else{
        printf("Error: Command must be like following:\n"
        "/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]\n");
    }
    free(command);
    command = NULL;
}



void vaccinateNow(char* command, char* args, size_t BF_size, size_t sList_size){
    char* id = NULL;        //VACCINATE NOW ARGUMENTS
    char* firstName;
    char* lastName;
    char* country;
    char* age;
    char* virusName;

    int BF_position;
    int num_of_spaces = get_num_of_spaces(command);

    if(num_of_spaces == 6){
        struct tm *tm;          //GET TODAY'S DATE
        time_t t;
        char todays_date[11];
        t = time(NULL);
        tm = localtime(&t);
        strftime(todays_date, sizeof(todays_date), "%d-%m-%Y", tm);

        id = strtok(args, " ");
        hashTableList* citizen_node = search_hashList(id);
        if(citizen_node != NULL){           //RECORD WITH GIVEN ID ALREADY EXISTS
            firstName = strtok(NULL, " ");  //CHECK FOR PROPER INPUT AND CITIZEN CONSISTENCY
            if(strcmp(firstName, citizen_node->record->firstName) == 0){
                lastName = strtok(NULL, " ");
                if(lastName != NULL){
                    if(strcmp(lastName, citizen_node->record->lastName) == 0){
                        country = strtok(NULL, " ");
                        if(country != NULL){
                            if(strcmp(country, citizen_node->record->country->name) == 0){
                                age = strtok(NULL, " ");
                                if(age != NULL){
                                    int age_i = atoi(age);
                                    if(age_i == citizen_node->record->age){
                                        virusName = strtok(NULL, "\n");
                                        if(virusName != NULL){
                                            nodeptr virus_node = searchBST(root, virusName);
                                            if(virus_node != NULL){     
                                                //VIRUS ALREADY EXISTS
                                                skipListNode* isVaccinated_node = search_skipList(virus_node->vaccinated, id);
                                                if(isVaccinated_node == NULL){  //CITIZEN NOT VACCINATED
                                                    for(int i = 0; i < 16; i++){    
                                                        //UPDATE VIRUS'S BLOOM FILTER
                                                        BF_position = hash_i(id, i) % BF_size;
                                                        setBit(virus_node->BF, BF_position);
                                                    }
                                                    isVaccinated_node = search_skipList(virus_node->notVaccinated, id);
                                                    //IF CITIZEN ALREADY EXISTS IN VIRUS'S NOT VACCINATED SKIPLIST
                                                    if(isVaccinated_node != NULL){
                                                        //REMOVE CITIZEN FROM IT
                                                        delete_skipListNode(virus_node->notVaccinated, id, sList_size);
                                                    }
                                                    //AND ADD TO VIRUS'S VACCINATED SKIPLIST
                                                    insertskipList(&(virus_node->vaccinated), id, "YES", todays_date, sList_size);
                                                    printf("Vaccination successful!\n");
                                                }
                                                else{
                                                    printf("ERROR: CITIZEN %s ALREADY VACCINATED ON %s\n", citizen_node->id, isVaccinated_node->record->dateVaccinated);
                                                }
                                            }
                                            else{   //NEW VIRUS - INSERT TO VIRUSES BST  
                                                virus_node = insertBST(&root, virusName, BF_size, sList_size);
                                                
                                                for(int i = 0; i < 16; i++){    
                                                    //UPDATE VIRUS'S BLOOM FILTER
                                                    BF_position = hash_i(id, i) % BF_size;
                                                    setBit(virus_node->BF, BF_position);
                                                }
                                                //AND INSERT TO GIVEN VIRUS'S VACCINATED SKIPLIST
                                                insertskipList(&(virus_node->vaccinated), id, "YES", todays_date, sList_size);
                                                printf("Vaccination successful!\n");
                                            }
                                        }       //HANLDE INPUT ERRORS
                                        else{
                                            printf("Error: Command must be like following:\n"
                                            "/vaccinateNow citizenID firstName lastName country age virusName\n");
                                        }
                                    }
                                    else{
                                        printf("Inconsistent age!\n");
                                    }
                                }
                                else{
                                    printf("Error: Command must be like following:\n"
                                    "/vaccinateNow citizenID firstName lastName country age virusName\n");
                                }
                            }
                            else{
                                printf("Inconsistent country!\n");
                            }
                        }
                        else{
                            printf("Error: Command must be like following:\n"
                            "/vaccinateNow citizenID firstName lastName country age virusName\n");
                        }
                    }
                    else{
                        printf("Inconsistent lastname!\n");
                    }
                }
                else{
                    printf("Error: Command must be like following:\n"
                    "/vaccinateNow citizenID firstName lastName country age virusName\n");
                }
            }
            else{
                printf("Inconsistent name!\n");
            }

        }
        else{                       //NEW CITIZEN ID
            if(id != NULL){                             //CHECK FOR PROPER INPUT
                if((strlen(id) <= 5) && is_number(id)){
                    firstName = strtok(NULL, " ");
                    if(firstName != NULL){
                        if(is_string(firstName)){
                            lastName = strtok(NULL, " ");
                            if(lastName != NULL){
                                if(is_string(lastName)){
                                    country = strtok(NULL, " ");
                                    if(country != NULL){
                                        if(is_string(country)){
                                            age = strtok(NULL, " ");
                                            if(age != NULL){   
                                                int age_i = atoi(age);
                                                if(age_i <= 120 && is_number(age)){
                                                    virusName = strtok(NULL, "\n");
                                                    if(virusName != NULL){  
                                                        //INSERT GIVEN VIRUS TO VIRUSES BST
                                                        //OR GET A POINTER TO ITS' NODE IF ALREADY EXISTS
                                                        nodeptr virus_node = insertBST(&root, virusName, BF_size, sList_size);
                                                        //UPDATE CITIZEN HASH TABLE
                                                        inserthashTable(id, firstName, lastName, country, age_i);
                                                        for(int i = 0; i < 16; i++){    
                                                            //UPDATE VIRUS'S BLOOM FILTER
                                                            BF_position = hash_i(id, i) % BF_size;
                                                            setBit(virus_node->BF, BF_position);
                                                        }
                                                        //INSERT TO VIRUS'S VACCINATED SKIPLIST
                                                        insertskipList(&(virus_node->vaccinated), id, "YES", todays_date, sList_size);
                                                        printf("Vaccination successful!\n");
                                                    }       //HANDLE INPUT ERRORS
                                                    else{
                                                        printf("Error: Command must be like following:\n"
                                                        "/vaccinateNow citizenID firstName lastName country age virusName\n");
                                                    }
                                                }
                                                else{
                                                    printf("Error: Given age should be a number between 0 and 120!\n");
                                                }

                                            }
                                            else{
                                                printf("Error: Command must be like following:\n"
                                                "/vaccinateNow citizenID firstName lastName country age virusName\n");
                                            }
                                        }
                                        else{
                                            printf("Error: Given country should only contain alphabet letters!\n");
                                        }
                                    }
                                    else{
                                        printf("Error: Command must be like following:\n"
                                        "/vaccinateNow citizenID firstName lastName country age virusName\n");
                                    }
                                }
                                else{
                                    printf("Error: Given lastname should only contain alphabet letters!\n"); 
                                }
                            }
                            else{
                                printf("Error: Command must be like following:\n"
                                "/vaccinateNow citizenID firstName lastName country age virusName\n");
                            }
                        }
                        else{
                            printf("Error: Given firstname should only contain alphabet letters!\n");
                        }
                    }
                    else{
                        printf("Error: Command must be like following:\n"
                        "/vaccinateNow citizenID firstName lastName country age virusName\n");
                    }
                }
                else{
                    printf("Error: Given id should be a number with 5 digits maximum!\n");
                }
            }
            else{
                printf("Error: Command must be like following:\n"
                "/vaccinateNow citizenID firstName lastName country age virusName\n");
            }
                    
        }
                
    }
    else{
        printf("Error: Command must be like following:\n"
        "/vaccinateNow citizenID firstName lastName country age virusName\n");
    }
    free(command);
    command = NULL;
}



void list_nonVaccinated_Persons(char* command, char* args){
    char* virusName;

    int num_of_spaces = get_num_of_spaces(command);

    if(num_of_spaces == 1){             
        virusName = strtok(args, "\n");
        if(virusName != NULL){          //IF VIRUSNAME IS GIVEN

            nodeptr virus_node = searchBST(root, virusName);
            //IF GIVEN VIRUS EXISTS IN DATABASE
            if(virus_node != NULL){
                //CALL PROPER FUNCTION TO PRINT ITS' NOT VACCINATED LIST
                print_skipList(virus_node->notVaccinated);
            }       //HANDLE INPUT ERRORS
            else{
                printf("Given virus does not exist in database!\n");
            }
        }
        else{
            printf("Error: Command must be like following:\n"
            "/list-nonVaccinated-Persons virusName\n");
        }
    }
    else{
        printf("Error: Command must be like following:\n"
        "/list-nonVaccinated-Persons virusName\n");
    }
    free(command);
    command = NULL;
}

