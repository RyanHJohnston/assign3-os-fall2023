#include "lib/lib.h"
#include "lib/proc.h"
#include "lib/queue.h"
#include "lib/threads.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
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
        main_thread->inputFile = (FileRead*)malloc(sizeof(FileRead));
        if (main_thread->inputFile == NULL) {
            fprintf(stderr, "ERROR: main_thread->inputFile is NULL\n");
            exit(EXIT_FAILURE);
        }

        main_thread->algorithm = argv[2];
        main_thread->quantum = 0;
        main_thread->inputFile->filename = argv[4]; 
        main_thread->inputFile->ready_q = (ReadyQueue*)malloc(sizeof(ReadyQueue));
        init_ready_queue(main_thread->inputFile->ready_q);
        
        pthread_create(&tid1, NULL, read_file, (void *)main_thread->inputFile);
        pthread_join(tid1, (void **)&main_thread->inputFile);
        

    } else { // RR
        printf("Else was reached\n");
    }
    
    /* free_new_args(argc, new_argv); */
    free(main_thread->inputFile);
    free(main_thread);

    
    return 0;
}
