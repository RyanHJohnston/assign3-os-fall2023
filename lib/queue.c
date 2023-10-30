#include "queue.h"
#include "proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

PCB *dequeue(ReadyQueue *q) {
    if (is_empty(q) == 1) {
        fprintf(stderr, "ERROR: Ready queue is empty when calling dequeue\n");
        return NULL;
    }

    PCB *temp = (PCB*)malloc(sizeof(PCB));
    if (temp == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate memory for temp in dequeue\n");
        return NULL;
    }

    // Deep copy from the front PCB to temp
    *temp = *q->front;  // This will copy all fields shallowly

    temp->CPUBurst = malloc(sizeof(int) * q->front->numCPUBurst);
    temp->IOBurst = malloc(sizeof(int) * q->front->numIOBurst);

    if (temp->CPUBurst == NULL || temp->IOBurst == NULL) {
        fprintf(stderr, "ERROR: memory was not properly allocated to arrays in the dequeue\n");
        free(temp->CPUBurst);  // It's safe to free a NULL pointer, so no need to check
        free(temp->IOBurst);
        free(temp);
        return NULL;
    }

    memcpy(temp->CPUBurst, q->front->CPUBurst, sizeof(int) * q->front->numCPUBurst);
    memcpy(temp->IOBurst, q->front->IOBurst, sizeof(int) * q->front->numIOBurst);

    // Remove the front PCB from the ReadyQueue and free its memory
    PCB *toFree = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    } else {
        q->front->prev = NULL;  // Set the prev pointer of the new front to NULL
    }

    free(toFree->CPUBurst);
    free(toFree->IOBurst);
    free(toFree);

    // Clear out next and prev pointers from the copied PCB
    temp->next = NULL;
    temp->prev = NULL;

    return temp;

}

void display_queue(ReadyQueue *q) {
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

void io_display_queue(IOQueue *q) {
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





