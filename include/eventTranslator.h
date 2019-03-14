#ifndef EVENT_TRANSLATOR_
#define EVENT_TRANSLATOR_

typedef enum eventTypes {
   createProc,
   exitProc,
   reqRes,
   interruptProc,
   timerInterrupt
} eventTypes;

int getEventType(char **event);

int getEventTime(char **event);

int getCreateEventPID(char **event);

int getResource(char **event);

int getRequestEventPID(char **event);

#endif // EVENT_TRANSLATOR
