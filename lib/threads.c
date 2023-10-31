#include "threads.h"
#include "lib.h"
#include "proc.h"
#include "queue.h"
#include <bits/time.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief Reads input file, creates process, and adds to ready queue line by line
 *
 * @param MainThread *f
 */
void *read_file(void *f) {
    /* FileRead *fmain_thread; */
    MainThread *main_thread;
    int i;
    FILE *fp;
    char ch;
    char line[MAX_LINE_LEN];
    char type_str[10];
    int count;
    struct timespec ts_begin;
    struct timespec ts_end;
    struct timespec ts_turnaround_begin;
    double elapsed;
    useconds_t sleep_time_ms;
    sem_t sem_cpu;

    main_thread = (MainThread*)f;
    fp = fopen(main_thread->input_file->filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Unable to open file %s\n", main_thread->input_file->filename);
        pthread_exit(NULL);
    }

    /* read the file line by line */
    /* create a new process or sleep depending on the keyword */
    /* while ( (ch = fgetc(fp)) != EOF) { */
    /*     putchar(ch); */
    /* } */

    fclose(fp);

    fp = fopen(main_thread->input_file->filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: Unable to open file %s\n", main_thread->input_file->filename);
        pthread_exit(NULL);
    }

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%s", type_str);
        Line currentLine;

        /* if process, create PCB struct, fill it up with data, and enter into ready queue */
        if (strcmp(type_str, "proc") == 0) {
            currentLine.type = TYPE_PROC;
            int main_thread_values[MAX_LINE_LEN];
            currentLine.data.processData.count = sscanf(line, "%*s %d %d %d %d %d %d %d %d %d %d %d",
                    &main_thread_values[0], &main_thread_values[1], &main_thread_values[2], &main_thread_values[3], &main_thread_values[4],
                    &main_thread_values[5], &main_thread_values[6], &main_thread_values[7], &main_thread_values[8], &main_thread_values[9]) - 1;

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
                currentLine.data.processData.values[i] = main_thread_values[i];
            }

            /* printf("Processed 'proc' with %d integers\n", currentLine.data.processData.count); */
            /* assign and/or allocate set up PCB variables */
            main_thread->input_file->proc = malloc(sizeof(*main_thread->input_file->proc));
            main_thread->input_file->proc->PID = gen_rand_pid();
            main_thread->input_file->proc->PR = currentLine.data.processData.values[0];
            main_thread->input_file->proc->numCPUBurst = currentLine.data.processData.values[1] / 2+1;
            main_thread->input_file->proc->numIOBurst = currentLine.data.processData.values[1] / 2;
            main_thread->input_file->proc->CPUBurst = malloc(sizeof(int) * main_thread->input_file->proc->numCPUBurst);
            main_thread->input_file->proc->IOBurst = malloc(sizeof(int) * main_thread->input_file->proc->numIOBurst);

            /* get values to the dynamic arrays */

            /* get cpu bursts */
            count = main_thread->input_file->proc->numCPUBurst;
            for (i = 0; i < count; ++i) {
                /* printf("Value %d Index %i\n", currentLine.data.processData.values[2 + i*2], 2 + i*2); */
                main_thread->input_file->proc->CPUBurst[i] = currentLine.data.processData.values[2 + i*2];
            }

            /* get io bursts */
            count = main_thread->input_file->proc->numIOBurst;
            for (i = 0; i < count; ++i) {
                main_thread->input_file->proc->IOBurst[i] = currentLine.data.processData.values[3 + i*2];
            }

            /* assign cpuindex and ioindex for PCB */
            main_thread->input_file->proc->cpuindex = 0;
            main_thread->input_file->proc->ioindex = 0;

            /* prev and next PCB are NULL for now, not sure what to do with them */
            /* i think this will be for the doubly linked list */
            main_thread->input_file->proc->prev = NULL;
            main_thread->input_file->proc->next = NULL;

            /* starts tracking time of the process entering the ready queue */
            /* ts_begin is for the waiting time */
            /* ts_turnaround_begin is for the turnaround time */
            clock_gettime(CLOCK_MONOTONIC, &ts_begin);
            /* clock_gettime(CLOCK_MONOTONIC, &ts_turnaround_begin); */

            main_thread->input_file->proc->ts_begin = ts_begin;
            /* main_thread->input_file->proc->ts_turnaround_begin = ts_turnaround_begin; */

            /* iterate total number of processes */
            ++main_thread->total_num_processes;

            /* add PCB to ready queue */
            enqueue(main_thread->ready_q, main_thread->input_file->proc);

            /* free memory */
            free(currentLine.data.processData.values);
            free(main_thread->input_file->proc->CPUBurst);
            free(main_thread->input_file->proc);

        } else if (strcmp(type_str, "sleep") == 0) {
            currentLine.type = TYPE_SLEEP;
            sscanf(line, "%*s %d", &currentLine.data.sleepData.duration);

            /* clock_gettime(CLOCK_MONOTONIC, &ts_begin); */
            sleep_time_ms = currentLine.data.sleepData.duration;
            usleep(sleep_time_ms * MS_CONVERT);
            /* clock_gettime(CLOCK_MONOTONIC, &ts_end); */
            /* elapsed = ts_end.tv_sec - ts_begin.tv_sec; */
            /* elapsed += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0; */
            /* printf("elepsed time = %.3lf ms\n\n", elapsed * MS_CONVERT); */
            /* main_thread->total_wait_time += elapsed; */
        } else if (strcmp(type_str, "stop") == 0) {
            currentLine.type = TYPE_STOP;
            break;
        } else {
            fprintf(stderr, "ERROR: No enums were met when reading %s\n", main_thread->input_file->filename);
            exit(EXIT_FAILURE);
        }
    }

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

    main_thread = (MainThread*)arg;
    double wait_time;
    double throughput;
    double throughput_elapsed_time;
    double turnaround_time_ms;
    useconds_t sleep_time_ms;
    int i;
    int processes_handled;
    struct timespec ts_begin;
    struct timespec ts_end;
    struct timespec ts_turnaround_begin;
    struct timespec ts_turnaround_end;

    /* init processes handled and begin measuring time taken for each process */
    processes_handled = 0;
    clock_gettime(CLOCK_MONOTONIC, &ts_begin);

    while (true) {
        /* this will be done using a while(1) loop */
        if (!is_empty(main_thread->ready_q)) {
            /* check algorithm name */
            if (strcmp(main_thread->alg, "FIFO") == 0) {
                /* fifo alg */
                struct timespec ts_now;
                clock_gettime(CLOCK_MONOTONIC, &ts_now);
                ++processes_handled;

                PCB *p;
                p = dequeue(main_thread->ready_q);

                /* gets the wait time of the processes in the ready queue */
                wait_time = (ts_now.tv_sec - p->ts_begin.tv_sec);
                wait_time += (ts_now.tv_nsec - p->ts_begin.tv_nsec) / SEC_CONVERT;

                /* assigns waiting time ready queue for PCB and iterates total waitimg time */
                p->wait_time_ready_q = wait_time;
                main_thread->total_wait_time += wait_time;

                if (p == NULL) {
                    fprintf(stderr, "ERROR: failed to dequeue process\n");
                    continue;
                }
                
                /* simulate the process */
                clock_gettime(CLOCK_MONOTONIC, &ts_turnaround_begin);
                for (i = 0; i < p->numCPUBurst; ++i) {
                    sleep_time_ms = p->CPUBurst[i] * MS_CONVERT;
                    usleep(sleep_time_ms);
                    main_thread->total_cpu_active_time += (double)sleep_time_ms / 1000.0;
                }
                clock_gettime(CLOCK_MONOTONIC, &ts_turnaround_end);
                turnaround_time_ms = (ts_turnaround_end.tv_sec - ts_turnaround_begin.tv_sec) * MS_CONVERT;
                turnaround_time_ms += (ts_turnaround_end.tv_nsec - ts_turnaround_begin.tv_nsec) / SEC_CONVERT;
                main_thread->total_turnaround_time += turnaround_time_ms;

                /* add to the io queue */
                io_enqueue(main_thread->io_q, p);

                /* once done with handling process in cpu scheduler, add to io queue */

            } else if (strcmp(main_thread->alg, "SJF") == 0) {
                /* sjf alg */
                PCB *p = find_shortest_job(main_thread->ready_q);
                if (p == NULL) {
                    fprintf(stderr, "ERROR: No process found for SJF\n");
                    continue;
                }
                remove_from_queue(main_thread->ready_q, p);  // Assuming this function removes a given PCB from the queue

                /* simulate the process */
                clock_gettime(CLOCK_MONOTONIC, &ts_turnaround_begin);
                for (i = 0; i < p->numCPUBurst; ++i) {
                    sleep_time_ms = p->CPUBurst[i] * MS_CONVERT;
                    usleep(sleep_time_ms);
                    main_thread->total_cpu_active_time += (double)sleep_time_ms / 1000.0;
                }
                clock_gettime(CLOCK_MONOTONIC, &ts_turnaround_end);
                turnaround_time_ms = (ts_turnaround_end.tv_sec - ts_turnaround_begin.tv_sec) * MS_CONVERT;
                turnaround_time_ms += (ts_turnaround_end.tv_nsec - ts_turnaround_begin.tv_nsec) / SEC_CONVERT;
                main_thread->total_turnaround_time += turnaround_time_ms;

                /* add to the io queue */
                io_enqueue(main_thread->io_q, p);
            } else if (strcmp(main_thread->alg, "PR") == 0) {
                /* pr alg */
                PCB *p = find_highest_priority_job(main_thread->ready_q);

                if (p == NULL) {
                    fprintf(stderr, "ERROR: No process found for PR\n");
                    continue;
                }

                remove_from_queue(main_thread->ready_q, p);  // Assuming this function removes a given PCB from the queue

                /* simulate the process */
                clock_gettime(CLOCK_MONOTONIC, &ts_turnaround_begin);
                for (i = 0; i < p->numCPUBurst; ++i) {
                    sleep_time_ms = p->CPUBurst[i] * MS_CONVERT;
                    usleep(sleep_time_ms);
                    main_thread->total_cpu_active_time += (double)sleep_time_ms / 1000.0;
                }
                clock_gettime(CLOCK_MONOTONIC, &ts_turnaround_end);
                turnaround_time_ms = (ts_turnaround_end.tv_sec - ts_turnaround_begin.tv_sec) * MS_CONVERT;
                turnaround_time_ms += (ts_turnaround_end.tv_nsec - ts_turnaround_begin.tv_nsec) / SEC_CONVERT;
                main_thread->total_turnaround_time += turnaround_time_ms;

                /* add to the io queue */
                io_enqueue(main_thread->io_q, p);
            } else if (strcmp(main_thread->alg, "RR") == 0) {
                /* RR alg */
                printf("%s detected\n", main_thread->alg);
            } else {
                fprintf(stderr, "ERROR: Algorithm not detected in CPUScheduler thread\n");
                pthread_exit(NULL);
            }
        } else {

            /* measure throughput given the process count and times */
            clock_gettime(CLOCK_MONOTONIC, &ts_end);

            throughput_elapsed_time = (ts_end.tv_sec - ts_begin.tv_sec) * MS_CONVERT;
            throughput_elapsed_time += (ts_end.tv_nsec - ts_begin.tv_nsec) / SEC_CONVERT;
            throughput = processes_handled / throughput_elapsed_time;
            main_thread->throughput = throughput;

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
    PCB *p;
    struct timespec ts_begin;
    struct timespec ts_end;
    struct timespec ts_turnaround_begin;
    struct timespec ts_turnaround_end;
    int i;
    int processes_handled;
    double throughput_elapsed_time;
    double throughput;
    double wait_time;
    double turnaround_time_ms;
    useconds_t sleep_time_ms;

    if (strcmp(main_thread->alg, "FIFO") == 1) {
        fprintf(stderr, "\nIO System Thread can only be used in FIFO, skipping\n\n");
        pthread_exit(NULL);
    }

    /* init processes handled and begin measuring time taken for each process */
    processes_handled = 0;
    clock_gettime(CLOCK_MONOTONIC, &ts_begin);

    while (true) {
        if (!io_is_empty(main_thread->io_q)) {
            struct timespec ts_now;
            clock_gettime(CLOCK_MONOTONIC, &ts_now);
            ++processes_handled;

            p = io_dequeue(main_thread->io_q);

            /* gets the wait time of the processes in the ready queue */
            wait_time = (ts_now.tv_sec - p->ts_begin.tv_sec);
            wait_time += (ts_now.tv_nsec - p->ts_begin.tv_nsec) / SEC_CONVERT;
            p->wait_time_ready_q = wait_time;
            main_thread->total_wait_time += wait_time;


            if (p == NULL) {
                fprintf(stderr, "ERROR: failed to dequeue IO process\n");
            }

            clock_gettime(CLOCK_MONOTONIC, &ts_turnaround_begin);
            for (i = 0; i < p->numIOBurst; ++i) {
                sleep_time_ms = p->IOBurst[i] * MS_CONVERT;
                usleep(sleep_time_ms);
                main_thread->total_cpu_active_time += (double)sleep_time_ms / 1000.0;
            }
            clock_gettime(CLOCK_MONOTONIC, &ts_turnaround_end);
            turnaround_time_ms = (ts_turnaround_end.tv_sec - ts_turnaround_begin.tv_sec) * MS_CONVERT;
            turnaround_time_ms += (ts_turnaround_end.tv_nsec - ts_turnaround_begin.tv_nsec) / SEC_CONVERT;
            main_thread->total_turnaround_time += turnaround_time_ms;

            /* add back to ready queue  */
            enqueue(main_thread->ready_q, p);
        } else {

            /* calculating total throughput */
            clock_gettime(CLOCK_MONOTONIC, &ts_end);
            throughput_elapsed_time = (ts_end.tv_sec - ts_begin.tv_sec) * MS_CONVERT;
            throughput_elapsed_time += (ts_end.tv_nsec - ts_begin.tv_nsec) / SEC_CONVERT;
            throughput = processes_handled / throughput_elapsed_time;
            main_thread->throughput = ((main_thread->throughput + throughput) / 2);

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
    printf("CPU Utilization                 : %2.3lf%%\n", main_thread->cpu_util / 10);
    printf("Throughput                      : %.3lf processes / ms\n", main_thread->throughput / MS_CONVERT);
    printf("Avg. Turnaround Time            : %2.1lfms\n", main_thread->total_turnaround_time / main_thread->total_num_processes);
    printf("Avg. Waiting Time in R Queue    : %2.1lfms\n", (main_thread->total_wait_time / main_thread->total_num_processes) * 100);
    printf("-------------------------------------------------------\n");
}

int gen_rand_pid() {
    return PID_MIN + rand() % (PID_MAX + 1 - PID_MIN);
}

