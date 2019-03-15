#include "idispatcher.h"
#include "parser.h"
#include "pcbQ.h"
#include "eventTranslator.h"
#include "queues.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void newProcess(int pid, int time, Q *qs[]) {
   if (pid < 1) return;
   isEmpty(qs[runQ])
      ? enQ(qs[runQ], newPCB(pid, time, running), running, time)
      : enQ(qs[readyQ], newPCB(pid, time, ready), ready, time);
}

void timeInterrupt(int time, Q *qs[]) {
   enQ(qs[readyQ], deQ(qs[runQ]), ready, time);
   enQ(qs[runQ], deQ(qs[readyQ]), running, time);
}

void requestResource(int pid, int res, int time, Q *qs[]) {
   PCB *p;
   if (hasProcess(qs[runQ], pid)) {
      p = pluck(qs[runQ], pid);
      enQ(qs[runQ], deQ(qs[readyQ]), running, time);
   } else if (hasProcess(qs[readyQ], pid))
      p = pluck(qs[readyQ], pid);
   else
      return;
   enQ(qs[resourceToQIndex(res)], p, blocked, time);
}

void resourceInterrupt(int pid, int res, int time, Q *qs[]) {
   int resQ = resourceToQIndex(res);
   PCB *p = pluck(qs[resQ], pid);
   isEmpty(qs[runQ])
      ? enQ(qs[runQ], p, running, time)
      : enQ(qs[readyQ], p, ready, time);
}

void removeProcess(int pid, int time, Q *qs[]) {
   for (int i = runQ; i < res5Q; i += 1) {
      if (!hasProcess(qs[i], pid)) continue;
      enQ(qs[deadQ], pluck(qs[i], pid), terminated, time);
      if (i == runQ) enQ(qs[runQ], deQ(qs[readyQ]), running, time);
   }
}

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

void freeEvent(char **event) {
   for (int i = 0; event[i]; i += 1)
      free(event[i]);
   free(event);
}

void startDispatching() {
   Q *qs[] = { newQ(), newQ(), newQ(), newQ(), newQ(), newQ(), newQ(), newQ() };
   char **event;

   while ((event = parseLine())) {
      dispatch(event, qs);
      freeEvent(event);
   }
   sortQ(qs[deadQ], &qs[deadQ]->front);
   printQTimings(qs[deadQ]);
   freeQs(qs, sizeof(qs) / sizeof(qs[0]));
}
