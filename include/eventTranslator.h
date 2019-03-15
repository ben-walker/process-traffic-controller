/**
 * Ben Walker
 * CIS*3110
 * 
 * Library to strip useful data from simulated CPU events.
 * "Useful data" includes: event type, time, pid, and resource number.
 */

#ifndef EVENT_TRANSLATOR_
#define EVENT_TRANSLATOR_

typedef enum eventTypes {
   createProc, // create process
   exitProc, // exit process
   reqRes, // request a resource for a process
   interruptProc, // interrupt from a resource
   timerInterrupt // interrupt from system timer
} eventTypes;

int getEventType(char **event);

int getEventTime(char **event);

int getLifespanEventPID(char **event);

int getResource(char **event);

int getResourceEventPID(char **event);

#endif // EVENT_TRANSLATOR
