#include "pcbQ.h"
#include "fatalError.h"
#include <stdlib.h>
#include <stdio.h>

Q *newQ() {
   Q *q = malloc(sizeof(Q));
   if (q == NULL) fatal("malloc");
   q->front = q->back = NULL;
   return q;
}

PCB *newPCB(int pid, int time, pcbStates state) {
   PCB *p = malloc(sizeof(PCB));
   if (p == NULL) fatal("malloc");
   p->pid = pid;
   p->state = state;
   p->stateStartTime = time;
   p->next = NULL;
   return p;
}

void enQ(Q *q, PCB *p) {
   if (isEmpty(q))
      q->front = q->back = p;
   else {
      q->back->next = p;
      q->back = p;
   }
   q->length += 1;
}

PCB *deQ(Q *q) {
   PCB *top = q->front;
   q->front = q->front->next;
   if (q->front == NULL)
      q->back = NULL;
   q->length -= 1;
   top->next = NULL;
   return top;
}

void printQ(Q *q) {
   printf("front <- ");
   for (PCB *p = q->front; p; p = p->next)
      printf("pid:%d:%d:%d ", p->pid, p->state, p->stateStartTime);
   printf("<- back\n");
}

bool isEmpty(Q *q) {
   return q->back == NULL;
}

int length(Q *q) {
   return q->length;
}
