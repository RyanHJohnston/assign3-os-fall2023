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
    PCB *temp;

    temp = p;

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
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
    PCB *temp;

    temp = p;

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
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





