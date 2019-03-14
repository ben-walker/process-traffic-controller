#include "idispatcher.h"
#include "parser.h"
#include "pcbQ.h"
#include "eventTranslator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum queues {
   runQ,
   readyQ,
   res1Q,
   res2Q,
   res3Q,
   res4Q,
   res5Q
} queues;

int resNumToQIdx(int res) {
   switch (res) {
      case 1: return res1Q;
      case 2: return res2Q;
      case 3: return res3Q;
      case 4: return res4Q;
      case 5: return res5Q;
   }
   return -1;
}

void newProcess(int pid, int time, Q *qs[]) {
   if (pid < 1)
      return;
   if (isEmpty(qs[runQ]))
      enQ(qs[runQ], newPCB(pid, time, running));
   else
      enQ(qs[readyQ], newPCB(pid, time, ready));
}

void timeInterrupt(int time, Q *qs[]) {
   enQ(qs[readyQ], deQ(qs[runQ]));
   enQ(qs[runQ], deQ(qs[readyQ]));
}

void requestResource(int pid, int res, Q *qs[]) {
   PCB *p = hasProcess(qs[runQ], pid)
      ? pluck(qs[runQ], pid)
      : pluck(qs[readyQ], pid);
   if (p != NULL)
      enQ(qs[resNumToQIdx(res)], p);
}

void resourceInterrupt(int pid, int res, Q *qs[]) {
   int resQ = resNumToQIdx(res);
   PCB *p = pluck(qs[resQ], pid);
   if (p != NULL)
      enQ(qs[readyQ], p);
}

void removeProcess(int pid, Q *qs[]) {
   PCB *p;
   for (int i = runQ; i < res5Q; i += 1)
      if (hasProcess(qs[i], pid))
         p = pluck(qs[i], pid);
}

void dispatch(char **event, Q *qs[]) {
   int time = getEventTime(event);

   switch (getEventType(event)) {
      case createProc:
         newProcess(getLifespanEventPID(event), time, qs);
         break;

      case exitProc:
         removeProcess(getLifespanEventPID(event), qs);
         break;

      case reqRes:
         requestResource(getResourceEventPID(event), getResource(event), qs);
         break;

      case interruptProc:
         resourceInterrupt(getResourceEventPID(event), getResource(event), qs);
         break;

      case timerInterrupt:
         timeInterrupt(time, qs);
         break;

      default:
         break;
   }
}

void startDispatching() {
   Q *qs[] = { newQ(), newQ(), newQ(), newQ(), newQ(), newQ(), newQ() };
   char **event;

   while ((event = parseLine()) != NULL) {
      dispatch(event, qs);
   }
}
