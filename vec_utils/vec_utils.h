#ifndef _VEC_UTILS_H
#define _VEC_UTILS_H
/*********************************************************************
  providing some utils for vector
 *********************************************************************/
#include<vector>
#include<iostream>
#include<climits>

int get_index(const std::vector <double> &vec, double x);
int get_index(const double *arr, unsigned dim, double x);

std::ostream& operator<< (std::ostream& out, const std::vector <double> &vec);
std::ostream& operator<< (std::ostream& out, const std::vector <int> &vec);
std::ostream& operator<< (std::ostream& out, const std::vector <std::vector <double> > &vec);
std::ostream& operator<< (std::ostream& out, const std::vector <std::vector <int> > &vec);

#define VECTOR_UTILS_OPERATOR(opers)\
  std::vector <double> operator opers (const std::vector <double> &lhs,const std::vector <double> &rhs);\
  std::vector <double> operator opers (const double &lhs,const std::vector <double> &rhs);\
  std::vector <double> operator opers (const std::vector <double> &lhs,const double &rhs);

VECTOR_UTILS_OPERATOR(+)
VECTOR_UTILS_OPERATOR(-)
VECTOR_UTILS_OPERATOR(*)
VECTOR_UTILS_OPERATOR( /)
#endif // for #ifndef _VEC_UTILS_H
