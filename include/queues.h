/**
 * Ben Walker
 * CIS*3110
 * 
 * Simple library to hold information about the available queues.
 */

#ifndef QUEUES_
#define QUEUES_

typedef enum queues { // all system queues
   runQ, // running queue; should only ever be of length 1
   readyQ, // ready queue
   res1Q, // resource 1 queue through to...
   res2Q,
   res3Q,
   res4Q,
   res5Q, // the resource 5 queue
   deadQ // queue of terminated processes
} queues;

static const int NUM_QUEUES = 8;

int resourceToQIndex(int resource);

#endif // QUEUES_
