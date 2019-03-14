#ifndef PCB_QUEUE_
#define PCB_QUEUE_

#include <stdbool.h>
#include <stdlib.h>

typedef enum pcbStates {
   ready,
   running,
   blocked,
   terminated
} pcbStates;

typedef struct PCB {
   int pid;
   struct PCB *next;
   pcbStates state;
   int stateStartTime;
   int readyTime, runTime, blockTime;
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

void printQs(Q *qs[], const char *labels[], size_t size);

bool isEmpty(Q *q);

bool hasProcess(Q *q, int pid);

int length(Q *q);

void updateState(PCB *p, pcbStates newState, int time);

void printQTimings(Q *q);

void sortQ(Q *q, PCB **head);

void freeQ(Q *q);

void freeQs(Q *qs[], size_t size);

#endif // PCB_QUEUE_
