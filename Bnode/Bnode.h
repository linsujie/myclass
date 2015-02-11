#ifndef _BNODE_H
#define _BNODE_H
template<class T> class Btree;

template<class T>
class Bnode {
  T content;
  Bnode *lleaf, *rleaf, *parent;

  Bnode(): lleaf(0), rleaf(0), parent(0) {}
  Bnode(const T &content_): lleaf(0), rleaf(0), parent(0) {
    content = content_;
  }

  int create_lleaf(const T &content_) {
    lleaf = new Bnode(content_);
    lleaf->Bnode.parent = this;
    return 0;
  }
  int create_rleaf(const T &content_) {
    rleaf = new Bnode(content_);
    rleaf->Bnode.parent = this;
    return 0;
  }

  int del_lleaf() {
    if (lleaf)
      lleaf->del_point();

    lleaf = 0;
    return 0;
  }
  int del_rleaf() {
    if (rleaf)
      rleaf->del_point();

    rleaf = 0;
    return 0;
  }
  int del_point() {
    del_lleaf();
    del_rleaf();
    delete this;
    return 0;
  }

public:
  friend class Btree<T>;
};


#endif // for #ifndef _BNODE_H
