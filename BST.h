#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stdio.h>
#include "bloomFilter.h"
#include "skipList.h"

typedef struct Node* nodeptr;

//BST VIRUS NODES
typedef struct Node{
    char* virusName;
    int* BF;                //WE KEEP A BLOOMFILTER FOR EVERY VIRUS 
    skipList* vaccinated;   //WE ALSO KEEP A VACCINATED AND A NOT VACCINATED SKIPLIST FOR IT
    skipList* notVaccinated;
    nodeptr left, right;
}node;
nodeptr root, tree;

/*------------------VIRUSES BINARY SEARCH TREE FUNCTIONS-------------------*/
void initialize(nodeptr* n, char* key, size_t bloomSize, size_t sList_size);
nodeptr searchBST(nodeptr n, char* key);
nodeptr insertBST(nodeptr* n, char* key, size_t bloomSize, size_t sList_size);
void printdata(nodeptr *n);
void deallocate(nodeptr* n);

#endif