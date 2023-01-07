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


int main () {
    int v[] = {2, 3, 1, 6, 5, 4};
    double u[] = {1.4, 2.2, 3.1, 4, 5.6, 6.8, 7, 8.1, 9.3, 10};

    insertionSort(u, 10);

    for (int i = 0; i < 10; i++) {
      cout << u[i] << ' ';
    }

    return 0;
}