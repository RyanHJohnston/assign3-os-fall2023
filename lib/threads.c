#include "threads.h"
#include "lib.h"
#include "proc.h"
#include "queue.h"
#include <bits/types/cookie_io_functions_t.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

void *read_file(void *f) {
    /* FileRead *ftemp; */
    MainThread *temp;
    int i;
    FILE *fp;
    char ch;
    char line[MAX_LINE_LEN];
    char type_str[10];
    int count;
    struct timespec ts_begin, ts_end;
    double elapsed;
    long sleep_time_ms;
    sem_t sem_cpu;

    printf("file read reached\n");
    /* ftemp = (FileRead*)f; */
    temp = (MainThread*)f;
    fp = fopen(temp->input_file->filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Unable to open file %s\n", temp->input_file->filename);
        pthread_exit(NULL);
    }

    /* read the file line by line */
    /* create a new process or sleep depending on the keyword */
    /* while ( (ch = fgetc(fp)) != EOF) { */
    /*     putchar(ch); */
    /* } */

    fclose(fp);

    fp = fopen(temp->input_file->filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Unable to open file %s\n", temp->input_file->filename);
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
            temp->input_file->proc = malloc(sizeof(*temp->input_file->proc));
            temp->input_file->proc->PID = gen_rand_pid();
            temp->input_file->proc->PR = currentLine.data.processData.values[0];
            temp->input_file->proc->numCPUBurst = currentLine.data.processData.values[1] / 2+1;
            temp->input_file->proc->numIOBurst = currentLine.data.processData.values[1] / 2;
            temp->input_file->proc->CPUBurst = malloc(sizeof(int) * temp->input_file->proc->numCPUBurst);
            temp->input_file->proc->IOBurst = malloc(sizeof(int) * temp->input_file->proc->numIOBurst);

            /* get values to the dynamic arrays */

            /* get cpu bursts */
            count = temp->input_file->proc->numCPUBurst;
            for (i = 0; i < count; ++i) {
                /* printf("Value %d Index %i\n", currentLine.data.processData.values[2 + i*2], 2 + i*2); */
                temp->input_file->proc->CPUBurst[i] = currentLine.data.processData.values[2 + i*2];
            }

            /* get io bursts */
            count = temp->input_file->proc->numIOBurst;
            for (i = 0; i < count; ++i) {
                /* printf("Value %d Index %i\n", currentLine.data.processData.values[3 + i*2], 3 + i*2); */
                temp->input_file->proc->IOBurst[i] = currentLine.data.processData.values[3 + i*2];
            }

            /* assign cpuindex and ioindex for PCB */
            temp->input_file->proc->cpuindex = 0;
            temp->input_file->proc->ioindex = 0;

            clock_gettime(CLOCK_MONOTONIC, &ts_end);
            temp->input_file->proc->ts_begin = ts_begin;
            temp->input_file->proc->ts_end = ts_end;

            /* prev and next PCB are NULL for now, not sure what to do with them */
            /* i think this will be for the doubly linked list */
            temp->input_file->proc->prev = NULL;
            temp->input_file->proc->next = NULL;

            /* add PCB to ready queue */
            enqueue(temp->ready_q, temp->input_file->proc);

            /* free memory */
            free(currentLine.data.processData.values);
            free(temp->input_file->proc->CPUBurst);
            free(temp->input_file->proc);

        } else if (strcmp(type_str, "sleep") == 0) {
            currentLine.type = TYPE_SLEEP;
            sscanf(line, "%*s %d", &currentLine.data.sleepData.duration);
            printf("Processed 'sleep' with duration %d\n", currentLine.data.sleepData.duration);


        } else if (strcmp(type_str, "stop") == 0) {
            currentLine.type = TYPE_STOP;
            printf("Processed 'stop: exiting...\n");
            break;
        } else {
            fprintf(stderr, "ERROR: No enums were met when reading %s\n", temp->input_file->filename);
            exit(EXIT_FAILURE);
        }
    }





    printf("\nEnd of file read program\n\n");    
    fclose(fp);
    pthread_exit(f);
}


/* CPU scheduler thread will check ready queue
 * if there is a process, it will pick one according to the scheduling algorithm from ready queue 
 * and hold CPU resource for the given CPU burst time 
 * (or for quantum time if the scheduling algorithm is RR). 
 * That means CPU thread will simply sleep for the given CPU burst time to emulate/simulate the process. 
 * Then it will release CPU resource and put this process into I/O queue 
 * (or ready queue if RR is used) 
 * or just terminate if this is the last CPU burst.  
 * Then CPU scheduler thread will check ready queue again and repeat the same for the next process. 
 * */
void *cpu_scheduler(void *f) {
    MainThread *temp;


    temp = (MainThread*)f;
    
    printf("is empty? value = %i\n", is_empty(temp->ready_q));
    
    /* this will be done using a while(1) loop */
    if (is_empty(temp->ready_q) == 0) {
        fprintf(stderr, "ERROR: ready queue is empty\n");
        pthread_exit(NULL);
    }

    /* check algorithm name */
    if (strcmp(temp->alg, "FIFO") == 0) {
        /* fifo alg */ 
        
        printf("%s detected\n", temp->alg);
        
    } else if (strcmp(temp->alg, "SJF") == 0) {
        /* sjf alg */
        printf("%s detected\n", temp->alg);
    } else if (strcmp(temp->alg, "PR") == 0) {
        /* pr alg */
        printf("%s detected\n", temp->alg);
    } else if (strcmp(temp->alg, "RR") == 0) {
        /* pr alg */
        printf("%s detected\n", temp->alg);
    } else {
        fprintf(stderr, "ERROR: Algorithm not detected in CPUScheduler thread\n");
        pthread_exit(NULL);
    }

    
    pthread_exit(f);
}



int gen_rand_pid() {
    return PID_MIN + rand() % (PID_MAX + 1 - PID_MIN);
}









