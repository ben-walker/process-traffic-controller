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
   int runTime, readyTime, blockTime;
} PCB;

typedef struct Q {
   PCB *front, *back;
} Q;

Q *newQ();

PCB *newPCB(int pid);

void enQ(Q *q, PCB *p);

PCB *deQ(Q *q);

void printQ(Q *q);

#endif // PCB_QUEUE_
