#include "pcbQueue.h"
#include <stdlib.h>

Queue *newQueue() {
   Queue *q = malloc(sizeof(Queue));
   q->front = q->back = NULL;
   return q;
}
