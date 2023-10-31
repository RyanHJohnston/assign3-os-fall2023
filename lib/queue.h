#ifndef QUEUE_H_
#define QUEUE_H_

#include "proc.h"
#include "lib.h"

typedef struct ReadyQueue {
    PCB *front;
    PCB *rear;
} ReadyQueue;

typedef struct IOQueue {
    PCB *front;
    PCB *rear;
} IOQueue;

typedef struct DoublyLinkedList {
    PCB *proc;
    struct DoublyLinkedList *next;
} DoublyLinkedList;

/* ready queue functions */
void init_ready_queue(ReadyQueue *q);
int is_empty(ReadyQueue *q);
void enqueue(ReadyQueue *q, PCB *p);
PCB *dequeue(ReadyQueue *q);
void display_queue(ReadyQueue *q);

/* io queue functions */
void init_io_queue(IOQueue *q);
int io_is_empty(IOQueue *q);
void io_enqueue(IOQueue *q, PCB *p);
PCB *io_dequeue(IOQueue *q);
void io_display_queue(IOQueue *q);

/* doubly linked list functions */


/* algorithms */
PCB* find_shortest_job(ReadyQueue *ready_q);
PCB* find_highest_priority_job(ReadyQueue *ready_q);
void remove_from_queue(ReadyQueue *q, PCB *p);

#endif
