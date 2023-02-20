#include <queue>
#include <stack>

using namespace std;

#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

template<class T>
class Stack : public queue<T> {
public:
  T dequeue() {
    T tmp = front();
    queue<T>::pop();
    return tmp;
  }
  void enqueue(const T& el) {
    push(el);
  }
};

template<class T>
class BSTNode {
public:
  BSTNode() {
    left = right = 0;
  }
  BSTNode(const T& e, BSTNode<T> *l = 0, BSTNode<T> *r = 0) {
    el = e; left = l; right = r;
  }
  T el;
  BSTNode<T> *left, *right;
};

template<class T>
class BST {
public:
  BST() {
    root = 0;
  }
  ~BST() {
    clear();
  }
  void clear() {
    clear(root); root = 0;
  }
  bool isEmpty() const {
    return root == 0;
  }
  void preorder() {
    preorder(root);
  }
  void postorder() {
    postorder(root);
  }
  T* search(const T& el) const {
    return search(root, el);
  }
  void breadthFirst();
  void iterativePreorder();
  void iterativeInorder();
  void iterativePostorder();
  void MorrisInorder();
  void insert(const T&);
  void deleteByMerging(BSTNode<T> *&);
  void findAndDeleteByMerging(const T&);
  void deleteByCopying(BSTNode<T> *&);
  void balance(T*, int, int);
protected:
  BSTNode<T>* root;
  void clear(BSTNode<T>*);
  T* search(BSTNode<T>*, const T&) const;
  void preorder(BSTNode<T>*);
  void inorder(BSTNode<T>*);
  void postorder(BSTNode<T>*);
  virtual void visit(BSTNode<T>*p) {
    cout  << p->el << ' ';
  }
};

template<class T>
T* BST<T>::search(BSTNode<T>* p, const T& el) const {
  while (p != 0)
    if (el == p->el)
      return &p->el;
    else if (el < p->el)
      p = p->left;
    else p = p->right;
  return 0;
}

template<class T>
void BST<T>::breadthFirst() {
  Queue<BSTNode<T>*> queue;
  BSTNode<T> *p = root;

  if (p != 0) {
    queue.enqueue(p);
    while (!queue.empty()) {
      p = queue.dequeue();
      visit(p);
      if (p->left != 0)
        queue.enqueue(p->left);
      if (p->right != 0)
        queue.enqueue(p->right);
    }
  }
}

template<class T>
void BST<T>::inorder(BSTNode<T> *p) {
  if (p != 0) {
    inorder(p->left);
    visit(p);
    inorder(p->right);
  }
}

template<class T>
void BST<T>::preorder(BSTNode<T> *p) {
  if (p != 0) {
    visit(p);
    preorder(p->left);
    preorder(p->right);
  }
}

template<class T>
void BST<T>::postorder(BSTNode<T>* p) {
  if (p != 0) {
    postorder(p->left);
    postorder(p->right);
    visit(p);
  }
}

template<class T>
void BST<T>::iterativePreorder() {
  Stack<BSTNode<T>*> travStack;
  BSTNode<T> *p = root;

  if (p != 0) {
    travStack.push(p);
    while (!travStack.empty()) {
      p = travStack.pop();
      visit(p);
      if (p->right != 0)
        travStack.push(p->right);
      if (p->left != 0)
        travStack.push(p->left);
    }
  }
}

template<class T>
void BST<T>::iterativePostorder() {
  Stack<BSTNode<T>*> travStack;
  BSTNode<T>* p = root, *q = root;
  while (p != 0) {
    for (; p->left != 0; p = p->left)
      travStack.push(p);
    while (p->right == 0 || p->right == q) {
      visit(p);
      q = p;

      if (travStack.empty())
        return;
      p = travStack.pop();
    }

    travStack.push(p);
    p = p->right;
  }
}

template<class T>
void BST<T>::iterativeInorder() {
  Stack<BSTNode<T>*> travStack;
  BSTNode<T> *p = root;

  while (p != 0) {
    while (p != 0) {
      if (p->right)
        travStack.push(p->right);
      travStack.push(p);
      p = p->left;
    }

    p = travStack.pop();
    while (!travStack.empty() && p->right == 0) {
      visit(p);
      p = travStack.pop();
    }
    visit(p);
    if (!travStack.empty())
      p = travStack.pop();
    else p = 0;
  }
}

template<class T>
void BST<T>::MorrisInorder() {
  BSTNode<T> *p = root, *tmp;

  while (p != 0)
    if (p->left == 0) {
      visit(p);
      p = p->right;
    }
    else {
      tmp = p->left;
      while (
        tmp->right != 0 &&
        tmp->right != p 
      ) {
        tmp = tmp->right;
      }

      if (tmp->right == 0) {
        tmp->right = p;
        p = p->left;
      }
      else {
        visit(p);
        tmp->right = 0;
        p = p->right;
      }
    }
}