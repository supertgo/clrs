#include <iostream>
#include <cstdlib>
using namespace std;
#include "heap.h"

Heap heap;
void program() {
  static int val = 123;
  int rn, p, q, r;
  if (heap.rootCnt == 0) {
    p = 0;
    rn = 1;
  }
  else {
    rn = rand() % 100 + 1;
    p = rand() % heap.rootCnt + 1;
    q = rand() % heap.rootCnt;
    r = rand() % heap.rootCnt;
  }

  if (rn <= 20)
    heap.allocateAtom(p, val++);
  else if (rn <= 40)
    heap.allocateList(p, q, r);
  else if (rn <= 60)
    heap.updateTail(q, r);
  else if (rn <= 80)
    heap.updateTail(q, r);
  else heap.deallocate(p);
  cout << heap;
}

int main() {
  for (int i = 0; i < 50; i++)
    program();
  return 0;
}