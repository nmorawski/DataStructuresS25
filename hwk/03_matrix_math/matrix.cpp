#include "matrix.h"

Matrix::Matrix() {
    rows_ = 0;
    columns_ = 0;
}

Matrix::Matrix(unsigned int rows, unsigned int cols, double fill){
    if (rows == 0 || cols == 0)
        Matrix();
    else {
        rows_ = rows;
        columns_ = cols;
        data = new double*[rows];
        for (unsigned int i = 0; i < rows; i++){
            data[i] = new double[cols];
            double *p;
            for (p=data[i]; p < data[i]+cols; ++p)
                *p = fill;
        }
    }
}
/*
const int n = 10;
double a[n];
double *p;
for ( p=a; p<a+n; ++p ){
    *p = sqrt( p-a );
}*/

Matrix::Matrix(const Matrix& other){
    rows_ = other.rows_;
    columns_ = other.columns_;
    data = new double*[rows_];
    for (unsigned int i = 0; i < rows_; i++){
        data[i] = new double[columns_];
        for (unsigned int j = 0; j < columns_; ++j)
            data[i][j] = other.data[i][j];
    }
}

bool Matrix::set(unsigned int row, unsigned int col, double val){
    if (row < rows_ && col < columns_){
        data[row][col] = val;
        return true;
    }
    return false;
}

bool Matrix::get(unsigned int row, unsigned int col, double &val){
    if (row < rows_ && col < columns_){
        val = data[row][col];
        return true;
    }
    return false;
}

bool Matrix::operator==(const Matrix& m2){
    if (rows_ != m2.rows_ || columns_ != m2.columns_)
        return false;
    else {
        for (unsigned int i = 0; i < rows_; i++){
            for (unsigned int j = 0; j < columns_; ++j){
                if (data[i][j] != m2.data[i][j])
                    return false;
            }
        }
    }
    return true;
}
/*std::ostream &Matrix::operator<<(std::ostream &out, const Matrix &m)
{
    // TODO: insert return statement here
}*/