#ifndef __MATRIX_H
#define __MATRIX_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <math.h>
#include <cassert>

class Matrix {
public:
Matrix(); //Default constructor 
Matrix(unsigned int rows, unsigned int cols, double fill);
Matrix(const Matrix& other); //Copy constructor
~Matrix(); //Destructor
void clear(); //change rows/cols to zero, deallocate memory

// ACCESSORS
unsigned int num_rows() const { return rows_; }
unsigned int num_cols() const { return columns_; }
bool get(const unsigned int row, const unsigned int col, double &val);//return T/F, value found saved to val
double* get_row(const unsigned int num_row); //If out of bounds, return pointer set to NULL
double* get_col(const unsigned int num_col);
// MODIFIERS
bool set(const unsigned int row, const unsigned int col, double val); //return T/F, value found saved as val
void multiply_by_coefficient(const double coeff);
bool swap_row(const unsigned int source, const unsigned int target);
bool add(const Matrix& b); //check dimensions the same, return T/F based on result
bool subtract(const Matrix& b); //^^^^^^^

void transpose(); //transpose matrix
Matrix* quarter(); 

Matrix& operator=(const Matrix& other);
bool operator==(const Matrix& m2);
bool operator!=(const Matrix& m2);

private: // REPRESENTATION (member variables)
unsigned int rows_;
unsigned int columns_;
double** data;
};

std::ostream& operator<<(std::ostream& out, Matrix& m);

#endif