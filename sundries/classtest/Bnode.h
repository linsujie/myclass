#ifndef _BNODE_H
#define _BNODE_H
#include<iostream>
#include<iomanip>
#include<cmath>

using namespace std;

template<class T> class Btree;

template<class T>
class Bnode {
  T content;
  int mult;
  int lhigh, rhigh, high;
  bool is_lleaf;
  Bnode *lleaf, *rleaf, *parent;

  Bnode(): lleaf(0), rleaf(0), parent(0), mult(1), lhigh(0), rhigh(0), high(0) {}
  Bnode(const T &content_): lleaf(0), rleaf(0), parent(0), mult(1), content(content_), lhigh(0), rhigh(0), high(0) {}

  T create_lleaf(const T &content_) {
    lleaf = new Bnode<T>(content_);
    lleaf->parent = this;
    lleaf->is_lleaf = true;
    lhigh++;
    high++;
    return content_;
  }
  T create_rleaf(const T &content_) {
    rleaf = new Bnode<T>(content_);
    rleaf->parent = this;
    rleaf->is_lleaf = false;
    rhigh++;
    high++;
    return content_;
  }

  Bnode <T> *grandpa() const {
    if (parent && parent->parent)
      return parent->parent;

    else
      return 0;
  }

  Bnode <T> *greate_grandpa() const {
    if (parent && parent->parent && parent->parent->parent)
      return parent->parent->parent;

    else
      return 0;
  }

  bool one_son() const {
    if ((lleaf && !rleaf) || (!lleaf && rleaf))
      return true;

    else
      return false;
  }

  Bnode <T> *son() const {
    if (lleaf)
      return lleaf;

    else
      return rleaf;
  }

  bool nonzero(vector <const Bnode <T> *> current) const {
    bool mark = false;

    for (unsigned i = 0; i < current.size(); i++) {
      if (current[i] != 0) {
        mark = true;
        break;
      }
    }

    return mark;
  }

#define PUTS(item) cout<<setw(width)<<setfill('0')<<item
#define space(tab) cout<<setw(tab)<<setfill(' ')<<""
  int print(int pflag) const {
    vector  <const Bnode <T> *> current, next;
    current.push_back(this);
    int nzero, nnonzero;
    int htmp = high, width = 5;

    while (nonzero(current)) {
      unsigned tab = (htmp <= 0) ? 0 : pow(2, htmp - 1) * (width + 1) - width / 2 - 1;

      next.clear();
      nzero = 0;
      nnonzero = 0;

      for (unsigned i = 0; i < current.size(); i++) {
        if (pflag != STATIC) space(tab);

        if (current[i]) {
          switch (pflag) {
          case CONTENT:
            PUTS(current[i]->content);
            break;

          case LEFT_H:
            PUTS(current[i]->lhigh);
            break;

          case RIGHE_H:
            PUTS(current[i]->rhigh);
            break;

          case HEIGHT:
            PUTS(current[i]->high);
            break;

          case MULTI:
            PUTS(current[i]->mult);
            break;
          }

          next.push_back(current[i]->lleaf);
          next.push_back(current[i]->rleaf);
          nnonzero++;

        } else {
          //space(width);
          if (pflag != STATIC) PUTS("0");

          next.push_back(0);
          next.push_back(0);
          nzero++;
        }

        if (pflag != STATIC) space(tab + 1);
      }

      if (pflag == STATIC)
        cout << nzero << " " << nnonzero;

      cout << endl;

      current = next;
      htmp--;
    }

    return 0;
  }

public:
  enum print_set {CONTENT, LEFT_H, RIGHE_H, HEIGHT, MULTI, STATIC};
  friend class Btree<T>;
};


#endif // for #ifndef _BNODE_H
