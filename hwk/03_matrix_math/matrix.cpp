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

Matrix& Matrix::operator=(const Matrix& other) {
        data = other.data; // Copy data
        rows_ = other.rows_;
        columns_ = other.columns_;
        return *this;
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

bool Matrix::operator!=(const Matrix& m2){
    if (rows_ != m2.rows_ || columns_ != m2.columns_)
        return true;
    else {
        for (unsigned int i = 0; i < rows_; i++){
            for (unsigned int j = 0; j < columns_; ++j){
                if (data[i][j] != m2.data[i][j])
                    return true;
            }
        }
    }
    return false;
}
// 0 x 0 matrix: [ ]
std::ostream& operator<<(std::ostream &out, Matrix &m){
    out << std::endl << m.num_rows() << " x "<< m.num_cols() << " matrix:" << std::endl;
    out << "[";
    for (unsigned int i = 0; i < m.num_rows(); i++){
            if (i > 0) out << " ";
            for (unsigned int j = 0; j < m.num_cols(); ++j){
                double v;
                m.get(i,j,v);
                out << " " << v;
            }
            if (i != m.num_rows()-1) out << std::endl;
        }
        out << " ]"<< std::endl;
    return out;
}

bool Matrix::add(const Matrix& b){
    if (rows_ != b.rows_ || columns_ != b.columns_)
        return false;
    else {
        for (unsigned int i = 0; i < rows_; i++){
            for (unsigned int j = 0; j < columns_; ++j)
                data[i][j] += b.data[i][j];
        }
    }
    return true;
}

bool Matrix::subtract(const Matrix& b){
    if (rows_ != b.rows_ || columns_ != b.columns_)
        return false;
    else {
        for (unsigned int i = 0; i < rows_; i++){
            for (unsigned int j = 0; j < columns_; ++j)
                data[i][j] -= b.data[i][j];
        }
    }
    return true;
}

void Matrix::multiply_by_coefficient(const double &coeff){
    for (unsigned int i = 0; i < rows_; i++){
        for (unsigned int j = 0; j < columns_; ++j)
            data[i][j] *= coeff;
    }
}

double* Matrix::get_row(unsigned int num_row){
    double* ptr = NULL;
    if (num_row >= rows_)
        return ptr;
    ptr = data[num_row];
    return ptr;
}

bool Matrix::swap_row(unsigned int source, unsigned int target){
    if (source >= rows_ || target >= rows_) return false;
    else if (source == target) return true;

    double* temp;
    temp = data[source];
    data[source] = data[target];
    data[target] = temp;

    delete temp;
    return true;
}