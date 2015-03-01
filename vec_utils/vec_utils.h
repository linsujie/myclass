#ifndef _VEC_UTILS_H
#define _VEC_UTILS_H
/*********************************************************************
  providing some utils for vector
 *********************************************************************/
#include<vector>
#include<iostream>
#include<cstring>
#include<climits>

int get_index(const std::vector <double> &vec, double x);
int get_index(const double *arr, unsigned dim, double x);

#define VECTOR_UTILS_OSTREAM(types)\
  std::ostream& operator<< (std::ostream& out, const std::vector <types> &vec);\
  std::ostream& operator<< (std::ostream& out, const std::vector <std::vector <types> > &vec);

VECTOR_UTILS_OSTREAM(int)
VECTOR_UTILS_OSTREAM(double)
VECTOR_UTILS_OSTREAM(std::string)

#define VECTOR_UTILS_OPERATOR(opers)\
  std::vector <double> operator opers (const std::vector <double> &lhs,const std::vector <double> &rhs);\
  std::vector <double> operator opers (const double &lhs,const std::vector <double> &rhs);\
  std::vector <double> operator opers (const std::vector <double> &lhs,const double &rhs);

VECTOR_UTILS_OPERATOR(+)
VECTOR_UTILS_OPERATOR(-)
VECTOR_UTILS_OPERATOR(*)
VECTOR_UTILS_OPERATOR( /)
#endif // for #ifndef _VEC_UTILS_H
