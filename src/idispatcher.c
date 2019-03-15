/**
 * Ben Walker
 * CIS*3110
 * 
 * Library to interpret events and allocate processes to the
 * correct queues.
 */

#include "idispatcher.h"
#include "parser.h"
#include "pcbQ.h"
#include "eventTranslator.h"
#include "queues.h"

/**
 * Create a new process and add it to the appropriate queue.
 */
void newProcess(int pid, int time, Q *qs[]) {
   if (pid < 1) return; // pid 0 is reserved; negatives not allowed
   isEmpty(qs[runQ]) // add process to the run queue if nothing is running, ready queue otherwise
      ? enQ(qs[runQ], newPCB(pid, time, running), running, time)
      : enQ(qs[readyQ], newPCB(pid, time, ready), ready, time);
}

/**
 * Preempt the running process with the next ready process.
 */
void timeInterrupt(int time, Q *qs[]) {
   enQ(qs[readyQ], deQ(qs[runQ]), ready, time); // enqueue in the ready queue what is dequeued from the run queue
   enQ(qs[runQ], deQ(qs[readyQ]), running, time);
}


/**
 * Move a process to the requested resource queue.
 */
void requestResource(int pid, int res, int time, Q *qs[]) {
   PCB *pcb;
   if (hasProcess(qs[runQ], pid)) { // run a new process if currently running process requests a resource
      pcb = pluck(qs[runQ], pid);
      enQ(qs[runQ], deQ(qs[readyQ]), running, time);
   } else if (hasProcess(qs[readyQ], pid))
      pcb = pluck(qs[readyQ], pid);
   else // only processes in ready/run queues can request a resource
      return;
   enQ(qs[resourceToQIndex(res)], pcb, blocked, time);
}

/**
 * Move a process from a resource queue to the ready/run queue.
 */
void resourceInterrupt(int pid, int res, int time, Q *qs[]) {
   int resourceQ = resourceToQIndex(res); // determine index of resource queue
   PCB *pcb = pluck(qs[resourceQ], pid); // "pluck" the process from the resource queue
   isEmpty(qs[runQ]) // add process to the run queue if nothing is running, ready queue otherwise
      ? enQ(qs[runQ], pcb, running, time)
      : enQ(qs[readyQ], pcb, ready, time);
}

/**
 * Move a process from any "living" queue to the "dead" queue (i.e. terminate a process).
 */
void removeProcess(int pid, int time, Q *qs[]) {
   for (int i = runQ; i < res5Q; i += 1) { // loop through all queues
      if (!hasProcess(qs[i], pid)) continue; // skip if process not in this queue
      enQ(qs[deadQ], pluck(qs[i], pid), terminated, time); // add process to the dead queue
      if (i == runQ) enQ(qs[runQ], deQ(qs[readyQ]), running, time); // start a new process if we plucked from the run queue
   }
}

/**
 * Perform the correct action on a process based on event type;
 * i.e. dispatch a process.
 */
void dispatch(char **event, Q *qs[]) {
   int time = getEventTime(event);

   switch (getEventType(event)) {
      case createProc:
         newProcess(getLifespanEventPID(event), time, qs);
         break;

      case exitProc:
         removeProcess(getLifespanEventPID(event), time, qs);
         break;

      case reqRes:
         requestResource(getResourceEventPID(event), getResource(event), time, qs);
         break;

      case interruptProc:
         resourceInterrupt(getResourceEventPID(event), getResource(event), time, qs);
         break;

      case timerInterrupt:
         timeInterrupt(time, qs);
         break;

      default:
         break;
   }
}

/**
 * Free memory associated with array of event strings.
 */
void freeEvent(char **event) {
   for (int i = 0; event[i]; i += 1)
      free(event[i]);
   free(event);
}

/**
 * Loop through all events (until blank line), dispatch processes based on these events.
 */
void startDispatching() {
   // setup enough queues (see queues.h)
   Q *qs[NUM_QUEUES];
   for (int i = 0; i < NUM_QUEUES; i += 1)
      qs[i] = newQ();
   char **event;

   while ((event = parseLine())) {
      dispatch(event, qs);
      freeEvent(event);
   }
   sortQ(qs[deadQ], &qs[deadQ]->front); // sort dead processes by PID ascending
   printQTimings(qs[deadQ]); // print out timing totals (run, ready, blocked) for dead processes
   freeQs(qs, NUM_QUEUES); // free queue memory
}
