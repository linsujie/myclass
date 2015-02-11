#ifndef _BTREE_H
#define _BTREE_H
#include"Bnode.h"

template<class T>
class Btree {
  Bnode <T> *base;
  Bnode <T> *last;


  T search(const T &content_, Bnode <T> *node) {
    T result;

    if (content_ < node->content) {
      if (node->lleaf)
        return search(content_, node->lleaf);

      else {
        cout << "Warning::Btree::content not found" << endl;
        return content_;
      }

    } else if (content_ > node->content) {
      if (node->rleaf)
        return search(content_, node->rleaf);

      else {
        cout << "Warning::Btree::content not found" << endl;
        return content_;
      }

    } else
      return content_;
  }

  T hit_step(const T &content_, Bnode <T> *node, int todo) {
    switch (todo) {
    case 0:
      node->mult++;
      size++;
      last = node;
      return content_;
    }
  }

  int after_ins(Bnode <T> *node, bool froml) {
    if (froml) {

      node->lhigh = node->lleaf->high + 1;
      node->high = fmax(node->lhigh, node->rhigh);

      //cout<<"balance factor is: "<<node->lhigh<<" "<<node->rhigh<<" "<<node->content<<endl;
      if (node->lhigh - node->rhigh >= 2)
        right_roatate(node);

    } else {

      node->rhigh = node->rleaf->high + 1;
      node->high = fmax(node->lhigh, node->rhigh);

      if (node->rhigh - node->lhigh >= 2)
        left_roatate(node);
    }

    return 0;
  }

  int seek(const T &content_, Bnode <T> *node) {
    T result;

    if (content_ < node->content) {
      if (node->lleaf) {
        seek(content_, node->lleaf);
        after_ins(node, true);

      } else {
        node->create_lleaf(content_);
        size++;
        last = node->lleaf;
      }

    } else if (content_ > node->content) {
      if (node->rleaf) {
        seek(content_, node->rleaf);
        after_ins(node, false);

      } else {
        node->create_rleaf(content_);
        size++;
        last = node->rleaf;
      }

    } else {
      node->mult++;
      size++;
      last = node;
    }

    return 0;
  }

  int insert(const T &content_) {
    if (base)
      seek(content_, base);

    else {
      base = new Bnode <T> (content_);
      size++;
      height = 1;
      last = base;
    }

    return 0;
  }

#define ROTATE(rrota,left,right)\
  int rrota(Bnode <T> *grandpa){\
    Bnode <T> *father=grandpa->left##leaf,\
                      *ad_son=father->right##leaf,\
                              *greate_grand=grandpa->parent;\
    \
    if(greate_grand)\
      if(greate_grand->left##leaf==grandpa)\
        greate_grand->left##leaf=father;\
      else\
        greate_grand->right##leaf=father;\
    \
    father->parent=greate_grand;\
    father->right##leaf=grandpa;\
    grandpa->parent=father;\
    grandpa->left##leaf=ad_son;\
    if(ad_son)\
      ad_son->parent=grandpa;\
    \
    if(ad_son)\
      grandpa->left##high=ad_son->high+1;\
    else\
      grandpa->left##high=0;\
    grandpa->high=fmax(grandpa->lhigh,grandpa->rhigh);\
    father->right##high=grandpa->high+1;\
    father->high=fmax(father->lhigh,father->rhigh);\
    \
    if(grandpa==base) base=father;\
    return 0;\
  }

  ROTATE(right_roatate, l, r)
  ROTATE(left_roatate, r, l)

  int list(Bnode <T> *node) {
    if (node) {
      list(node->lleaf);
      cout << node->content << " ";
      list(node->rleaf);
    }

    return 0;
  }

public:
  int size;
  int height;

  Btree(): base(0), size(0), height(0) {};
  Btree(const T &content_): size(1), height(1) {
    base = new Bnode <T> (content_);
  }

  T search(const T &content_) {
    if (base)
      return search(content_, base);

    else
      cout << "Warning::Btree::you are searching in an empty tree" << endl;

    return content_;
  }

  int add(const T &content_) {
    insert(content_);
    height = base->high + 1;
    return 0;
  }

  int print() {
    return print(Bnode<T>::CONTENT);
  }

  int print(int pflag) {
    if (base)
      base->print(pflag);

    else
      cout << "Warning::Btree::you are printing an empty tree" << endl;

    return 0;
  }

  int list() {
    list(base);
    cout << endl;
    return 0;
  }

};
#endif // for #ifndef _BTREE_H
