/**
 * Ben Walker
 * CIS*3110
 * 
 * Utility library to print a stderr message and exit
 * in event of a fatal error.
 */

#include "fatalError.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * perror a message and exit with failure.
 */
void fatal(const char *msg) {
   perror(msg);
   exit(EXIT_FAILURE);
}
