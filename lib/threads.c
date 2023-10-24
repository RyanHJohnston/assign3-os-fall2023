#include "threads.h"
#include <stdio.h>

void *read_file(void *f) {
    FileRead *ftemp;
    int i;
    FILE *fp;
    char ch;
    
    printf("file read reached\n");
    ftemp = (FileRead*)f;
    fp = fopen(ftemp->filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Unable to open file %s\n", ftemp->filename);
        pthread_exit(NULL);
    }

    /* read the file line by line */
    /* create a new process or sleep depending on the keyword */
    while ( (ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }

    
    /* memcpy(f, ftemp, sizeof(*ftemp)); */
    /* f = ftemp; */
    /* free(ftemp); */
    fclose(fp);
    pthread_exit(f);
}
