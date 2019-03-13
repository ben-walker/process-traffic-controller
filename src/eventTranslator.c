#include "eventTranslator.h"
#include <string.h>

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
