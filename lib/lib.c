#include "lib.h"


/*----------------------------------- HELPER FUNCTIONS ----------------------------------*/
/**
 * @brief Prints a given array
 *
 * @param array
 * @param length
 */
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

/**
 * @brief Validates length and args of the program
 *
 * @param length
 * @param args
 */
int is_valid(int length, char **args) {
    char **new_args;
    int i;
    int j;
    char *s;
    char *ext;
    int count;
    

    if (length != 5 && length != 7) {
        fprintf(stderr, "ERROR: The program must be run as follows:\n");
        fprintf(stderr, "main.o -alg [FIFO|SJF|PR|RR] [-quantum [integer (ms)]] -input [file name]\n");
        return FAILURE;
    }

    if (length == 7) {
        return is_quantum(length, args);
    }

    new_args = get_args(length, args);

    char alg_names[][20] = {
        "fifo",
        "sjf",
        "pr"
    };

    if (strcmp(new_args[1],"-alg") != 0) {
        fprintf(stderr, "ERROR: -alg flag must be specified\n");
        return FAILURE;
    }

    /* lowercase the string */
    s = new_args[2];
    for (i = 0; s[i]; ++i) {
        s[i] = tolower(s[i]);
    }

    count = 0;
    for (i = 0; i < sizeof(alg_names)/sizeof(alg_names[0]); ++i) {
        if (strcmp(alg_names[i], s) == 0) {
            ++count;
        }
    }

    if (count != 1) {
        fprintf(stderr, "ERROR: Given algorithm is not found\n");
        return FAILURE;
    }

    if (strcmp(new_args[3], "-input") != 0) {
        fprintf(stderr, "ERROR: -input flag must be typed\n");
        return FAILURE;
    }

    ext = strrchr(new_args[4], '.');
    if (!ext) {
        printf("%s\n", new_args[4]);
        fprintf(stderr, "ERROR: Input file must have an extension\n");
        return FAILURE;
    }

    free_new_args(length, new_args);
    return 0;
}

/**
 * @brief Validates length and args of the program is RR
 *
 * @param length
 * @param args
 */
static int is_quantum(int length, char **args) {
    int i;
    char *alg;
    char **new_args;
    char *ext;

    new_args = get_args(length, args);
    if (strcmp(new_args[1],"-alg") != 0) {
        fprintf(stderr, "ERROR: -alg flag must be specified\n");
        return FAILURE;
    }

    alg = new_args[2];
    for (i = 0; alg[i]; ++i) {
        alg[i] = tolower(alg[i]);
    }

    if (strcmp(alg, "rr") != 0) {
        fprintf(stderr, "ERROR: Type RR algorithm when using -quantum flag\n");
        return FAILURE;
    }

    if (strcmp(new_args[3], "-quantum") != 0) {
        fprintf(stderr, "ERROR: -quantum flag must be used when using RR\n");
        return FAILURE;
    }

    if (is_int(new_args[4]) != 0) {
        fprintf(stderr, "ERROR: the given int after -quantum is not an integer\n");
        return FAILURE;
    }

    if (strcmp(new_args[5], "-input") != 0) {
        fprintf(stderr, "ERROR: -input flag must be typed\n");
        return FAILURE;
    }
    
    ext = strrchr(new_args[6], '.');
    if (!ext) {
        printf("%s\n", new_args[6]);
        fprintf(stderr, "ERROR: Input file must have an extension\n");
        return FAILURE;
    }

    free_new_args(length, new_args);
    return SUCCESS;
}

/**
 * @brief Checks if given str is an int
 *
 * @param str
 * @return SUCCESS
 */
static int is_int(char *str) {
    char *endptr;
    errno = 0;

    long value = strtol(str, &endptr, 10); // Base 10

    if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN)) ||
            (errno != 0 && value == 0)) {
        return FAILURE; 
    }

    if (*endptr != '\0') {
        return FAILURE; 
    }

    return SUCCESS; 
}

/*
 * Gets args and returns a copied new args
 */
char **get_args(int length, char **args) {
    static int i;
    char **new_args = malloc((length + 1) * sizeof(*new_args));
    for (i = 0; i < length; ++i) {
        size_t length = strlen(args[i]) + 1;
        new_args[i] = malloc(length);
        memcpy(new_args[i], args[i], length);
    }
    new_args[length] = NULL;
    return new_args;
}

/*
 * Gets an element from the new args
 */
char *get_element_from_new_args(int length, char **new_args, int element_index) {
    static int i;
    char *result_str;

    if (element_index >= length || element_index < 0) {
        fprintf(stderr, "ERROR: Fetching index out of bounds, returning -1\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < length; ++i) {
        if (element_index == i) {
            size_t length = strlen(new_args[i]) + 1;
            result_str = malloc(length);
            memcpy(result_str, new_args[i], length);
            return result_str;
        }

    }

    fprintf(stderr, "ERROR: Condition should not have been reached, element not found in new_args\n");
    exit(EXIT_FAILURE);
}

/*
 * Prints out the new args
 */
void print_new_args(int length, char **new_args) {
    static int i;

    for (i = 0; i < length; ++i) {
        fprintf(stdout, "%s\n", new_args[i]);
    }
}

/*
 * Frees the copied args, use only when done using the new args
 */
void free_new_args(int length, char **args) {
    static int i;
    for (i = 0; i < length; ++i) {
        free(args[i]);
    }
    free(args);
}

/* ------------------------------- DATA STRUCTURE FUNCTIONS ---------------------------- */






/* ------------------------------- THREAD FUNCTIONS -------------------------------------*/


















