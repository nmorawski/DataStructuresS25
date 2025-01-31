#ifndef __MATRIX_H
#define __MATRIX_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>

//If either dimension is 0, the resulting matrix should be of size 0×0.

/*
*/

class Matrix {
public:
Matrix(); //Default constructor 
Matrix(unsigned int rows, unsigned int cols, double fill);
Matrix(const Matrix& other); //Copy constructor
//~Matrix(); //Destructor
//void clear(); //change rows/cols to zero, deallocate memory (replace destructor?)

// ACCESSORS
unsigned int num_rows() const { return rows_; }
unsigned int num_cols() const { return columns_; }
bool get(unsigned int row, unsigned int col, double &val);//return T/F, value found saved to val
//double* get_row(unsigned int num_row); //If out of bounds, return pointer set to NULL
//double* get_col(unsigned int num_col);
// MODIFIERS
bool set(unsigned int row, unsigned int col, double val); //return T/F, value found saved as val
//void multiply_by_coefficient(const double &coeff);
//bool swap_row(unsigned int source, unsigned int target);
//bool add(const Matrix& b); //check dimensions the same, return T/F based on result
//bool subtract(const Matrix& b); //^^^^^^^

//void transpose(); //transpose matrix
/*
The final method we expect you to implement, quarter(), is not a traditional matrix operation. 
The method takes no arguments and returns a Matrix* containing four new Matrix elements in order:
an Upper Left (UL) quadrant, an Upper Right (UR) quadrant, a Lower Left (LL) quadrant, and finally
a Lower Right (LR) quadrant. All four quadrants should be the same size. Remember that when a 
function ends all local variables go out of scope and are destroyed, so you will need to be particularly 
careful about how you construct and return the quadrants
*/
//Matrix* quarter();

bool operator==(const Matrix& m2);
bool operator!=(const Matrix& m2);
//std::ostream& operator<<(std::ostream& out, const Matrix& m);

private: // REPRESENTATION (member variables)
double** data;
unsigned int rows_;
unsigned int columns_;
};

#endif