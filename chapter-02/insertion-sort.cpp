#include <iostream>
using namespace std;

template <typename T>
void insertionSort(T data[], int n) {
  for (int i = 0, j; i < n; i++) {
    T tmp = data[i];

    for (j = i; j > 0 && tmp < data[j - 1]; j--) {
      data[j] = data[j - 1];
    }

    data[j] = tmp;
  }
}

template <typename T>
void insertionSortDecreasing(T data[], int n) {
  for (int i = 0, j; i < n; i++) {
    T tmp = data[i];

    for (j = i; j > 0 && tmp > data[j - 1]; j--) {
      data[j] = data[j - 1];
    }

    data[j] = tmp;
  }
}

template <typename T>
int linear_search(T data[], T x, int n) {
  for (int i = 0; i < n; i++) {
    if (data[i] == x) {
      return true;
    }
  }

  return false;
}


int* add_binary_intergers(int a[], int b[], int n) {
  int *c = new int[n + 1];
  int carry = 0;

  for (int i = 0; i < n; i++) {
    c[i] = (a[i] + b[i] + carry) % 2;
    carry = (a[i] + b[i] + carry) / 2;
  }

  c[n + 1] = carry;

  return c;
}

int main () {
  int v[] = {1, 0, 0};
  int u[] = {1, 0, 0};
  double w[] = {1.4, 2.2, 3.1, 4, 5.6, 6.8, 7, 8.1, 9.3, 10};

  int *newArray = add_binary_intergers(v, u, 3);
  

  return 0;
}