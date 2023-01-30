#include <iostream>
using namespace std;

void matrices_multiplication(float** A, float** B, float** P, int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) { 
      for (int k = 0; k < size; k++) {
        P[i][j] += A[i][k] * B[k][j] ;
      }
    }
  }
}

int main () {

}