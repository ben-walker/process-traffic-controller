#include "parser.h"
#include "fatalError.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **parseLine() {
   size_t len, count = 0;
   char *line = NULL;
   if (getline(&line, &len, stdin) == -1) {
      free(line);
      return NULL;
   }
   line[strcspn(line, "\r\n")] = 0;
   if (strlen(line) == 0) {
      free(line);
      return NULL;
   }

   char *tok, *saved, **delimLine = malloc(sizeof(char *) * MAX_ELEMS);
   if (delimLine == NULL) fatal("malloc");

   for (tok = strtok_r(line, " ", &saved); tok; tok = strtok_r(NULL, " ", &saved)) {
      if (count == MAX_ELEMS)
         break;
      delimLine[count++] = strdup(tok);
      delimLine[count] = NULL;
   }
   free(line);
   return delimLine;
}
