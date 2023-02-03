#include <iostream>
using namespace std;

int get_matrix_column(float** M) {
  return sizeof(M) / sizeof(M[0][0]);
}

int get_matrix_row(float**M ) {
  return sizeof(M) / sizeof(M[0]);
}

bool check_if_its_possible_to_multiplication(float** A, float** B) {
  if(get_matrix_column(A) == get_matrix_row(B)) {
    return 1;
  }

  return 0;
}

void matrices_multiplication(float** A, float** B, float** P, int size) {
  if(check_if_its_possible_to_multiplication(A, B)) return;

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