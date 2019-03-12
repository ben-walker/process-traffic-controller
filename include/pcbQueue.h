#ifndef PCB_QUEUE_
#define PCB_QUEUE_

typedef struct PCB {
   int pid;
   struct PCB *next;
} PCB;

typedef struct Queue {
   PCB *front, *back;
} Queue;

Queue *newQueue();

PCB *newPCB(int pid);

void enqueue(Queue *q, PCB *p);

PCB *dequeue(Queue *q);

#endif // PCB_QUEUE_
