#ifndef PCB_QUEUE_
#define PCB_QUEUE_

#include <stdbool.h>

typedef struct PCB {
   int pid;
   struct PCB *next;
   int stateChangeTime;
} PCB;

typedef struct Q {
   PCB *front, *back;
} Q;

Q *newQ();

PCB *newPCB(int pid);

void enQ(Q *q, PCB *p);

PCB *deQ(Q *q);

void printQ(Q *q);

bool isEmpty(Q *q);

int length(Q *q);

#endif // PCB_QUEUE_
