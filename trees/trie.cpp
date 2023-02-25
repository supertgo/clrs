#include <iostream>
#include <cstring>
#include <cstdlib>
#include "trie.h"

using namespace std;

TrieLeafNode::TrieLeafNode(char *suffix) {
  leaf = true;
  word = new char[strlen(suffix)+1];

  if (word == 0) {
    cerr << "Out of memory2.\n";
    exit(-1);
  }
  strcpy(word, suffix);
}

TrieNonLeafNode::TrieNonLeafNode(char ch) {
  ptrs = new TrieNonLeafNode*;
  letters = new char[2];

  if (ptrs == 0 || letters == 0) {
    cerr << "Out of memory3.\n";
    exit(1);
  }
  leaf = false;
  endOfWord = false;
  *ptrs = 0;
  *letters = ch;
  *(letters+1) = '\0';
}

Trie::Trie(char *word) : notFound(-1) {
  root = new TrieNonLeafNode(*word);
  createLeaf(*word, word + 1, root);
}

void Trie::printTrie(int depth, TrieNonLeafNode *p, char *prefix) {
  register int i;
  if (p->leaf) {
    TrieLeafNode *lf = (TrieLeafNode*)p;
    for (i = 1; i <= depth; i++) {
      cout << " ";
    }
    cout << " >> " << prefix << "|" << lf->word << endl;
  } else {
    for (i = strlen(p->letters) - 1; i >= 0; i--)
      if (p->ptrs[i] != 0) {
        prefix[depth] = p->letters[i];
        prefix[depth +1] = '\0';
        printTrie(depth +1, p->ptrs[i], prefix);
      }
    if (p->endOfWord) {
      prefix[depth] = '\0';
      for (i = 1; i <= depth + 1; i++)
        cout << " ";
      cout << " >>>" << prefix << "\n";
    }
  }
}

int Trie::position(TrieNonLeafNode *p, char ch) {
  int i = 0;
  for (; i < strlen(p->letters) && p->letters[i] != ch; i++);
  if (i < strlen(p->letters))
    return i;
  else return notFound;
}

bool Trie::wordFound(char *word) {
  TrieNonLeafNode *p = root;
  TrieLeafNode *lf;
  int pos;

  while (true) {
    if (p->leaf) {
      lf = (TrieLeafNode*) p;
      if (strcmp(word, lf->word) == 0)
        return true;
      else return false;
    }
    else if (*word == '\0')
      if (p->endOfWord)
        return true;
      else return false;
    else if ((pos = position(p, *word)) != notFound &&
      p->ptrs[pos] != 0) {
        p = p->ptrs[pos];
        word++;
      }
    else return false;
  }
}

void Trie::addCell(char ch, TrieNonLeafNode *p, int stop) {
  int i, len = strlen(p->letters);
  char *s = p->letters;
  TrieNonLeafNode **tmp = p->ptrs;
  p->letters = new char[len +2];
  p->ptrs = new TrieNonLeafNode*[len+1];

  if (p->letters == 0 || p->ptrs == 0) {
    cerr << "Out of memory.\n";
    exit(1);
  }

  for (i = 0; i < len + 1; i++)
    p->ptrs[i] = 0;
  if (stop < len)
    for (i = len; i >= stop + 1; i--) {
      p->ptrs[i] = tmp[i - 1];
      p->letters[i] = s[i -1];
    }
  p->letters[stop] = ch;
  for (i = stop - 1; i >= 0; i--) {
    p->ptrs[i] = tmp[i];
    p->letters[i] = s[i];
  }
  p->letters[len+1] = '\0';
  delete [] s;
}

void Trie::createLeaf(char ch, char *suffix, TrieNonLeafNode *p) {
  int pos = position(p, ch);
  if (pos == notFound) {
    for (pos = 0; pos < strlen(p->letters) &&
      p->letters[pos] < ch; pos++);
    addCell(ch, p, pos);
  }
  p->ptrs[pos] = (TrieNonLeafNode*) new TrieLeafNode(suffix);
}

void Trie::insert(char *word) {
  TrieNonLeafNode *p = root;
  TrieLeafNode *lf;
  int offset, pos;
  char *hold = word;

  while (true) {
    if (*word == '\0') {
      if (p->endOfWord)
        cout << "Duplicate entry1 " << hold << endl;
      else p->endOfWord = true;
      return;
    }
  
    pos = position(p, *word);
    if (pos == notFound) {
      createLeaf(*word, word + 1, p);
      return;
    }
    else if (pos != notFound && p->ptrs[pos]->leaf) {
      lf = (TrieLeafNode*) p->ptrs[pos];
      if (strcmp(lf->word, word + 1) == 0) {
        cout << "Duplicate entry2 " << hold << endl;
        return;
      }
      offset = 0;

      do {
        pos = position(p, word[offset]);
        if (strlen(word) == offset+1) {
          p->ptrs[pos] = new TrieNonLeafNode(word[offset]);
          p->ptrs[pos]->endOfWord = true;
          createLeaf(lf->word[offset], lf->word + offset + 1, p->ptrs[pos]);
          return;
        }
        else if (strlen(lf->word) == offset) {
          p->ptrs[pos] = new TrieNonLeafNode(word[offset +1]);
          p->ptrs[pos]->endOfWord = true;
          createLeaf(word[offset+1], word + offset + 2, p->ptrs[pos]);
          return;
        }
        p->ptrs[pos] = new TrieNonLeafNode(word[offset+1]);
        p = p->ptrs[pos];
        offset++;
      } while (word[offset] == lf->word[offset -1]);
      offset--;

      char *s = "";
      if (strlen(word) > offset + 2)
        s = word + offset + 2;
      createLeaf(word[offset + 1], s, p);

      if (strlen(lf->word) > offset + 1)
        s = lf->word + offset + 1;
      else s = "";
      createLeaf(lf->word[offset], s, p);
      delete [] lf->word;
      delete lf;
      return;
    }
    else {
      p = p->ptrs[pos];
      word++;
    }
  }
}
