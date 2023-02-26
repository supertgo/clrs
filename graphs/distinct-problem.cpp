#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <list>
#include <stack>
#include <iterator>
using namespace std;
#include "genBST.h"

class VertexArrayRec;
class LocalTree;
class Network;

class Vertex {
public:
  Vertex() {
  }
  Vertex(int id, int c, int ef, bool f, Vertex *t = 0) {
    idNum = id; capacity = c; edgeFlow = ef; forward = f; twin = t;
  }
  bool operator== (const Vertex& v) const {
    return idNum == v.idNum;
  }
  bool operator!= (const Vertex& v) const {
    return idNum != v.idNum;
  }
  bool operator< (const Vertex& v) const {
    return idNum < v.idNum;
  }
  bool operator> (const Vertex& v) const {
    return idNum > v.idNum;
  }
private:
  int idNum, capacity, edgeFlow;
  bool forward;
  Vertex *twin;
  friend class Network;
  friend ostream& operator<< (ostream&, const Vertex&);
};

class NetTreeNode {
public:
  NetTreeNode(forwardArrayRec **v = 0) {
    verticesPtr = v;
    adjacent = new list<Vertex>;
  }
  bool operator< (const NetTreeNode &tr) const {
    return strcmp(idName, tr.idName) < 0;
  }
  bool operator== (const NetTreeNode& tr) const {
    return strcmp(idName, tr.idName) == 0;
  }
private:
  int idNum;
  char *idName;
  VertexArrayRec **verticesPtr;
  list<Vertex> *adjacent;
  friend class Network;
  friend class LocalTree;
  friend ostream& operator<< (ostream&, const NetTreeNode&);
};

class VertexArrayRec {
public:
  VertexArrayRec() {
    adjacent = 0;
  }
private:
  char *idName;
  int vertexFlow;
  bool labeled;
  int parent;
  Vertex *corrVer;
  list<Vertex> *adjacent;
  friend class Network;
  friend class LocalTree;
  friend ostream& operator<< (ostream&, const Network&);
};

class LocalTree : public BST<NetTreeNode> {
  void visit(BSTNode<NetTreeNode>* p) {
    (*(p->el.verticesPtr)) [p->el.idNum].idName = p->el.idName;
    (*(p->el.verticesPtr)) [p->el.idNum].adjacent = p->el.adjacent;
  }
};

class Network {
public:
  Network() : sink(1), source(0), none(-1), numOfVertices(2) {
    verticesPtr = new VertexArrayRec*;
  }
  void readCommittees(char *committees);
  void FordFulkersonMaxFlow();
private:
  const int sink, source, none;
  int numOfVertices;
  VertexArrayRec *vertices;
  VertexArrayRec **verticesPtr;

  int edgeSlack(Vertex *u) const {
    return u->capacity - u->edgeFlow;
  }
  int min(int n, int m) const {
    return n < m ? n : m;
  }
  bool Labeled(Vertex *v) const {
    return vertices[v->idNum].labeled;
  }
  void label(Vertex *, int);
  void augmentPath();
  friend class LocalTree;
  friend ostream& operator<< (ostream&, const Network&);
};

ostream& operator<< (ostream& out, const NetTreeNode& tr) {
  out << tr.idNum << ' ' << tr.idName << ' ';
  return out;
}

ostream& operator<< (ostream& out, const Vertex& vr) {
  out << vr.idNum << ' ' << vr.capacity << ' ' << vr.edgeFlow << ' '
     << vr.forward << "|";
  return out;
}

ostream& operator<< (ostream& out, const Network& net) {
  ostream_iterator<Vertex> output(out, " ");
  for (int i = 0; i < net.numOfVertices; i++) {
    out << i << ": "
      << net.vertices[i].idName << '|'
      << net.vertices[i].vertexFlow << '|'
      << net.vertices[i].labeled << '|'
      << net.vertices[i].parent << '|'
      << /* net,veritices[i].corrVer << */ " ->";
    if (net.vertices[i].adjacent != 0)
      copy(net.vertices[i].adjacent->begin(),
        net.vertices[i].adjacent->end(), output);
    out << endl;
  }

  return out;
}

