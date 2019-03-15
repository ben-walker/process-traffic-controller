/**
 * Ben Walker
 * CIS*3110
 * 
 * Library to strip useful data from simulated CPU events.
 * "Useful data" includes: event type, time, pid, and resource number.
 */

#include "eventTranslator.h"
#include "numbers.h"
#include <string.h>
#include <stdlib.h>

// character representation of events
static const char *CREATE = "C",
   *EXIT = "E",
   *REQUEST = "R",
   *INTERRUPT = "I",
   *TIMER = "T";

// enum of time and type indices within an event
typedef enum indices {
   timeIdx,
   typeIdx
} indices;

/**
 * Returns the enum representation (int) of an event type.
 */
int getEventType(char **event) {
   const char *type = event[typeIdx];
   if (strncmp(type, CREATE, 1) == 0) return createProc;
   if (strncmp(type, EXIT, 1) == 0) return exitProc;
   if (strncmp(type, REQUEST, 1) == 0) return reqRes;
   if (strncmp(type, INTERRUPT, 1) == 0) return interruptProc;
   if (strncmp(type, TIMER, 1) == 0) return timerInterrupt;
   return -1;
}

/**
 * Strips the time from an event, returns it as an int.
 */
int getEventTime(char **event) {
   int timeNum;
   if (toNum(event[timeIdx], &timeNum) == EXIT_FAILURE) return -1;
   return timeNum;
}

/**
 * Returns the PID as an int from a "lifespan" (i.e. Create or Exit) event.
 */
int getLifespanEventPID(char **event) {
   int pid;
   if (toNum(event[typeIdx + 1], &pid) == EXIT_FAILURE) return -1;
   return pid;
}

/**
 * Returns the resource number from a Resource or Resource Interrupt event.
 */
int getResource(char **event) {
   int type = getEventType(event);
   if (type != reqRes && type != interruptProc) return -1; // ensure event is of the right type
   int resource;
   if (toNum(event[typeIdx + 1], &resource) == EXIT_FAILURE) return -1;
   return resource;
}

/**
 * Returns the PID from a resource (i.e. Resource or Resource Interrupt) event.
 */
int getResourceEventPID(char **event) {
   int pid;
   if (toNum(event[typeIdx + 2], &pid) == EXIT_FAILURE) return -1;
   return pid;
}
