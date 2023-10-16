#include "lib.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*----------------------------------- HELPER FUNCTIONS ----------------------------------*/
void print_array(double *array, int length) {
    if (array == NULL) {
        fprintf(stderr, "ERROR: Array in print_array is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    int i;
    for (i = 0; i < length; ++i) {
        fprintf(stdout, "%f\n", array[i]);
    }
    
    return;
}

void check_inputs(int argc, char **argv) {
    char **new_argv;
    int i;
    int j;
    /* char **alg_names; */
    const unsigned int alg_name_size;

    char strings[][20] = {
        "Hello",
        "World",
        "This is",
        "An array",
        "Of strings"
    };


    /* if (argc != 5) { */
    /*     fprintf(stderr, "ERROR: The program must be run as follows:\n"); */
    /*     fprintf(stderr, "main.o -alg [FIFO|SJF|PR|RR] [-quantum [integer (ms)]] -input [file name]\n"); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    new_argv = get_argv(argc, argv);
    char alg_names[][20] = {
        "fifo",
        "sjf",
        "pr",
        "rr"
    };

    char *s;
    
    /* lowercase the string */
    s = get_element_from_new_argv(argc, new_argv, 2);
    for (i = 0; s[i]; ++i) {
        s[i] = tolower(s[i]);
    }
    
    int count;

    count = 0;
    for (i = 0; i < 4; ++i) {
        if (strcmp(alg_names[i], s) == 0) {
            fprintf(stdout, "Success!\n");
            ++count;
        }
    }

    if (count != 0) {
        fprintf(stderr, "ERROR: Given algorithm is not found\n");
        exit(EXIT_FAILURE);
    }

    free(s);
    free_new_argv(argc, new_argv);
}

char **get_argv(int argc, char **argv) {
    static int i;
    char **new_argv = malloc((argc + 1) * sizeof(*new_argv));
    for (i = 0; i < argc; ++i) {
        size_t length = strlen(argv[i]) + 1;
        new_argv[i] = malloc(length);
        memcpy(new_argv[i], argv[i], length);
    }
    new_argv[argc] = NULL;
    return new_argv;
}

char *get_element_from_new_argv(int argc, char **new_argv, int element_index) {
    static int i;
    char *result_str;
    
    if (element_index >= argc || element_index < 0) {
        fprintf(stderr, "ERROR: Fetching index out of bounds, returning -1\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < argc; ++i) {
        if (element_index == i) {
            size_t length = strlen(new_argv[i]) + 1;
            result_str = malloc(length);
            memcpy(result_str, new_argv[i], length);
            return result_str;
        }
        
    }

    fprintf(stderr, "ERROR: Condition should not have been reached, element not found in new_argv\n");
    exit(EXIT_FAILURE);
}

void print_new_argv(int argc, char **new_argv) {
    static int i;

    for (i = 0; i < argc; ++i) {
        fprintf(stdout, "%s\n", new_argv[i]);
    }
}

void free_new_argv(int argc, char **argv) {
    static int i;
    for (i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    free(argv);
}





/* ------------------------------- THREAD FUNCTIONS -------------------------------------*/
