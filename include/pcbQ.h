#ifndef PCB_QUEUE_
#define PCB_QUEUE_

#include <stdbool.h>

typedef enum pcbStates {
   ready,
   running
} pcbStates;

typedef struct PCB {
   int pid;
   struct PCB *next;
   pcbStates state;
   int stateStartTime;
   int runTime;
} PCB;

typedef struct Q {
   int length;
   PCB *front, *back;
} Q;

Q *newQ();

PCB *newPCB(int pid, int time, pcbStates state);

void enQ(Q *q, PCB *p);

PCB *deQ(Q *q);

PCB *pluck(Q *q, int pid);

void printQ(Q *q);

bool isEmpty(Q *q);

bool hasProcess(Q *q, int pid);

int length(Q *q);

#endif // PCB_QUEUE_
