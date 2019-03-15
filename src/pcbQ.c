#include "pcbQ.h"
#include "fatalError.h"
#include <stdio.h>

Q *newQ() {
   Q *q = malloc(sizeof(Q));
   if (q == NULL) fatal("malloc");
   q->front = q->back = NULL;
   q->length = 0;
   return q;
}

PCB *newPCB(int pid, int time, pcbStates state) {
   PCB *p = malloc(sizeof(PCB));
   if (p == NULL) fatal("malloc");
   p->pid = pid;
   p->next = NULL;
   p->state = state;
   p->stateStartTime = time;
   p->readyTime = p->runTime = p->blockTime = 0;
   return p;
}

void enQ(Q *q, PCB *p, pcbStates newState, int time) {
   if (p == NULL) return;
   isEmpty(q) ? (q->front = p) : (q->back->next = p);
   q->back = p;
   q->length += 1;
   updateState(p, newState, time);
}

PCB *deQ(Q *q) {
   if (isEmpty(q)) return NULL;
   PCB *top = q->front;

   q->front = q->front->next;
   if (q->front == NULL) q->back = NULL;
   q->length -= 1;
   top->next = NULL;
   return top;
}

PCB *pluck(Q *q, int pid) {
   if (q->front->pid == pid) return deQ(q);
   PCB *prev, *top = q->front;

   while (top != NULL && top->pid != pid) {
      prev = top;
      top = top->next;
   }

   if (top == NULL) return NULL;
   prev->next = top->next;
   top->next = NULL;
   q->length -= 1;
   return top;
}

void printQ(Q *q) {
   if (isEmpty(q)) {
      printf("(empty)\n");
      return;
   }
   printf("front <- ");
   for (PCB *p = q->front; p; p = p->next)
      printf("[%d] ", p->pid);
   printf("<- back\n");
}

int calcProc0(Q *q) {
   int sysTime = 0, procTime = 0;
   for (PCB *p = q->front; p; p = p->next) {
      procTime += p->runTime;
      sysTime = p->stateStartTime > sysTime ? p->stateStartTime : sysTime;
   }
   return sysTime - procTime;
}

void printQTimings(Q *q) {
   printf("0 %d\n", calcProc0(q));
   for (PCB *p = q->front; p; p = p->next)
      printf("%d %d %d %d\n", p->pid, p->runTime, p->readyTime, p->blockTime);
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
   for (PCB *p = q->front; p; p = p->next)
      if (p->pid == pid) return true;
   return false;
}

int length(Q *q) {
   return q->length;
}

void updateState(PCB *p, pcbStates newState, int time) {
   if (p == NULL) return;
   pcbStates oldState = p->state;
   int oldTime = p->stateStartTime;
   p->state = newState;
   p->stateStartTime = time;

   int timeDiff = time - oldTime;
   switch (oldState) {
      case ready:
         p->readyTime += timeDiff;
         break;

      case running:
         p->runTime += timeDiff;
         break;

      case blocked:
         p->blockTime += timeDiff;
         break;

      case terminated:
         break;
   }
}

void freeQ(Q *q) {
   PCB *p;
   while ((p = deQ(q)))
      free(p);
   free(q);
}

void freeQs(Q *qs[], size_t size) {
   for (int i = 0; i < size; i += 1)
      freeQ(qs[i]);
}

// Adapted from: https://stackoverflow.com/a/21390410
void sortQ(Q *q, PCB **head) {
   if (length(q) <= 1) return;
   bool done = false;
   PCB **prev, *curr, *next;

   while (!done) {
      prev = head;
      curr = *head;
      next = (*head)->next;

      done = true;
      while (next) {
         if (curr->pid > next->pid) {
            curr->next = next->next;
            next->next = curr;
            *prev = next;
            done = false;
         }
         prev = &curr->next;
         curr = next;
         next = next->next;
      }
   }
   q->front = *head;
   q->back = curr;
}
