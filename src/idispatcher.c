#include "idispatcher.h"
#include "parser.h"
#include "pcbQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int TIME = 0, EVENT = 1;
static const char *CREATE = "C", *EXIT = "E", *REQUEST = "R", *INTERRUPT = "I", *TIMER = "T";

void dispatch(char **line) {
   if (strcmp(line[EVENT], CREATE) == 0) {

   } else if (strcmp(line[EVENT], EXIT) == 0) {

   } else if (strcmp(line[EVENT], REQUEST) == 0) {
      
   } else if (strcmp(line[EVENT], INTERRUPT) == 0) {
      
   } else if (strcmp(line[EVENT], TIMER) == 0) {
      
   }
}

void startDispatching() {
   Queue *readyQ = newQueue();
   char **processLine;

   while ((processLine = parseLine()) != NULL) {
      dispatch(processLine);
   }
}
