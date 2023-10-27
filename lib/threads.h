#ifndef THREADS_H_
#define THREADS_H_

#include "lib.h"
#include "proc.h"
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 256
#define PID_MAX 999
#define PID_MIN 1
#define DELIM " \n\t"


typedef enum LineType{
    TYPE_PROC,
    TYPE_SLEEP,
    TYPE_STOP
} LineType;

typedef struct ProcessData{
    int *values;
    int count;  // The number of integers read.
} ProcessData;

typedef struct SleepData{
    int duration;
} SleepData;

typedef union LineData{
    ProcessData processData;
    SleepData sleepData;
} LineData;

typedef struct Line {
    LineType type;
    LineData data;
} Line;

/*
 * File read thread
 * get the file name, open it, currPID=0
 */
typedef struct FileReadThread {
    char *filename;
    PCB *proc;
    /* ReadyQueue *ready_q; */
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
    ReadyQueue *ready_q;
    /* CPUScheduler *cpuSched; */
    /* IOSystem *ioSystem; */
    /* ReadyQueue *readyQueue; */
    /* IOQueue *ioQueue; */
    /* DoublyLinkedList *linkedList; */
} MainThread;


void *read_file(void *f);



#endif
