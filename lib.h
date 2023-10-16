#ifndef LIB_H_
#define LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

/* -------------- THREAD STRUCTS ---------------- */



/* ----------- BASIC FUNCTIONS ---------------- */

void print_array(double *array, int length);
void check_inputs(int argc, char **argv);
char **get_argv(int argc, char **argv);
char *get_element_from_new_argv(int argc, char **new_argv, int element_index);
void print_new_argv(int argc, char **new_argv);
void free_new_argv(int argc, char **argv);

/* -------------- THREAD FUNCTIONS --------------- */



#endif // LIB_H_
