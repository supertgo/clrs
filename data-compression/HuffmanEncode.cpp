#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#include "HuffmanCoding.h"

int main(int argc, char* argv[]) {
  char fileName[30];
  HuffmanCoding Huffman;
  if (argc != 2) {
    cout << "Enter a file name: ";
    cin >> fileName;
  }
  else strcpy(fileName, argv[1]);
  ifstream fln(fileName, ios::binary);
  if (fln.fail()) {
    cerr << "Cannot open " << fileName << endl;
    return 0;
  }
  Huffman.decompress(fileName, fln);
  fln.close();
  return 0;
}