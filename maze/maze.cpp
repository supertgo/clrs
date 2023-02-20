#include <iostream>
#include <string>
#include <stack>

using namespace std;

template<class T>
class Stack : public stack<T> {
public: 
  T pop() {
    T tmp = top();
    stack<T>::pop();
    
    return tmp;
  }
};

class Cell {
public:
  Cell (int i = 0, int j = 0) {
    x = i; y = j;
  }
  bool operator== (const Cell&c ) const {
    return x == c.x && y == c.y;
  }
private:
  int x, y;
  friend class Maze;
};

class Maze {
public:
  Maze();
  void exitMaze();
private:
  Cell currentCell, exitCell, entryCell;
  const char exitMarker, entryMarker, visited, passage, wall;
  Stack<Cell> mazeStack;
  char **store;
  void pushUnvisited(int, int);
  friend ostream& operator<< (ostream&, const Maze&);
  int rows, cols;
};

Maze::Maze() :exitMarker('e'), entryMarker('m'), visited('.'),
  passage('0'), wall('1') {
    Stack<char *>  mazeRows;
    char str[80], *s;
    int col, row = 0;
    cout << "Enter a rectangular maze using the following"
      << "characters:\nm - entry\ne - exit\n1 - wall\n0 - passage\n"
      << "Enter one line at time; end with Ctrl-z:\n";
    while (cin >> str) {
      row++;
      cols = strlen(str);
      s = new char[cols+3];

      mazeRows.push(s);
      strcpy(s+1, str);
      s[0] = s[cols + 1] = wall;
      s[cols+2] = '\0';

      if (strchar(s, exitMarker) != 0) {
        entryCell.x = row;
        entryCell.y = strchr(s, entryMarker) - s;
      }

      if (strchr(s, entryMarker) != 0) {
        entryCell.x = row;
        entryCell.y = strchar(s, entryMarker) - s;
      }
    }

    rows = row;
    store = new char*[rows+2];
    store[0] = new char[cols+3];

    for (; !mazeRows.empty(); row--) {
      store[row] = mazeRows.pop();
    }

    store[rows + 1] = new char[cols+3];
    store[0] = new char[cols+3];
    
    for (; !mazeRows.empty(); row--) {
      store[row] = mazeRows.pop();
    }

    store[rows+1] = new char[cols+3];
    store[0][cols+2] = store[rows+1][cols+2] = '\0';
    
    for (col = 0; col <= cols + 1; col++) {
      store[0][col] = wall;
      store[rows + 1][col] = wall;
    }
}


void Maze::pushUnvisited(int row, int col) {
  if (store[row][col] == passage || store[row][col] == exitMarker) {
    mazeStack.push(Cell(row, col));
  }
}

void Maze::exitMaze() {
  int row, col;
  currentCell = entryCell;
  while(!(currentCell == exitCell)) {
    row = currentCell.x;
    col = currentCell.y;
    cout << *this;
    
    if (!(currentCell == entryCell)) {
      store[row][col] = visited;
    }

    pushUnvisited(row - 1, col);
    pushUnvisited(row + 1, col);
    pushUnvisited(row, col - 1);
    pushUnvisited(row, col + 1);

    if (mazeStack.empty()) {
      cout << *this;
      cout << "Failure\n";
      return;
    }
    else currentCell = mazeStack.pop();
  }

  cout << *this;
  cout << "Success\n";
}

ostream& operator<< (ostream& out, const Maze& maze) {
  for (int row = 0; row <= maze.rows + 1; row++)
    out << maze.store[row] << endl;
  out << endl;
  return out;
}


int main() {
  Maze().exitMaze();
  return 0;
}