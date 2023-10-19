#ifndef PROC_H_
#define PROC_H_

#include <time.h>

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


#endif
