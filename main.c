#include "lib/lib.h"
#include "lib/proc.h"
#include "lib/queue.h"
#include "lib/threads.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* -------- DIRECTIONS ----------------*/
/*
 * input: prog -alg [FIFO|SJF|PR|RR] [-quantum [integer (ms)]] -input [file name]
 * output:   Input File Name              : input file name

     CPU Scheduling Alg           : FIFO|SJF|PR|RR (quantum)

     CPU utilization              : ....

     Throughput                   : ....

     Avg. Turnaround time         : ....

     Avg. Waiting time in R queue : ....
*/



int main(int argc, char *argv[]) {
    
    if (is_valid(argc, argv) == 1) {
        return 1;
    }

    
    char **new_argv;
    MainThread *main_thread;
    
    struct timespec ts_cpu_begin;
    struct timespec ts_cpu_end;
    double total_observation_time;
    
    int i;
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;

    /* begin program cpu measuring */
    clock_gettime(CLOCK_MONOTONIC, &ts_cpu_begin);
    main_thread = (MainThread*)malloc(sizeof(MainThread));
    
    /* not RR */
    if (argc == 5) {
        main_thread->input_file = (FileRead*)malloc(sizeof(FileRead));
        if (main_thread->input_file == NULL) {
            fprintf(stderr, "ERROR: main_thread->input_file is NULL\n");
            exit(EXIT_FAILURE);
        }

        main_thread->alg = argv[2];
        main_thread->quant = 0;
        main_thread->input_file->filename = argv[4]; 
        main_thread->ready_q = (ReadyQueue*)malloc(sizeof(ReadyQueue));
        main_thread->io_q = (IOQueue*)malloc(sizeof(IOQueue));
        main_thread->cpu_sch = (CPUScheduler*)malloc(sizeof(CPUScheduler));
        main_thread->total_num_processes = 0;
        main_thread->cpu_util = 0.0;
        main_thread->total_cpu_active_time = 0.0;
        main_thread->throughput = 0.0;
        main_thread->total_wait_time = 0.0;
        main_thread->total_turnaround_time = 0.0;
        main_thread->avg_turnaround_time = 0.0;
        main_thread->avg_waiting_ready_q_time = 0.0;
        init_ready_queue(main_thread->ready_q);
        init_io_queue(main_thread->io_q);

        /* create semaphore for following threads  */
        sem_init(&main_thread->sem_name, 0, 0);
        
        /* File read, cpu scheduler, and io system threads */
        pthread_create(&tid1, NULL, read_file, (void *)main_thread);
        pthread_join(tid1, (void **)&main_thread);
        
        pthread_create(&tid2, NULL, cpu_scheduler, (void *)main_thread);
        pthread_join(tid2, (void **)&main_thread);

        pthread_create(&tid3, NULL, io_system, (void *)main_thread);
        pthread_join(tid3, (void **)&main_thread);

        sem_destroy(&main_thread->sem_name);
       
        /* get cpu usage */
        clock_gettime(CLOCK_MONOTONIC, &ts_cpu_end);
        total_observation_time = (ts_cpu_end.tv_sec - ts_cpu_begin.tv_sec) +
            (ts_cpu_end.tv_nsec - ts_cpu_begin.tv_nsec) / 1e9;
        main_thread->cpu_util = (main_thread->total_cpu_active_time / total_observation_time);

        /* display metrics  */
        display_metrics(main_thread);


    } else { // RR
        printf("Else was reached\n");
    }


    printf("\nEND OF PROGRAM\n");
    /* free_new_args(argc, new_argv); */
    free(main_thread->input_file);
    free(main_thread->ready_q);
    free(main_thread->io_q);
    free(main_thread);

    
    return 0;
}
