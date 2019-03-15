/**
 * Ben Walker
 * CIS*3110
 * 
 * Utility library to convert a string to a number.
 */

#include "numbers.h"
#include <stdlib.h>
#include <limits.h>

/**
 * Convert a string to an integer.
 */
int toNum(const char *string, int *numeric) {
   if (!string) return EXIT_FAILURE;
   char *end = NULL;
   long converted = strtol(string, &end, 0);

   // check for strtol errors
   if (converted == LONG_MAX || converted == LONG_MIN || end == string)
      return EXIT_FAILURE;
   *numeric = converted;
   return EXIT_SUCCESS;
}
