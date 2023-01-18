#include <iostream>
using namespace std;

int binary_search_recursive(int *v, int key, int start, int end) {
  int middle = (start + end) / 2;

  if (start > end) return -1;

  if (middle == key) return middle;

  if (v[middle] > key) {
    return binary_search_recursive(v, key, 0, middle -1);
  } 

  return binary_search_recursive(v, key, middle + 1, end);
}

bool binary_search(int *v, int key, int size) {
  int start = 0;
  int end = size;
  int middle;

  while(start <= end && start < size) {
    middle = (start + end) / 2;

    if (v[middle] == key) return true;

    if (v[middle] > key) {
      end = middle -1;
    } else {
      start = middle + 1;
    }
  }

  return false;
}

int main () {
  int v[10] = {1, 2, 3, 4, 10, 12, 18, 25, 40, 45};

  cout << binary_search(v, 11, 10);
}