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
#include <semaphore.h>
#include <unistd.h>

#define MAX_LINE_LEN 256
#define PID_MAX 999
#define PID_MIN 1
#define DELIM " \n\t"
#define MS_CONVERT 1000.0
#define SEC_CONVERT 1000000000.0;

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
    unsigned int duration;
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
    char *alg;
    
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
    char *alg;
    int quant;
    FileRead *input_file;
    CPUScheduler *cpu_sch;
    /* IOSystem *io_sys; */
    ReadyQueue *ready_q;
    IOQueue *io_q;
    /* DoublyLinkedList *ll; */
    sem_t sem_name;
    int total_num_processes;
    double cpu_util;
    double total_cpu_active_time;
    double throughput;
    double total_wait_time;
    double total_turnaround_time;
    double avg_turnaround_time;
    double avg_waiting_ready_q_time;
} MainThread;


void *read_file(void *f);
void *cpu_scheduler(void *arg);
void *io_system(void *arg);
void display_metrics(MainThread *main_thread);

#endif
