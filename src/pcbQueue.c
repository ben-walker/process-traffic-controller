#include "pcbQueue.h"
#include "fatalError.h"
#include <stdlib.h>
#include <stdio.h>

Queue *newQueue() {
   Queue *q = malloc(sizeof(Queue));
   if (q == NULL) fatal("malloc");
   q->front = q->back = NULL;
   return q;
}

PCB *newPCB(int pid) {
   PCB *p = malloc(sizeof(PCB));
   if (p == NULL) fatal("malloc");
   p->pid = pid;
   p->next = NULL;
   return p;
}

void enqueue(Queue *q, PCB *p) {
   if (q->back == NULL)
      q->front = p;
   else
      q->back->next = p;
   q->back = p;
}

PCB *dequeue(Queue *q) {
   PCB *top = q->front;
   if (top == NULL)
      return NULL;

   q->front = top->next;
   if (q->front == NULL)
      q->back = NULL;
   return top;
}

void printQueue(Queue *q) {
   PCB *p = q->front;
   printf("front <- ");
   while(p){
      printf("pid:%d ", p->pid);
      p = p->next;
   }
   printf("<- back\n");
}
