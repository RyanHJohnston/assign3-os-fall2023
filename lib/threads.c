#include "threads.h"
#include "lib.h"
#include "proc.h"
#include "queue.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>

/**
 * @brief Reads input file, creates process, and adds to ready queue line by line
 *
 * @param MainThread *f
 */
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
    useconds_t sleep_time_ms;
    sem_t sem_cpu;

    printf("---------- FILE READ THREAD ----------\n");

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

            /* sleep for given amount of ms */
            sleep_time_ms = currentLine.data.sleepData.duration * MS_CONVERT;
            usleep(sleep_time_ms);
        } else if (strcmp(type_str, "stop") == 0) {
            currentLine.type = TYPE_STOP;
            printf("Processed 'stop: exiting...\n");
            break;
        } else {
            fprintf(stderr, "ERROR: No enums were met when reading %s\n", temp->input_file->filename);
            exit(EXIT_FAILURE);
        }
    }

    printf("---------- FILE READ THREAD END ----------\n\n");

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
void *cpu_scheduler(void *arg) {
    MainThread *main_thread;

    printf("---------- CPU SCHEDULER THREAD ----------\n");
    main_thread = (MainThread*)arg;
    useconds_t sleep_time_ms;
    int i;

    display_queue(main_thread->ready_q);

    while (true) {
        /* this will be done using a while(1) loop */
        if (!is_empty(main_thread->ready_q)) {
            /* check algorithm name */
            if (strcmp(main_thread->alg, "FIFO") == 0) {
                /* fifo alg */ 
                PCB *p;
                p = dequeue(main_thread->ready_q);
                if (p == NULL) {
                    fprintf(stderr, "ERROR: failed to dequeue process\n");
                    continue;
                }
                /* simulate the process */
                printf("Dequeued PID: %d\n", p->PID);
                display_queue(main_thread->ready_q);

                for (i = 0; i < p->numCPUBurst; ++i) {
                    sleep_time_ms = p->CPUBurst[i] * MS_CONVERT;
                    usleep(sleep_time_ms);
                }

                /* add to the io queue */
                io_enqueue(main_thread->io_q, p);

                /* once done with handling process in cpu scheduler, add to io queue */

            } else if (strcmp(main_thread->alg, "SJF") == 0) {
                /* sjf alg */
                printf("%s detected\n", main_thread->alg);
            } else if (strcmp(main_thread->alg, "PR") == 0) {
                /* pr alg */
                printf("%s detected\n", main_thread->alg);
            } else if (strcmp(main_thread->alg, "RR") == 0) {
                /* pr alg */
                printf("%s detected\n", main_thread->alg);
            } else {
                fprintf(stderr, "ERROR: Algorithm not detected in CPUScheduler thread\n");
                pthread_exit(NULL);
            }
        } else {
            printf("---------- CPU SCHEDULER THREAD END ----------\n\n");
            pthread_exit(arg);
        }
    }
    
    fprintf(stderr, "ERROR: No condition met in CPU Scheduler Thread\n");
    pthread_exit(NULL);
}

/* io system thread only uses FIFO when choosing the proc from the io queue */
void *io_system(void *arg) {
    MainThread *main_thread;

    main_thread = (MainThread*)arg;

    if (strcmp(main_thread->alg, "FIFO") == 1) {
        fprintf(stderr, "\nIO System Thread can only be used in FIFO, skipping\n\n");
        pthread_exit(NULL);
    }

    int i;
    PCB *p;
    useconds_t sleep_time_ms;
    
    printf("---------- IO SYSTEM THREAD ----------\n");
    while (true) {
        if (!io_is_empty(main_thread->io_q)) {

            p = io_dequeue(main_thread->io_q);
            if (p == NULL) {
                fprintf(stderr, "ERROR: failed to dequeue IO process\n");
            }

            printf("Dequeued PID: %d\n", p->PID);
            io_display_queue(main_thread->io_q);

            for (i = 0; i < p->numIOBurst; ++i) {
                sleep_time_ms = p->IOBurst[i] * MS_CONVERT;
                usleep(sleep_time_ms);
            }

            /* add back to ready queue  */
            enqueue(main_thread->ready_q, p);
        } else {
            printf("---------- IO SYSTEM THREAD END ----------\n\n");
            pthread_exit(arg);
        }
    }
    
    fprintf(stderr, "ERROR: No condition met in IO System Thread\n");
    pthread_exit(NULL);
}


void display_metrics(MainThread *main_thread) {
    printf("-------------------------------------------------------\n");
    printf("Input File Name                 : %s\n", main_thread->input_file->filename);
    printf("CPU Scheduling Alg              : %s\n", main_thread->alg);
    printf("CPU Utilization                 : %f%%\n", 0.0);
    printf("Throughput                      : %f processes / ms\n", 0.0);
    printf("Avg. Turnaround Time            : %f ms\n", 0.0);
    printf("Avg. Waiting Time in R Queue    : %f ms\n", 0.0);
    printf("-------------------------------------------------------\n");
}

int gen_rand_pid() {
    return PID_MIN + rand() % (PID_MAX + 1 - PID_MIN);
}









