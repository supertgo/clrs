#include <iostream>
using namespace std;

template <typename T>
T sum_array(T v[], int size) {
    T sum = 0;

     for (int i = 0; i < size; i++) {
        sum += v[i];
    }

    return sum;
}

int main () {
    int v[] = {2, 3, 1, 6, 5, 4};
    double u[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    cout << "First array: " << sum_array(v, 6) << "\n";
    cout << "Second array: " << sum_array(u, 10) << "\n";

    return 0;
}