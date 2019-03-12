#ifndef PCB_QUEUE_
#define PCB_QUEUE_

typedef enum pcbState {
   ready,
   running,
   blocked
} pcbState;

typedef struct PCB {
   int pid;
   pcbState state;
   struct PCB *next;
} PCB;

typedef struct Queue {
   PCB *front, *back;
} Queue;

Queue *newQueue();

PCB *newPCB(int pid);

void enqueue(Queue *q, PCB *p);

PCB *dequeue(Queue *q);

void printQueue(Queue *q);

#endif // PCB_QUEUE_
