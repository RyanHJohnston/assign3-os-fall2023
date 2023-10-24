#ifndef THREADS_H_
#define THREADS_H_

#include "lib.h"
#include "proc.h"
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * File read thread
 * get the file name, open it, currPID=0
 */
typedef struct FileReadThread {
    char *filename;
    PCB *proc;
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
    /* CPUScheduler *cpuSched; */
    /* IOSystem *ioSystem; */
    /* ReadyQueue *readyQueue; */
    /* IOQueue *ioQueue; */
    /* DoublyLinkedList *linkedList; */
} MainThread;


void *read_file(void *f);



#endif
