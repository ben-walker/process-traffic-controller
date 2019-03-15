#include "queues.h"

int resourceToQIndex(int resource) {
   switch (resource) {
      case 1: return res1Q;
      case 2: return res2Q;
      case 3: return res3Q;
      case 4: return res4Q;
      case 5: return res5Q;
   }
   return -1;
}
