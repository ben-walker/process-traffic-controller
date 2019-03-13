#include "idispatcher.h"
#include "parser.h"
#include "pcbQueue.h"
#include "eventTranslator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dispatch(char **event) {
   switch (getEventType(event)) {
      case createProc:
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
   Queue *readyQ = newQueue();
   char **event;

   while ((event = parseLine()) != NULL) {
      dispatch(event);
   }
}
