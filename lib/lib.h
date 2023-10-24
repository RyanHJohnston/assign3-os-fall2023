#ifndef LIB_H_
#define LIB_H_

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

/* #include "threads.h" */
/* #include "queue.h" */
/* #include "proc.h" */

#define SUCCESS 0
#define FAILURE 1

/* NOTE: Each of the three threads in the MainThread will process each item in the data structures
 * ex. FileReadThread will process an item in ReadyQueue and repeat until it's empty
 * this goes for CPUSchedulerThread and IOSystemThread
 * They all have to go inside of MainThread
 * The algorithm that is chosen will decide which items will be selected in the Queues
 * VERY IMPORTANT: Each 'item' is a process that is represented by the PCB
 * MainThread will be designated an algorithm
 * Then the 3 nested threads will find out what algorithm needs to be used (if FIFO else if RR)
 * 
 * FileReadThread - reads the input.txt, creates a PCB from each line in input.txt,
 *                             inserts each PCB into ReadyQueue. The CPUSchedulerThread will then
 *                             read from the ReadyQueue.
 *
 * CPUSchedulerThread - depending on the algorithm used, it will select a PCB from
 *                      the ReadyQueue (then removing the item from the queue) depending on
 *                      the priority of the PCB and the algorithm used. It will then sleep
 *                      for PCB->CPUBurst[i] miliseconds to simulate code being executed on the CPU.
 *                      After it is done with the current PCB, it will insert into the IOQueue.
 *                      IOSystemThread will then read from the IOQueue for the PCB to be processed.
 * 
 * IOSystemThread - 
 *  
 *
 *
 *
 * Here's how to read the input.txt with FIFO:
 * proc 1 7 10 20 10 50 20 40 10
 * 
 * proc - keyword
 * 1 - the priority of the process
 * 7 - the amount of numbers that exist after
 * Then all of the numbers afterwards will be assigned attributes in the PCB block
 * ex.
 * 
 * PCB {
 *   numCPUBurst = 4 (it's just # of numbers divided by 2+1, so it's 7/4 in this case)
 *   numIOBurst = 3 (it's just # of numbers divided by 2, so it's 7/2 in this case)
 *   CPUBurst = 10, 10, 20, 10 (pattern is CPUBUrst, IOBurst, CPUBurst, IOBurst, etc)
 *   IOBurst = 20, 50, 40
 *   cpuindex = 0
 *   ioindex = 0
 *   ...
 * }
 *
 * Once this PCB struct is created, insert it into the ReadyQueue
 * Each line in the input.txt file is its own PCB that will be inserted into the ReadyQueue
 * You can write a command that will have the thread sleep for 50ms then wake up and continue with
 * the next PCB.
 * Once all PCB are processed, break the while loop and exit the thread
 */


/* ----------- BASIC FUNCTIONS ---------------- */

void print_array(int *array, int length);
int is_valid(int arg, char **argk);
static int is_quantum(int length, char **args);
static int is_int(char *str);
char **get_args(int length, char **args);
char *get_element_from_new_args(int length, char **new_args, int element_index);
void print_new_args(int length, char **new_args);
void free_new_args(int length, char **args);



#endif // LIB_H_
