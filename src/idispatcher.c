#include "idispatcher.h"
#include "parser.h"
#include "pcbQ.h"
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
   Q *readyQ =  newQ(),
      *res1Q = newQ(),
      *res2Q = newQ(),
      *res3Q = newQ(),
      *res4Q = newQ(),
      *res5Q = newQ();
   char **event;

   while ((event = parseLine()) != NULL) {
      dispatch(event);
   }
}
