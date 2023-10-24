#ifndef PROC_H_
#define PROC_H_

#include <time.h>
#include "lib.h"

typedef struct PCB {
    int PID;
    int PR;
    int numCPUBurst;
    int numIOBurst;

    /* create dynamic arrays to store cpu burst times */
    int *CPUBurst;
    int *IOBurst;
    
    int cpuindex;
    int ioindex;
    
    struct timespec ts_begin;
    struct timespec ts_end;

    struct PCB *prev;
    struct PCB *next;
    
    /* more fields for performance measures */
    /* use the system time to determine how much waited etc */
} PCB;

PCB *new_proc(int PID, int PR, int numCPUBurst, int numIOBurst, 
        int *CPUBurst, int *IOBurst, int cpuindex, int ioindex,
        struct timespec ts_begin, struct timespec ts_end,
        struct PCB *prev, struct PCB *next);

/* generates a random PID 1 - 1000 */
int gen_rand_pid();

#endif
