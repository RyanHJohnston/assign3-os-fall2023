#include "lib/lib.h"
#include "lib/proc.h"
#include "lib/queue.h"
#include "lib/threads.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;

    /* new_argv = get_args(argc, argv); */

    /* read file */
    /* create process */
    /* add to ready queue */
    int i;
    
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
        main_thread->cpu_sch = (CPUScheduler*)malloc(sizeof(CPUScheduler));        
        init_ready_queue(main_thread->ready_q);
        
        /* create semaphore for following threads  */
        sem_init(&main_thread->sem_name, 0, 0);
        
        
        /* File read thread & cpu sched thread */
        pthread_create(&tid1, NULL, read_file, (void *)main_thread);
        pthread_create(&tid2, NULL, cpu_scheduler, (void *)main_thread);
        
        pthread_join(tid1, (void **)&main_thread);
        pthread_join(tid2, (void **)&main_thread);
        
        sem_destroy(&main_thread->sem_name);
        
        printf("sch thread done\n");

    } else { // RR
        printf("Else was reached\n");
    }
    
    /* free_new_args(argc, new_argv); */
    free(main_thread->input_file);
    free(main_thread);

    
    return 0;
}
