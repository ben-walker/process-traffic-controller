/**
 * Ben Walker
 * CIS*3110
 * 
 * Library to manage queues and the elements (PCBs) within them.
 */

#include "pcbQ.h"
#include "fatalError.h"
#include <stdio.h>

/**
 * Allocate and initialize a new queue.
 */
Q *newQ() {
   Q *q = malloc(sizeof(Q));
   if (q == NULL) fatal("malloc");
   q->front = q->back = NULL;
   q->length = 0;
   return q;
}

/**
 * Allocate and initialize a new PCB.
 */
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

/**
 * Add a PCB to the back of a queue.
 */
void enQ(Q *q, PCB *p, pcbStates newState, int time) {
   if (p == NULL) return;
   // if there's nothing in the queue, add PCB to the front
   isEmpty(q) ? (q->front = p) : (q->back->next = p);
   q->back = p;
   q->length += 1;
   updateState(p, newState, time);
}

/**
 * Remove PCB from the front of a queue.
 */
PCB *deQ(Q *q) {
   if (isEmpty(q)) return NULL;
   PCB *top = q->front;

   q->front = q->front->next;
   if (q->front == NULL) q->back = NULL; // if resultant queue is empty, set back to null
   q->length -= 1;
   top->next = NULL; // delink PCB from the rest of the queue
   return top;
}

/**
 * Remove PCB from any point in a queue.
 */
PCB *pluck(Q *q, int pid) {
   if (q->front->pid == pid) return deQ(q); // if first PCB is the target, just use dequeue
   PCB *prev, *top = q->front;

   while (top != NULL && top->pid != pid) { // loop until target PCB or end of queue
      prev = top;
      top = top->next;
   }

   if (top == NULL) return NULL; // check for end of queue
   // de-link PCB and return it
   prev->next = top->next;
   top->next = NULL;
   q->length -= 1;
   return top;
}

/**
 * Show a visual representation of the processes in a queue.
 */
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

/**
 * Calculates the running time of process 0 (that is, the amount of time when
 * no other processes were running).
 */
int calcProc0(Q *q) {
   int sysTime = 0, procTime = 0;
   for (PCB *p = q->front; p; p = p->next) {
      procTime += p->runTime; // tally running time of all processes
      // the last (largest) state transition time will be the total system running time
      sysTime = p->stateStartTime > sysTime ? p->stateStartTime : sysTime;
   }
   return sysTime - procTime;
}

/**
 * Prints the running, ready, and blocked time totals for all processes in a queue.
 */
void printQTimings(Q *q) {
   printf("0 %d\n", calcProc0(q));
   for (PCB *p = q->front; p; p = p->next)
      printf("%d %d %d %d\n", p->pid, p->runTime, p->readyTime, p->blockTime);
}

/**
 * Visualize an array of queues, with labels prepended to each array print out.
 */
void printQs(Q *qs[], const char *labels[], size_t size) {
   for (int i = 0; i < size; i += 1) {
      printf("%s: ", labels[i]);
      printQ(qs[i]);
   }
}

/**
 * Returns true if the queue has no processes.
 */
bool isEmpty(Q *q) {
   return q->back == NULL; // the back of the queue is NULL if it's empty
}

/**
 * Returns true if the queue has a process with a given PID.
 */
bool hasProcess(Q *q, int pid) {
   for (PCB *p = q->front; p; p = p->next)
      if (p->pid == pid) return true;
   return false;
}

/**
 * Returns how many processes are in a queue.
 */
int length(Q *q) {
   return q->length;
}

/**
 * Updates the state and time totals of a PCB.
 */
void updateState(PCB *p, pcbStates newState, int time) {
   if (p == NULL) return;
   pcbStates oldState = p->state;
   int oldTime = p->stateStartTime;
   p->state = newState;
   p->stateStartTime = time;

   int timeDiff = time - oldTime; // difference between start of the old state and start of the new state
   switch (oldState) { // add the time difference to the appropriate total based on previous state
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

/**
 * Free the memory associated with a queue and its processes.
 */
void freeQ(Q *q) {
   PCB *p;
   while ((p = deQ(q)))
      free(p);
   free(q);
}

/**
 * Free the memory of an array of queues.
 */
void freeQs(Q *qs[], size_t size) {
   for (int i = 0; i < size; i += 1)
      freeQ(qs[i]);
}

/**
 * Adapted from: https://stackoverflow.com/a/21390410
 * Perform a simple bubble sort on a queue; sorted by PID ascending.
 */
void sortQ(Q *q, PCB **head) {
   if (length(q) <= 1) return; // don't sort empty or length 1 queues
   bool done = false; // set to true when the queue is sorted
   PCB **prev, *curr, *next;

   while (!done) {
      prev = head;
      curr = *head;
      next = (*head)->next;

      done = true; // if we get through the whole loop with no swaps the sort is done
      while (next) { // loop through the entire queue
         if (curr->pid > next->pid) { // swap PCBs if PIDs are out of order
            curr->next = next->next;
            next->next = curr;
            *prev = next;
            done = false; // we had to swap, so sorting is not done
         }
         // iterate pointers
         prev = &curr->next;
         curr = next;
         next = next->next;
      }
   }
   // re-align the pointers to the front and back of the queue
   q->front = *head;
   q->back = curr;
}
