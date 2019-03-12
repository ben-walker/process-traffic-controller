#include "parser.h"
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

   char *tok, *saved, **delimLine = malloc(sizeof(char *) * MAX_ELEMS);
   if (delimLine == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
   }

   for (tok = strtok_r(line, " ", &saved); tok; tok = strtok_r(NULL, " ", &saved)) {
      if (count == MAX_ELEMS)
         break;
      delimLine[count] = strdup(tok);
      count += 1;
   }
   free(line);
   return delimLine;
}
