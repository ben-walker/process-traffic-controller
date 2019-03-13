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

void newProcess(int pid, Q *qs[]) {
   if (pid < 1)
      return;
   if (isEmpty(qs[runQ]))
      enQ(qs[runQ], newPCB(pid));
   else
      enQ(qs[readyQ], newPCB(pid));
}

void dispatch(char **event, Q *qs[]) {
   int pid;

   switch (getEventType(event)) {
      case createProc:
         newProcess(getCreateEventPID(event), qs);
         break;

      case exitProc:
         break;

      case reqRes:
         break;

      case interruptProc:
         break;

      case timerInterrupt:
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
