#ifndef _SPARSE_MATRIX_H
#define _SPARSE_MATRIX_H

#include <ostream>

class Matrix {
  int rows,columns; //Number of rows (height) and columns (width)
  int *offset;      //Offsets of first non-zero element in each row
  int *size;        //Sizes of sub-rows containing first & last non-zero element
  double **data;    //Pointers to dynamically allocated sub-row memory

  void deepCopy(const Matrix &copy);

 public:

  Matrix(int rows,int columns);
  Matrix(const Matrix &copy) {deepCopy(copy);}
 ~Matrix() {clear();}

  void operator =(const Matrix &copy) {clear(); deepCopy(copy);}

  void clear();
  int memoryUsed() const;

  double get(int row,int column) const;
  void set(int row,int column,double value);

  Matrix operator - () const;                               //Unary - (negation)
  Matrix operator + (const Matrix &m) const;                //Addition
  Matrix operator - (const Matrix &m) {return *this+(-m);}  //Subtraction
  Matrix operator * (double scalar) const;                  //Scalar multiply

  friend std::ostream& operator << (std::ostream &,const Matrix &);
};

inline Matrix operator * (double scalar,const Matrix &m) {return m*scalar;}

#endif
