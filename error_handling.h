#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define __USE_XOPEN
#define _GNU_SOURCE


/*----WE USE THESE FUNCTIONS TO HANDLE EVERY POSSIBLE ERROR INPUT DURING INTERACTION WITH USER-------*/
int get_num_of_spaces(char *str);
bool is_number(char* str);
bool is_string(char* str);
bool virus_valid_name(char* str);
bool valid_date(char* date);
bool date2_greater_date1(char* date1, char* date2);




#endif