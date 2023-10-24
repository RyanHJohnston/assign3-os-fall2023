#include "proc.h"
#include <stdlib.h>

PCB *new_proc(int PID, int PR, int numCPUBurst, int numIOBurst, 
        int *CPUBurst, int *IOBurst, int cpuindex, int ioindex,
        struct timespec ts_begin, struct timespec ts_end,
        struct PCB *prev, struct PCB *next) {
    
    PCB *temp;
    
    temp = (PCB*)malloc(sizeof(PCB));
    temp->PID = PID;
    temp->PR = PR;
    temp->numCPUBurst = numCPUBurst;
    temp->numIOBurst = numIOBurst;
    temp->CPUBurst = CPUBurst;
    temp->IOBurst = IOBurst;
    temp->cpuindex = cpuindex;
    temp->ioindex = ioindex;
    temp->prev = prev;
    temp->next = next;
    
    return temp;
}




