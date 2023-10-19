#ifndef LIB_H_
#define LIB_H_

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>

/* -------------- THREAD STRUCTS ---------------- */



/* ----------- BASIC FUNCTIONS ---------------- */

void print_array(double *array, int length);
int is_valid(int arg, char **argk);
static int is_quantum(int length, char **args);
static int is_int(char *str);
char **get_args(int length, char **args);
char *get_element_from_new_args(int length, char **new_args, int element_index);
void print_new_args(int length, char **new_args);
void free_new_args(int length, char **args);

/* -------------- THREAD FUNCTIONS --------------- */



#endif // LIB_H_
