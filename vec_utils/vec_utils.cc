#include<cstdlib>
#include<cstdarg>
#include"vec_utils.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ostream;

int get_index(const vector <double> &vec, double x) {
  int ind;

  if (vec.size() == 0 || x > vec[vec.size() - 1]) return vec.size() - 1;

  else if (x < vec[0]) return -1;

  int lmark = 0,
      hmark = vec.size() - 1;

  while (hmark - lmark > 1) {
    ind = (lmark + hmark) / 2;

    if (vec[ind] > x)
      hmark = ind;

    else
      lmark = ind;
  }

  ind = lmark;
  return ind;
}

int get_index(const double *arr, unsigned dim, double x) {
  int ind;

  if (dim == 0 || x > arr[dim - 1]) return dim - 1;

  else if (x < arr[0]) return -1;

  int lmark = 0,
      hmark = dim - 1;

  while (hmark - lmark > 1) {
    ind = (lmark + hmark) / 2;

    if (arr[ind] > x)
      hmark = ind;

    else
      lmark = ind;
  }

  ind = lmark;
  return ind;
}

#define OUTSTREAM(type)\
  ostream& operator<< (ostream& out,const vector <type> &vec){\
    out<<vec[0];\
    for(unsigned i=1;i<vec.size();i++) out<<"\t"<<vec[i];\
    return out;\
  }\
ostream& operator<< (ostream& out, const vector <vector <type> > &vec) {\
  for(unsigned i = 0; i < vec.size(); i++) out << vec[i] << endl;\
  return out;\
}

OUTSTREAM(double)
OUTSTREAM(int)
OUTSTREAM(string)

#define NOT_DIVIDE(lhs,oper,rhs) res.push_back( lhs oper rhs )
#define IS_DIVIDE(lhs,oper,rhs)\
  if(rhs==0) res.push_back(0);\
  else res.push_back( lhs oper rhs )

#define OPERFUNC(opers,div_or_not)\
  vector <double> operator opers (const double &lhs, const vector <double> &rhs){\
    vector <double> res;\
    for(unsigned i=0;i<rhs.size();i++){\
      div_or_not(lhs,opers,rhs[i]);\
    }\
    return res;\
  }\
  vector <double> operator opers (const vector <double> &lhs, const double &rhs){\
    vector <double> res;\
    for(unsigned i=0;i<lhs.size();i++){\
      div_or_not(lhs[i],opers,rhs);\
    }\
    return res;\
  }\
  vector <double> operator opers (const vector <double> &lhs,const vector <double> &rhs){\
    if(lhs.size()!=rhs.size()){\
      cout<<"ERROR::std::vector::"<<#opers<<":The size of the vector in the left hand side is diffrent from the right hand side"<<endl;\
      exit(1);\
    }\
    vector <double> res;\
    for(unsigned i=0;i<lhs.size();i++){\
      div_or_not(lhs[i],opers,rhs[i]);\
    }\
    return res;\
  }\
   
OPERFUNC(+, NOT_DIVIDE)
OPERFUNC(-, NOT_DIVIDE)
OPERFUNC(*, NOT_DIVIDE)
OPERFUNC( /, IS_DIVIDE)
