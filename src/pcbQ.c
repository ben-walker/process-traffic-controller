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
   PCB *pcb = malloc(sizeof(PCB));
   if (pcb == NULL) fatal("malloc");
   pcb->pid = pid;
   pcb->next = NULL;
   pcb->state = state;
   pcb->stateStartTime = time;
   pcb->readyTime = pcb->runTime = pcb->blockTime = 0;
   return pcb;
}

/**
 * Add a PCB to the back of a queue.
 */
void enQ(Q *q, PCB *pcb, pcbStates newState, int time) {
   if (pcb == NULL) return;
   // if there's nothing in the queue, add PCB to the front
   isEmpty(q) ? (q->front = pcb) : (q->back->next = pcb);
   q->back = pcb;
   q->length += 1;
   updateState(pcb, newState, time);
}

/**
 * Remove PCB from the front of a queue.
 */
PCB *deQ(Q *q) {
   if (isEmpty(q)) return NULL;
   PCB *pcb = q->front;

   q->front = q->front->next;
   if (q->front == NULL) q->back = NULL; // if resultant queue is empty, set back to null
   q->length -= 1;
   pcb->next = NULL; // delink PCB from the rest of the queue
   return pcb;
}

/**
 * Remove PCB from any point in a queue.
 */
PCB *pluck(Q *q, int pid) {
   if (q->front->pid == pid) return deQ(q); // if first PCB is the target, just use dequeue
   PCB *prev, *pcb = q->front;

   while (pcb != NULL && pcb->pid != pid) { // loop until target PCB or end of queue
      prev = pcb;
      pcb = pcb->next;
   }

   if (pcb == NULL) return NULL; // check for end of queue
   // de-link PCB and return it
   prev->next = pcb->next;
   pcb->next = NULL;
   q->length -= 1;
   return pcb;
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
   for (PCB *pcb = q->front; pcb; pcb = pcb->next)
      printf("[%d] ", pcb->pid);
   printf("<- back\n");
}

/**
 * Calculates the running time of process 0 (that is, the amount of time when
 * no other processes were running).
 */
int calcProc0(Q *q) {
   int sysTime = 0, procTime = 0;
   for (PCB *pcb = q->front; pcb; pcb = pcb->next) {
      procTime += pcb->runTime; // tally running time of all processes
      // the last (largest) state transition time will be the total system running time
      sysTime = pcb->stateStartTime > sysTime ? pcb->stateStartTime : sysTime;
   }
   return sysTime - procTime;
}

/**
 * Prints the running, ready, and blocked time totals for all processes in a queue.
 */
void printQTimings(Q *q) {
   printf("0 %d\n", calcProc0(q));
   for (PCB *pcb = q->front; pcb; pcb = pcb->next)
      printf("%d %d %d %d\n", pcb->pid, pcb->runTime, pcb->readyTime, pcb->blockTime);
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
   for (PCB *pcb = q->front; pcb; pcb = pcb->next)
      if (pcb->pid == pid) return true;
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
void updateState(PCB *pcb, pcbStates newState, int newTime) {
   if (pcb == NULL) return;
   pcbStates oldState = pcb->state;
   int oldTime = pcb->stateStartTime;
   pcb->state = newState;
   pcb->stateStartTime = newTime;

   int timeDiff = newTime - oldTime; // difference between start of the old state and start of the new state
   switch (oldState) { // add the time difference to the appropriate total based on previous state
      case ready:
         pcb->readyTime += timeDiff;
         break;

      case running:
         pcb->runTime += timeDiff;
         break;

      case blocked:
         pcb->blockTime += timeDiff;
         break;

      case terminated:
         break;
   }
}

/**
 * Free the memory associated with a queue and its processes.
 */
void freeQ(Q *q) {
   PCB *pcb;
   while ((pcb = deQ(q)))
      free(pcb);
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
