#include "eventTranslator.h"
#include "numbers.h"
#include <string.h>
#include <stdlib.h>

static const char *CREATE = "C",
   *EXIT = "E",
   *REQUEST = "R",
   *INTERRUPT = "I",
   *TIMER = "T";

typedef enum indices {
   timeIdx,
   typeIdx
} indices;

int getEventType(char **event) {
   const char *type = event[typeIdx];
   if (strncmp(type, CREATE, 1) == 0)
      return createProc;
   if (strncmp(type, EXIT, 1) == 0)
      return exitProc;
   if (strncmp(type, REQUEST, 1) == 0)
      return reqRes;
   if (strncmp(type, INTERRUPT, 1) == 0)
      return interruptProc;
   if (strncmp(type, TIMER, 1) == 0)
      return timerInterrupt;
   return -1;
}

int getEventTime(char **event) {
   int timeNum;
   if (toNum(event[timeIdx], &timeNum) == EXIT_FAILURE)
      return -1;
   return timeNum;
}

int getLifespanEventPID(char **event) {
   int pid;
   if (toNum(event[typeIdx + 1], &pid) == EXIT_FAILURE)
      return -1;
   return pid;
}

int getResource(char **event) {
   int type = getEventType(event);
   if (type != reqRes && type != interruptProc)
      return -1;
   int resource;
   if (toNum(event[typeIdx + 1], &resource) == EXIT_FAILURE)
      return -1;
   return resource;
}

int getResourceEventPID(char **event) {
   int pid;
   if (toNum(event[typeIdx + 2], &pid) == EXIT_FAILURE)
      return -1;
   return pid;
}
