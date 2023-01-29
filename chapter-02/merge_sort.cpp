#include <iostream>
using namespace std;

void print_array(int* v, int size) {
  for (int i = 0; i < size ; i++) {
    cout << v[i] << ' ';
  }

  cout << '\n';
}

void merge(int* v, int left, int mid, int right) {
  auto const left_size = mid - left + 1;
  auto const right_size = right - mid;

  auto *leftArray = new int[left_size];
  auto *rightArray = new int[right_size];

  for (auto i = 0; i < left_size; i++) 
    leftArray[i] = v[left + i];
  for (auto j = 0; j < right_size; j++)
    rightArray[j] = v[mid + 1 + j];

  auto indexOfSubArrayOne = 0;
  auto indexOfSubArrayTwo = 0;
  int indexOfMergedArray = left;
  
  while (indexOfSubArrayOne < left_size && indexOfSubArrayTwo < right_size) {
    if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
      v[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
      indexOfSubArrayOne++;
    } else {
      v[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
      indexOfSubArrayTwo++;
    }
    indexOfMergedArray++;
  }

  while (indexOfSubArrayOne < left_size) {
     v[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
     indexOfSubArrayOne++;
     indexOfMergedArray++;
  }

  while (indexOfSubArrayTwo < right_size) {
    v[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
    indexOfSubArrayTwo++;
    indexOfMergedArray++;
  }

  delete[] leftArray;
  delete[] rightArray;
}


void merge_sort(int* v, int const left, int const right) {
  if (left >= right) return;

  auto mid = left + (right - left) / 2;

  merge_sort(v, left, mid);
  merge_sort(v, mid + 1, right);

  merge(v, left, mid, right);
}

int binary_search(int *v, int key, int size) {
  int start = 0;
  int end = size;
  int middle;

  while(start <= end && start < size) {
    middle = (start + end) / 2;

    if (v[middle] == key) return key;

    if (v[middle] > key) {
      end = middle -1;
    } else {
      start = middle + 1;
    }
  }

  return -1;
}

int* two_sum(int* v, int target, int size) {
  merge_sort(v, 0, 5);
  int *result = new int[2];

  for (int i = 0; i < size; i++) {
    int search = binary_search(v, target - v[i], size);

    if (search != -1) {
      result[0] = search;
      result[1] = v[i];
    }
  }

  return result;
}

int main () {
  int v[] = {2, 3, 1, 6, 5, 4};

  int *result = two_sum(v, 3, 6);
  
  print_array(result, 2);
  return 0;
}