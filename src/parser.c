/**
 * Ben Walker
 * CIS*3110
 * 
 * Library to parse a line from stdin and convert it to
 * an array of strings (white space delimited).
 */

#include "parser.h"
#include "fatalError.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Parse a line from stdin into array of strings (white space delimited).
 */
char **parseLine() {
   size_t len, count = 0;
   char *line = NULL;
   if (getline(&line, &len, stdin) == -1) { // if EOF or no line
      free(line);
      return NULL;
   }
   line[strcspn(line, "\r\n")] = 0; // strip \r and \n from line
   if (strlen(line) == 0) {
      free(line);
      return NULL;
   }

   char *tok, *saved, **delimLine = malloc(sizeof(char *) * MAX_ELEMS);
   if (delimLine == NULL) fatal("malloc");

   // loop through line tokens and add them to array of strings
   for (tok = strtok_r(line, " ", &saved); tok; tok = strtok_r(NULL, " ", &saved)) {
      if (count == MAX_ELEMS) break;
      delimLine[count++] = strdup(tok);
      delimLine[count] = NULL;
   }
   free(line);
   return delimLine;
}
