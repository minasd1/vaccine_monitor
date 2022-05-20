#include "BST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


//INITIALIZE A BST NODE
void initialize(nodeptr* n, char* key, size_t bloomSize, size_t sList_size){
    //ALLOCATE PROPER MEMORY FOR DATA AND THE DATA STRUCTURE MEMBERS
    *n = malloc(sizeof(struct Node));
    (*n)->left = (*n)->right = NULL;
    (*n)->virusName = malloc(strlen(key) + 1);
    strcpy((*n)->virusName, key);
    initialize_bloom(&((*n)->BF), bloomSize);
    (*n)->vaccinated = initialize_skipList(sList_size);
    (*n)->notVaccinated = initialize_skipList(sList_size);
}

//INSERT A NODE TO BST, RETURN A POINTER TO IT IF ALREADY EXISTS
nodeptr insertBST(nodeptr* n, char* key, size_t bloomSize, size_t sList_size){
    //IF NODE DOES NOT ALREADY EXISTS, INITIALIZE IT
    if(*n == NULL){
        initialize(n, key, bloomSize, sList_size);
        return *n;
    }
    int result = strcmp(key, (*n)->virusName);
    //IF NODE ALREADY EXISTS, RETURN A POINTER TO IT 
    if(result == 0){
        return *n;
    }
    else if(result < 0){
        return insertBST(&((*n)->left), key, bloomSize, sList_size);
    }
    else{
        return insertBST(&((*n)->right), key, bloomSize, sList_size);
    }
    
}

//SEARCH BST FOR GIVEN KEY
nodeptr searchBST(nodeptr n, char* key){
    //IF KEY NOT FOUND, RETURN NULL, ELSE RETURN A POINTER TO THE NODE WITH THAT KEY
    if(n == NULL){
        return NULL;
    }
    int result = strcmp(key,n->virusName);
    if(result == 0){
        return n;
    }
    else if(result < 0){
        return searchBST(n->left,key);
    }
    else{
        return searchBST(n->right, key);
    }
}

//PRINT BST DATA (ALL VIRUSES NAMES)
void printdata(nodeptr* n){
    if(*n == NULL){
        return;
    }
    printdata(&((*n)->left));
    printdata(&((*n)->right));

    printf("%s\n", (*n)->virusName);
}

//FREE BST MEMORY
void deallocate(nodeptr* n){
    if((*n) == NULL){
        return;
    }
    deallocate(&((*n)->left));
    deallocate(&((*n)->right));

    free((*n)->virusName);
    (*n)->virusName = NULL;
    free_bloom(&((*n)->BF));
    destroy_skipList((*n)->vaccinated);
    destroy_skipList((*n)->notVaccinated);
    free(*n);
    *n = NULL;
    
}






