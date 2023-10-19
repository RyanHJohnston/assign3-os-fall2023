#ifndef THREADS_H_
#define THREADS_H_

#include "proc.h"
#include "queue.h"

/*
 * File read thread
 * get the file name, open it, currPID=0
 */
typedef struct FileReadThread {
    char *filename;
    
} FileRead;


typedef struct CPUSchedulerThread {

} CPUScheduler;

/*
 * This will only use FIFO
 */
typedef struct IOSystemThread {
    
} IOSystem;


/* 
 * Process cli args and get the simulation parameters (e.g. ALG, QUANTUM, inputfile)
 * create/init the necessary data structures (Ready_Q and IO_Q (double linked lists of PCB))
 * Create/start the following three threads with appropriate parameters
 * wait until all threads are done
 * print performance metrics
 */
typedef struct MainThread {
    char *algorithm;
    int quantum;
    FileRead *inputFile;
    ReadyQueue *readyQueue;
    IOQueue *ioQueue;
    DoublyLinkedList *linkedList;
} Main;




#endif