void Network::readCommittees(char *fileName) {
  char i, name[80], *s;
  LocalTree committeeTree, memberTree;
  Vertex memberVer(0, 1, 0, false), commVer(0, 1, 0, true);
  Vertex *commVerAddr, *memberVerAddr;
  NetTreeNode committeeTreeNode(verticesPtr),
    memberTreeNode(verticesPtr), *member;
  list<Vertex> *sourceList = new list<Vertex>;
  ifstream fln(fileName);
  if (fln.fail()) {
    cerr << "Cannot open" << fileName << endl;
    exit(-1);
  }

  while (!fln.eof()) {
    fln >> name[0];
    if (fln.eof())
      break;
    for (i = 0; name[i] != ':';)
      name[++i] = fln.get();
    for (i--; isspace(name[i]); i--);
    name[i + 1] = '\0';
    s = strdup(name);
    committeeTreeNode.idNum = commVer.idNum = numOfVertices++;
    committeeTreeNode.idName = s;

    for (bool lastMember = false; lastMember == false; ) {
      fln >> name[0];
      for (i = 0; name[i] != ',' && name[i] != ';';)
        name[++i] = fln.get();
      if (name[i] == ';')
        lastMember = true;
      for (i--; isspace(name[i]); i--);
      name[i + 1] = '\0';
      s = strdup(name);
      memberTreeNode.idName = s;
      commVer.forward = false;
      if ((member = memberTree.search(memberTreeNode)) == 0) {
        memberVer.idNum = memberTreeNode.idNum = numOfVertices++;
        memberTreeNode.adjacent->push_front(Vertex(sink, 1, 0, true));
        memberTreeNode.adjacent->push_front(commVer);
        commVerAddr = &*memberTreeNode.adjacent->begin();
        memberTree.insert(memberTreeNode);
        memberTreeNode.adjacent = new list<Vertex>;
      }
      else {
        memberVer.idNum = member->idNum;
        member->adjacent->push_front(commVer);
        commVerAddr = &*member->adjacent->begin();
      }
      memberVer.forward = true;
      committeeTreeNode.adjacent->push_front(memberVer);
      memberVerAddr->twin = commVerAddr;
      commVerAddr->twin = memberVerAddr;
    }
    commVer.forward = true;
    sourceList->push_front(commVer);
    committeeTree.insert(committeeTreeNode);
    committeeTreeNode.adjacent = new list<Vertex>;
  }
  fln.close();
  cout << "\nCommittee tree:\n";
  cout << "\nMember tree:\n";
  vertices = *verticesPtr = new VertexArrayRec[numOfVertices];

  if (vertices == 0) {
    cerr << "Not enough memory\n";
    exit(-1);
  }

  vertices[source].idName = "source";
  vertices[sink].idName = "sink";
  vertices[source].adjacent = sourceList;
  vertices[source].parent = none;
  committeeTree.inorder();
  memberTree.inorder();
}

void Network::label(Vertex *u, int v) {
  vertices[u->idNum].labeled = true;
  if (u->forward)
    vertices[u->idNum].vertexFlow =
      min(vertices[v].vertexFlow, edgeSlack(u));
  else vertices[u->idNum].vertexFlow =
    min(vertices[v].vertexFlow, u->edgeFlow);
  vertices[u->idNum].parent = v;
  vertices[u->idNum].corrVer = u;
}

void Network::augmentPath() {
  register int i, sinkFlow = vertices[sink].vertexFlow;
  Stack<char *> path;
  for (i = sink; i != source; i = vertices[i].parent) {
    path.push(vertices[i].idName);
    if (vertices[i].corrVer->forward) 
      vertices[i].corrVer->edgeFlow += sinkFlow;
    else vertices[i].corrVer->edgeFlow -= sinkFlow;
    if (vertices[i].parent != source && i != sink)
      vertices[i].corrVer->twin->edgeFlow = 
        vertices[i].corrVer->edgeFlow;
  }
  for (i = 0; i < numOfVertices; i++)
    vertices[i].labeled = false;
  cout << " source";

  while(!path.empty())
    cout << " => " << path.pop();
  cout << " (augmented by " << sinkFlow << ");\n";
}

void Network::FordFulkersonMaxFlow() {
  Stack<int> labeled;
  Vertex *u;
  list<Vertex>::iterator it;
  for (int i = 0; i < numOfVertices; i++) {
    vertices[i].labeled = false;
    vertices[i].vertexFlow = 0;
    vertices[i].parent = none;
  }
  vertices[source].vertexFlow = INT32_MAX;
  labeled.push(source);
  cout << "Augmenting paths: \n";
  while (!labeled.empty()) {
    int v = labeled.front();
    for (it = vertices[v].adjacent->begin(), u = &*it;
      it != vertices[v].adjacent->end(); it++, u = &*it)
        if (!Labeled(u)) {
          if (u->forward && edgeSlack(u) > 0)
            label(u, v);
          else if (!u->forward && u->edgeFlow > 0)
            label(u, v);
          if (Labeled(u))
            if (u->idNum == sink) {
              augmentPath();
              while (!labeled.empty())
                labeled.pop();
              labeled.push(source);
              break;
            }
            else {
              labeled.push(u->idNum);
              vertices[u->idNum].labeled = true;
            }
        }
  }
}

int main(int argc, char * argv[]) {
  char fileName[80];
  Network net;
  if (argc != 2) {
    cout << "Enter a file name: ";
    cin.getline(fileName, 80);
  }
  else strcpy(fileName, argv[1]);
  net.readCommittees(fileName);
  cout << net;
  net.FordFulkersonMaxFlow();
  cout << net;
  return 0;
}