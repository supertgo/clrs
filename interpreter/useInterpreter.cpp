#include "interpreter.h"

using namespace std;

int main() {
  Statement statement;
  cout << "The program processes statements of the following format:\n"
    << "\t<id> = <expr>;\n\tprint <id>\n\tend\n\n";
  
  while(true) {
    statement.getStatement();
  }

  return 0;
}