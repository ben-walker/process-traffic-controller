#ifndef QUEUES_
#define QUEUES_

typedef enum queues {
   runQ,
   readyQ,
   res1Q,
   res2Q,
   res3Q,
   res4Q,
   res5Q,
   deadQ
} queues;

int resourceToQIndex(int resource);

#endif // QUEUES_
