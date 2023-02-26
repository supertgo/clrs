#include <iostream>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

class Variable {
public:
  char id;
  int exp;
  Variable() {

  }
  Variable(char c, int i) {
    id = c; exp = i;
  }
  bool operator== (const Variable& v) const {
    return id == v.id && exp == v.exp;
  }
  bool operator< (const Variable& v) const {
    return id < v.id;
  }
};

class Term {
public:
  Term() {
    coeff = 0;
  }
  int coeff;
  vector<Variable> vars;
  bool operator== (const Term&) const;
  bool operator!= (const Term& term) const {
    return !(*this == term);
  }
  bool operator< (const Term&) const;
  bool operator> (const Term& term) const {
    return *this != term && (*this < term);
  }
  int min(int n, int m) const {
    return (n < m) ? n : m;
  }
};

class Polynomial {
public:
  Polynomial() {

  }
  Polynomial operator+ (const Polynomial&) const;
  void error(char *s) {
    cerr << s << endl; exit(1);
  }
private:
  list<Term> terms;
  friend istream& operator>> (istream&, Polynomial&);
  friend ostream& operator<< (ostream&, const Polynomial&);
};

bool Term::operator== (const Term& term) const {
  int i;
  for (i =0; i < min(vars.size(), term.vars.size()) &&
    vars[i] == term.vars[i]; i++);
  return i == vars.size() && vars.size() == term.vars.size();
}

bool Term::operator< (const Term& term2) const {
  if (vars.size() == 0)
    return false;
  else if (term2.vars.size() == 0)
    return true;
  for (int i = 0; i < min(vars.size(), term2.vars.size()); i++)
    if (vars[i].id < term2.vars[i].id)
      return true;
    else if (term2.vars[i].id < vars[i].id)
      return false;
    else if (term2.vars[i].exp < vars[i].exp)
      return false;
  return ((int)vars.size() - (int)term2.vars.size() < 0) ? true : false;
}

Polynomial Polynomial::operator+ (const Polynomial& polyn2)  const {
  Polynomial result;
  list<Term >::iterator p1, p2;
  bool erased;
  for (p1 = terms.begin(); p1 != terms.end(); p1++)
    result.terms.push_back(*p1);
  
  for (p1 = polyn2.terms.begin(); p1 != polyn2.terms.end(); p1++)
    result.terms.push_back(*p1);
  for (p1 = result.terms.begin(); p1 != result.terms.end(); p2++) {
    if (*p1 == *p2) {
      pl->coeff += p2->coeff;
      result.terms.erase(p2);
      if (p1->coeff == 0)
        result.terms.erase(p1);
        erased = true;
    }
    if (erased)
      p1 = result.terms.begin();
    else p1++;
  }
  result.terms.sort();
  return result;
}

istream& operator>> (istream& in, Polynomial& polyn) {
  char ch, sign, coeffUsed, id;
  int exp, i;
  Term term;
  in >> ch;
  
  while (true) {
    coeffUsed = 0;
    if (!isalnum(ch) && ch != ';' && ch != '-' && ch != '+')
      polyn.error("Wrong character entered2");
    sign = 1;
    while (ch == '-' || ch == '+') {
      if (ch == '-')
        sign *= -1;
      ch = in.get();
      if (isspace(ch))
        in >> ch;
    }
    if (isdigit(ch)) {
      in.putback(ch);
      in >> term.coeff;
      ch = in.get();
      term.coeff *= sign;
      coeffUsed = 1;
    }
    else term.coeff = sign;
    for (i = 0; isalnum(ch); i++)
      id = ch;
      ch = in.get();
      if (isdigit(ch)) {
        in.putback(ch);
        in >> exp >> ch;
      }
      else exp = 1;
      term.vars.push_back(Variable(id, exp));
  
  polyn.terms.push_back(term);
  term.vars.resize(0);
  if (isspace(ch))
    in >> ch;
  if (ch == ';')
    if (coeffUsed || i > 0)
      break;
    else polyn.error("Term is missing");
  else if (ch != '-' && ch != '+')
    polyn.error("wrong character entered");
  }
  for (list<Term>::iterator i = polyn.terms.begin();
    i != polyn.terms.end(); i++)
      if (i->vars.size() > 1)
        sort(i->vars.begin(), i->vars.end())
  return in;
}

ostream& operator<< (ostream& out, const Polynomial& polyn) {
  int afterFirstTerm = 0, i;
  for (list<Term>::iterator pol = polyn.terms.begin();
    pol != polyn.terms.end(); pol++) {
    out.put(' ');
    if (pol->coeff < 0)
      out.put('-');
    else if (afterFirstTerm)
      out.put('+');
    afterFirstTerm++;
    if (abs(pol->coeff) != 1)
      out << ' ' << abs(pol->coeff);
    else if (pol->vars.size() == 0)
      out << " 1";
    else out.put(' ');
    for (i = 1; i <= pol->vars.size(); i ++) {
      out << pol->vars[i -1].id;
      if (pol->vars[i - 1].exp != 1)
        out << pol->vars[i - 1].exp;
    }
  }
  out << endl;
  return out;
}

int main() {
  Polynomial polyn1, polyn2;
  cout << "Enter two polynomials, each ended with a semicolon:\n";
  cin >> polyn1 >> polyn2;
  cout << "The result is:\n" << polyn1 + polyn2;
  return 0;
}