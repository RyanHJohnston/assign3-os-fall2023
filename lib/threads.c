#include "threads.h"
#include "lib.h"
#include "proc.h"
#include "queue.h"
#include <bits/types/cookie_io_functions_t.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *read_file(void *f) {
    FileRead *ftemp;
    int i;
    FILE *fp;
    char ch;
    char line[MAX_LINE_LEN];
    char type_str[10];
    int count;
    struct timespec ts_begin, ts_end;
    double elapsed;
    long sleep_time_ms;

    printf("file read reached\n");
    ftemp = (FileRead*)f;
    fp = fopen(ftemp->filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Unable to open file %s\n", ftemp->filename);
        pthread_exit(NULL);
    }

    /* read the file line by line */
    /* create a new process or sleep depending on the keyword */
    /* while ( (ch = fgetc(fp)) != EOF) { */
    /*     putchar(ch); */
    /* } */

    fclose(fp);

    fp = fopen(ftemp->filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Unable to open file %s\n", ftemp->filename);
        pthread_exit(NULL);
    }
    
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%s", type_str);
        Line currentLine;
        
        /* if process, create PCB struct, fill it up with data, and enter into ready queue */
        if (strcmp(type_str, "proc") == 0) {
            clock_gettime(CLOCK_MONOTONIC, &ts_begin); // begin time
            currentLine.type = TYPE_PROC;
            int temp_values[MAX_LINE_LEN];
            currentLine.data.processData.count = sscanf(line, "%*s %d %d %d %d %d %d %d %d %d %d %d",
                    &temp_values[0], &temp_values[1], &temp_values[2], &temp_values[3], &temp_values[4],
                    &temp_values[5], &temp_values[6], &temp_values[7], &temp_values[8], &temp_values[9]) - 1;

            /* create PCB struct (simulated process) from read values */ 
            /* printf("Process 'proc' with %d integers\n", currentLine.data.processData.count); */
            currentLine.data.processData.values = malloc(currentLine.data.processData.count * sizeof(int));
            if (currentLine.data.processData.values == NULL) {
                fprintf(stderr, "ERROR: failed to allocate memory for proc values\n");
                fclose(fp);
                pthread_exit(NULL);
            }

            /* copy data from static array to dynamic array */
            for (i = 0; i < currentLine.data.processData.count; ++i) {
                currentLine.data.processData.values[i] = temp_values[i];
            }

            /* printf("Processed 'proc' with %d integers\n", currentLine.data.processData.count); */
    
            /* assign and/or allocate set up PCB variables */
            ftemp->proc = malloc(sizeof(*ftemp->proc));
            ftemp->proc->PID = gen_rand_pid();
            ftemp->proc->PR = currentLine.data.processData.values[0];
            ftemp->proc->numCPUBurst = currentLine.data.processData.values[1] / 2+1;
            ftemp->proc->numIOBurst = currentLine.data.processData.values[1] / 2;
            ftemp->proc->CPUBurst = malloc(sizeof(int) * ftemp->proc->numCPUBurst);
            ftemp->proc->IOBurst = malloc(sizeof(int) * ftemp->proc->numIOBurst);
            
            /* get values to the dynamic arrays */

            /* get cpu bursts */
            count = ftemp->proc->numCPUBurst;
            for (i = 0; i < count; ++i) {
                /* printf("Value %d Index %i\n", currentLine.data.processData.values[2 + i*2], 2 + i*2); */
                ftemp->proc->CPUBurst[i] = currentLine.data.processData.values[2 + i*2];
            }
            
            /* get io bursts */
            count = ftemp->proc->numIOBurst;
            for (i = 0; i < count; ++i) {
                /* printf("Value %d Index %i\n", currentLine.data.processData.values[3 + i*2], 3 + i*2); */
                ftemp->proc->IOBurst[i] = currentLine.data.processData.values[3 + i*2];
            }
            
            /* assign cpuindex and ioindex for PCB */
            ftemp->proc->cpuindex = 0;
            ftemp->proc->ioindex = 0;
            
            clock_gettime(CLOCK_MONOTONIC, &ts_end);
            ftemp->proc->ts_begin = ts_begin;
            ftemp->proc->ts_end = ts_end;
            
            /* prev and next PCB are NULL for now, not sure what to do with them */
            ftemp->proc->prev = NULL;
            ftemp->proc->next = NULL;

            /* add PCB to ready queue */
            enqueue(ftemp->ready_q, ftemp->proc);

            /* free memory */
            free(currentLine.data.processData.values);
            free(ftemp->proc->CPUBurst);
            free(ftemp->proc);

        } else if (strcmp(type_str, "sleep") == 0) {
            currentLine.type = TYPE_SLEEP;
            sscanf(line, "%*s %d", &currentLine.data.sleepData.duration);
            printf("Processed 'sleep' with duration %d\n", currentLine.data.sleepData.duration);
        } else if (strcmp(type_str, "stop") == 0) {
            currentLine.type = TYPE_STOP;
            printf("Processed 'stop: exiting...\n");
            break;
        } else {
            fprintf(stderr, "ERROR: No enums were met when reading %s\n", ftemp->filename);
            exit(EXIT_FAILURE);
        }
    }





    printf("\nEnd of file read program\n\n");    

    fclose(fp);
    pthread_exit(f);
}


int gen_rand_pid() {
    return PID_MIN + rand() % (PID_MAX + 1 - PID_MIN);
}









