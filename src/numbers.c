#include "numbers.h"
#include <stdlib.h>
#include <limits.h>

int toNum(const char *val, int *numeric) {
   if (!val) return EXIT_FAILURE;
   char *end = NULL;
   long converted = strtol(val, &end, 0);

   if (converted == LONG_MAX || converted == LONG_MIN || end == val)
      return EXIT_FAILURE;
   *numeric = converted;
   return EXIT_SUCCESS;
}
