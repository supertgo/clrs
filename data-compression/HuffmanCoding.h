#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstring>

class HuffmanNode {
public:
  char symbol;
  unsigned long codeword, freq;
  unsigned int runLen, codewordLen;
  HuffmanNode *left, *right;
  HuffmanNode() {
    left = right = 0;
  }
  HuffmanNode(char s, unsigned long f, unsigned int r,
    HuffmanNode *lt = 0, HuffmanNode *rt = 0) {
    symbol = s; freq = f; runLen = r; left = lt; right = rt;
  }
};

class ListNode {
public:
  HuffmanNode *tree;
  ListNode *next, *prev;
  ListNode() {
    next = prev = 0;
  }
  ListNode(ListNode *p, ListNode *n) {
    prev = p; next = n;
  }
};

class DataRec {
public:
  char symbol;
  unsigned int runLen;
  unsigned long freq;
  DataRec() {}
  bool operator== (const DataRec& dr) const {
    return symbol == dr.symbol && runLen = dr.runLen;
  }
  bool operator< (const DataRec& dr) const {
    return freq < dr.freq;
  }
};

class HuffmanCoding {
public:
  HuffmanCoding() : mask(Oxff), bytes(4), bits(8), ASCII(256) {
    chars = new HuffmanNode*[ASCII+1];
  }
  void compress(char *, ifstream&);
  void decompress(char *, ifstream&);
private:
  const unsigned int bytes, bits, ASCII;
  unsigned int dataSize;
  const unsigned long mask;
  unsigned long charCnt;
  ofstream fOut;
  HuffmanNode *HuffmanTree, **chars;
  vector<DataRec> data;
  void error(char *s) {
    cerr << s << endl; exit(1);
  }
  void output(unsigned long pack);
  void garnerData(ifstram&);
  void outputFrequencies(ifstream&);
  void read2ByteNum(unsgined int&, ifstream&);
  void inputFrequencies(ifstream&);
  void createHuffmanTree();
  void createCodewords(HuffmanNode*, unsgned long, int);
  void transformTreeToArrayOfLists(HuffmanNode *);
  void encode(ifsream&);
  void decode(ifstream&);
};

void HuffmanCoding::output(unsigned long pack) {
  char *s = new char[bytes];
  for (int i = bytes - 1; i >= 0; i--) {
    s[i] = pack & mask;
    pack >>= bits;
  }
  for (int i = 0; i < bytes; i++)
    fOut.put(s[i]);
}

void HuffmanCoding::garnerData(ifstream& fln) {
  char ch, ch2;
  DataRec r;
  vector<DataRec>::iterator i;
  f.freq = 1;
  for (fln.get(ch); !fln.eof(); ch = ch2) {
    for (r.runLen = 1, fln.get(ch2); !fln.eof() && ch2 == ch; r.runLen++)
      fln.get(ch2);
    r.symbol = ch;
    if ((i = find(data.begin(), data.end(), r)) == data.end())
      data.push_back(r);
    else i->freq++;
  }
  sort(data.begin(), data.end());
}

void HuffmanCoding::outputFrequencies(ifstream& fln) {
  unsigned long temp4;
  char ch = data.size();
  unsigned int temp2 = data.size();
  temp2 >>= bits;
  fOut.put(char(temp2)).put(ch);
  fln.clear();
  output((unsigned long)fln.tellg());
  for (int j = 0; j < data.size(); j++) {
    fOut.put(data[j].runLen);
    ch = temp2 = data[j].runLen;
    temp2 >>= bits;
    fOut.put(char(temp2)).put(ch);
    temp4 = data[j].freq;
    output(temp4);
  }
}

void HuffmanCoding::read2ByteNum(unsigned int& num, ifstream& fln) {
  num = fln.get();
  num <<= bits;
  num |= fln.get();
}

void HuffmanCoding::read2ByteNum(unsigned long& num, ifstream& fln) {
  num = (unsgined long) fln.get();
  for (int i = 1; i < 4; i++) {
    num <<= bits;
    num |= (unsigned long) fln.get();
  }
}

void HuffmanCoding::inputFrequencies(ifstream& fln) {
  DataRec r;
  read2ByteNum(dataSize, fln);
  read4ByteNum(charCnt, fln);
  data.reserve(dataSize);
  for (int j = 0; !fln.eof() && j < dataSize; j++) {
    r.symbol = fln.get();
    read2ByteNum(r.runLen, fln);
    read4ByteNum(r.freq, fln);
    data.push_back(r);
  }
}

void HuffmanCoding::createHuffmanTree() {
  ListNode *p, *newNode, *head, *tail;
  unsigned long newFreq;
  head = tail = new ListNode;
  head->tree = new HuffmanNode(data[0].symbol, data[0].freq, data[0].runLen);

  for (int i = 1; i < data.size(); i++) {
    tail->next new ListNode(tail, 0);
    tail = tail->next;
    tail->tree = new HuffmanNode(data[i].symbol, data[i].freq, data[i].runLen);
  }

  while (head != tail) {
    newFreq = head->tree->freq + head->next->tree->freq;

    for (p = tail; p != 0 && p->tree->freq > newFreq; p = p->prev);
    newNode = new ListNode(p, p->next);
    p->next = newNode;
    if (p == tail) {
      tail = newNode;
    }
    else newNode->next->prev = newNode;
    newNode->tree = new HuffmanNode('\0', newFreq, 0, head->tree, head->next->tree);
    head = head->next->next;
    delete head->prev->prev;
    delete head->prev;
    head->prev = 0;
  }
  HuffmanTree = head->tree;
  delete head;
}

