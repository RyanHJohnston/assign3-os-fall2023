#ifndef QUEUE_H_
#define QUEUE_H_

#include "proc.h"

typedef struct ReadyQueue {
    PCB *front;
    PCB *rear;
} ReadyQueue;

typedef struct IOQueue {
    PCB *frontProc;
    PCB *rearProc;
} IOQueue;

typedef struct DoublyLinkedList {
    PCB *proc;
    struct DoublyLinkedList *next;
} DoublyLinkedList;


#endif
