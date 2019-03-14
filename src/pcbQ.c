#include "pcbQ.h"
#include "fatalError.h"
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

PCB *pluck(Q *q, int pid) {
   if (q->front->pid == pid)
      return deQ(q);

   PCB *prev, *top = q->front;
   while (top != NULL && top->pid != pid) {
      prev = top;
      top = top->next;
   }
   if (top == NULL)
      return NULL;
   prev->next = top->next;
   top->next = NULL;
   q->length -= 1;
   return top;
}

void printQ(Q *q) {
   printf("front <- ");
   for (PCB *p = q->front; p; p = p->next)
      printf("pid:%d:%d:%d ", p->pid, p->state, p->stateStartTime);
   printf("<- back\n");
}

void printQs(Q *qs[], const char *labels[], size_t size) {
   for (int i = 0; i < size; i += 1) {
      printf("%s: ", labels[i]);
      printQ(qs[i]);
   }
}

bool isEmpty(Q *q) {
   return q->back == NULL;
}

bool hasProcess(Q *q, int pid) {
   for (PCB *p = q->front; p; p = p->next) {
      if (p->pid == pid)
         return true;
   }
   return false;
}

int length(Q *q) {
   return q->length;
}