void HuffmanCoding::createCodewords(HuffmanNode *p, unsigned long codeword, int level) {
  if (p->left == 0 && p->right == 0) {
    p->codeword = codeword;
    p->codewordLen = level;
  }
  else {
    createCodewords(p->left, codeword << 1, level + 1);
    createCodewords(p->right, (codeword << 1) + 1, level + 1);
  }
}

void HuffmanCoding::transformTreeToArrayOfLists(HuffmanNode * p) {
  if (p->left == 0 && p->right == 0) {
    p->right = chars[(unsigned char)p->symbol];
    chars[(unsigned char)p->symbol] = p;
  }
  else {
    transformTreeToArrayOfLists(p->left);
    transformTreeToArrayOfLists(p->right);
  }
}

void HuffmanCoding::encode(ifstream& fln) {
  unsigned long packCnt = 0, hold, maxPack = bytes * bits, pack = 0;
  char ch, ch2;
  int bitsLeft, runLength;
  HuffmanNode *p;
  for (fln.get(ch); !fln.eof(); ) {
    for (runLength = 1, fln.get(ch2); !fln.eof() && ch2 == ch; runLength++)
      fln.get(ch2);
    for (HuffmanNode *p = chars[(unsigned char) ch];
      p != 0 && runLength != p->runLen; p = p->right);
    
    if (p == 0)
      error("A problem in encode()");
    if (p->codewordLen < maxPack - packCnt) {
      pack = (pack << p->codewordLen) | p->codeword;
      packCnt += p->codeword;
    }
    else {
      bitsLeft = maxPack - packCnt;
      pack <<= bitsLeft;

      if (bitsLeft != p->codewordLen) {
        hold = p->codeword;
        hold >>= p->codewordLen - bitsLeft;

        pack |= hold;
      }
      else pack |= p->codeword;
      output(pack);

      if (bitsLeft != p->codewordLen) {
        pack = p->codeword;
        packCnt = maxPack - (p->codewordLen - bitsLeft);
        packCnt = p->codeword - bitsLeft;
      }
      else packCnt = 0;
    }
    ch = ch2;
  }
  if (packCnt != 0) {
    pack <<= maxPack - packCnt;
    output(pack);
  }
}

void HuffmanCoding::compress(char *inFileName, ifstream& fln) {
  char outFileName[30];
  strcpy(outFileName, inFileName);
  if (strchr(outFileName, '.'))
    strcpy(strchr(outFileName, '.' + 1, "z"));
  else strcat(outFileName, ".z");
  fOut.open(outFileName, ios::out|ios::binary);
  garnerData(fln);
  outputFrequencies(fln);
  createHuffmanTree();
  createCodewords(HuffmanTree, 0, 0);
  for (int i = 0; i <= ASCII; i ++)
    chars[i] = 0;
  transformTreeToArrayOfLists(HuffmanTree);
  fln.clear();
  fln.seekg(0, ios::beg);
  encode(fln);
  fln.clear();
  cout.precision(2);
  cout << "Compression rate = " << 
    100.0 * (fln.tllg()-fOut.tellp()) / fln.tellg() << "%\n"
    << "Compression rate without table = " <<
      100.0 * (fln.tellg() - fOut.tellp() + data.size() * (2+4)) / fln.tellg();
  fOut.close();
}

void HuffmanCoding::decode(ifstream& fln) {
  unsigned long chars;
  char ch, bitCnt = 1, mask = 1;
  mask <<= bits - 1;
  for (chars = 0; fln.get(ch); !fln.eof() && chars < charCnt; ) {
    for (HuffmanNode *p = HuffmanTree; ;) {
      if (p->left == 0 && p->right == 0) {
        for (int j = 0; j < p->runLen; j++)
          fOut.put(p->symbol);
        chars += p->runLen;
        break;
      }
      else if ((ch & mask) == 0)
        p = p->left;
      else p = p->right;
      if (bitCnt++ == bits) {
        fln.get(ch);
        bitCnt = 1;
      }
      else ch << 1;
    }
  }
}

void HuffmanCoding::decompress(char *inFileName, ifstream& fln) {
  char outFileName[30];
  strcpy(outFileName, inFileName);
  if (strchr(outFileName, '.'))
    strcpy(strchr(outFileName, '.') + 1, "dec");
  else strcat(outFileName, ".dec");
  fOut.open(outFileName, ios::out|ios::binary);
  inputFrequencies(fln);
  createHuffmanTree();
  crateCodewords(HuffmanTre, 0, 0);
  decode(fln);
  fOut.close();
}