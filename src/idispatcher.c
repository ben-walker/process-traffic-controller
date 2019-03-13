#include "idispatcher.h"
#include "parser.h"
#include "pcbQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int TIME_I = 0, EVENT_I = 1;
static const char *CREATE = "C", *EXIT = "E", *REQUEST = "R", *INTERRUPT = "I", *TIMER = "T";

void dispatch(char **event) {
   if (strcmp(event[EVENT_I], CREATE) == 0) {

   } else if (strncmp(event[EVENT_I], EXIT, 1) == 0) {

   } else if (strncmp(event[EVENT_I], REQUEST, 1) == 0) {

   } else if (strncmp(event[EVENT_I], INTERRUPT, 1) == 0) {

   } else if (strncmp(event[EVENT_I], TIMER, 1) == 0) {

   }
}

void startDispatching() {
   Queue *readyQ = newQueue();
   char **event;

   while ((event = parseLine()) != NULL) {
      dispatch(event);
   }
}
