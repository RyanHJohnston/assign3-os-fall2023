#include "queue.h"
#include "proc.h"
#include <stdio.h>

void init_ready_queue(ReadyQueue *q) {
    q->front = q->rear = NULL;
}

int is_empty(ReadyQueue *q) {
    return q->front == NULL;
}

void enqueue(ReadyQueue *q, PCB *p) {
    PCB *temp_p;
    
    printf("assign vals reached\n");
    temp_p = (PCB*)malloc(sizeof(PCB));
    temp_p->PID = p->PID;
    temp_p->PR = p->PR;
    temp_p->numCPUBurst = p->numCPUBurst;
    temp_p->numIOBurst = p->numIOBurst;
    temp_p->CPUBurst = p->CPUBurst;
    temp_p->IOBurst = p->IOBurst;
    temp_p->cpuindex = p->cpuindex;
    temp_p->ioindex = p->ioindex;
    temp_p->ts_begin = p->ts_begin;
    temp_p->ts_end = p->ts_end;
    temp_p->next = temp_p->next;
    temp_p->prev = temp_p->prev;

    if (q->rear == NULL) {
        q->front = q->rear = temp_p;
        return;
    }

    q->rear->next = temp_p;
    q->rear = temp_p;
}

void dequeue(ReadyQueue *q, PCB *p) {
    if (is_empty(q)) {
        fprintf(stderr, "ERROR: Queue is empty\n");
        return;
    }

    PCB *temp;

    temp = q->front;
    while (temp != NULL) {
        temp = temp->next;
    }
    printf("NULL\n");
}

void displayQueue(ReadyQueue *q) {
    if (is_empty(q)) {
        fprintf(stderr, "Ready Queue is empty\n");
        return;
    }

    PCB *temp;

    temp = q->front;
    printf("PIDs: ");
    while (temp != NULL) {
        printf("%i -> ", temp->PID);
        temp = temp->next;
    }
    printf("\nEnd of queue\n");
}


void init_io_queue(IOQueue *q) {
    q->front = q->rear = NULL;
}

int io_is_empty(IOQueue *q) {
    return q->front == NULL;
}

void io_enqueue(IOQueue *q, PCB *p) {
    PCB *temp_p;

    if (q->rear == NULL) {
        q->front = q->rear = temp_p;
        return;
    }

    q->rear->next = temp_p;
    q->rear = temp_p;
}

void io_dequeue(IOQueue *q, PCB *p) {
    if (io_is_empty(q)) {
        fprintf(stderr, "ERROR: Queue is empty\n");
        return;
    }

    PCB *temp;

    temp = q->front;
    while (temp != NULL) {
        temp = temp->next;
    }
    printf("NULL\n");
}

void io_displayQueue(IOQueue *q) {
    if (io_is_empty(q)) {
        fprintf(stderr, "Ready Queue is empty\n");
        return;
    }

    PCB *temp;

    temp = q->front;
    printf("PIDs: ");
    while (temp != NULL) {
        printf("%i -> ", temp->PID);
        temp = temp->next;
    }
    printf("\nEnd of queue\n");
}





