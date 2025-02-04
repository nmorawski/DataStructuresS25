#include "Matrix.h"

Matrix::Matrix() { //Default
    rows_ = 0;
    columns_ = 0;
    data=nullptr;
}

Matrix::Matrix(unsigned int rows, unsigned int cols, double fill)
    : rows_(rows), columns_(cols), data(nullptr) { //Constructor
    if (rows == 0 || cols == 0) {
        rows_ = 0;
        columns_ = 0;
        return;
    }
    data = new double*[rows_]; //Creation of 2D array
    for (unsigned int i = 0; i < rows_; i++){
        data[i] = new double[cols];
        std::fill(data[i], data[i] + columns_, fill); //Fill array
    }
}

Matrix::Matrix(const Matrix& other) //Copy Constructor
    : rows_(other.rows_), columns_(other.columns_), data(nullptr) {
    if (rows_ == 0 || columns_ == 0) return; //If copying default matrix

    data = new double*[rows_]; //Creation of 2D array
    for (unsigned int i = 0; i < rows_; i++){
        data[i] = new double[columns_];
        for (unsigned int j = 0; j < columns_; j++)
            data[i][j] = other.data[i][j]; //Fill array
    }
}

Matrix::~Matrix(){ //Destructor
    if (data) { //If data exists
        for (unsigned int i = 0; i < rows_; i++){ //Delete row of array
            if (data[i])
                delete [] data[i];
        }
        delete [] data;
    }
}

void Matrix::clear(){ //Similar to destructor, but also converts matrix back to default
    if (data) {
        for (unsigned int i = 0; i < rows_; i++)
            delete[] data[i];
        delete[] data;
        data = nullptr;
    }
    rows_ = 0;
    columns_ = 0;
}

bool Matrix::set(const unsigned int row, const unsigned int col, double val){
    if (row < rows_ && col < columns_){ //Check in bounds
        data[row][col] = val;
        return true;
    }
    return false;
}

bool Matrix::get(const unsigned int row, const unsigned int col, double &val){
    if (row < rows_ && col < columns_){ //Check in bounds
        val = data[row][col];
        return true;
    }
    return false;
}

Matrix& Matrix::operator=(const Matrix& other) { //Assignment operator
    if (this == &other) return *this;  //Prevent self-assignment

    clear(); //Clear the matrix of data ahead of being assigned

    rows_ = other.rows_; //operates almost exactly like copy constructor
    columns_ = other.columns_;
    data = new double*[rows_];
    for (unsigned int i = 0; i < rows_; i++) {
        data[i] = new double[columns_];
        for (unsigned int j = 0; j < columns_; j++)
            data[i][j] = other.data[i][j];
    }
    return *this;
}

bool Matrix::operator==(const Matrix& m2){ //Check if two matrices are the same
    if (rows_ != m2.rows_ || columns_ != m2.columns_) //If dimensions differ
        return false;
    else {
        for (unsigned int i = 0; i < rows_; i++){ //Iterate over each cell, look for differences
            for (unsigned int j = 0; j < columns_; ++j){
                if (data[i][j] != m2.data[i][j])
                    return false;
            }
        }
    }
    return true;
}

// Opposite of operator==
bool Matrix::operator!=(const Matrix& m2){ //Check if two matrices are different
    if (rows_ != m2.rows_ || columns_ != m2.columns_) //If dimensions differ
        return true;
    else {
        for (unsigned int i = 0; i < rows_; i++){//Iterate over each cell, look for differences
            for (unsigned int j = 0; j < columns_; ++j){
                if (data[i][j] != m2.data[i][j])
                    return true;
            }
        }
    }
    return false;
}

// This is the overloading of the '<<' operator, which allows for customized printing to the 
// command line. Iterates cell by cell in the 2D array.
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
            if (i != m.num_rows()-1) out << std::endl; //Check if not last row of matrix
        }
        out << " ]"<< std::endl; //This is done to match the given sample outputs
    return out;
}

bool Matrix::add(const Matrix& b){ //Add matrices A & B (A+B), result saved to A
    if (rows_ != b.rows_ || columns_ != b.columns_) //Check for same dimensions
        return false;
    else {
        for (unsigned int i = 0; i < rows_; i++){
            for (unsigned int j = 0; j < columns_; ++j)
                data[i][j] += b.data[i][j];
        }
    }
    return true;
}

// Very similar to the add() function
bool Matrix::subtract(const Matrix& b){ //Subtract matrices A & B (A-B), result saved to A
    if (rows_ != b.rows_ || columns_ != b.columns_) //Check for same dimensions
        return false;
    else {
        for (unsigned int i = 0; i < rows_; i++){
            for (unsigned int j = 0; j < columns_; ++j)
                data[i][j] -= b.data[i][j];
        }
    }
    return true;
}

// Takes in a double, then iterates through each number in the array, multiplying all by the same
// coefficient.
void Matrix::multiply_by_coefficient(const double coeff){
    for (unsigned int i = 0; i < rows_; i++){
        for (unsigned int j = 0; j < columns_; ++j)
            data[i][j] *= coeff;
    }
}

// The get_row function takes in an unsigned int as the index of the desired row, and returns a 
// pointer to the desired row. Initially, I had this pointer point directly to the given row in
// the 2D array. However, due to the presence of the given delete statement (delete [] r1; 
// matrix_main.cpp:108), this was throwing memeory issues. I have changed it to now making a copy
// of the row on the heap, and then returning a pointer that points to this array
double* Matrix::get_row(const unsigned int num_row){
    double* ptr = NULL;
    if (num_row >= rows_) //If out of bounds, return null
        return ptr;
    ptr = new double[rows_];
    for (unsigned int i = 0; i < rows_; i++) //Populate
        ptr[i] = data[num_row][i];
    return ptr;
}

// The get_row function takes in an unsigned int as the index of the desired column, and returns a 
// pointer to the desired column. Due to the structure of the 2D matrix, there is no simple way to
// copy the column straight from the Matrix object, so an array must be made on the heap, which is
// then filled with the contents of the respective column from the matrix
double* Matrix::get_col(const unsigned int num_col){
    double* ptr = NULL;
    if (num_col >= columns_) //If out of bounds, return null
        return ptr;
    ptr = new double[rows_];
    for (unsigned int i = 0; i < rows_; ++i) //Populate
        ptr[i] = data[i][num_col];
    return ptr;
}

// The swap_row function takes in two unsigned integers, which represent the two indices of the 
// rows that we want to swap. To perform this feature, a temporary pointer is created and points
// to the source row. The pointer of the source row, in the matrix, is then assigned to the target
// row, whose pointer is assigned back to the source row, with the help of the presence of the temp
// pointer. Upon completion, the temp pointer is deleted, due to the pointer going out of scope,
// but the row it points to remains intact
bool Matrix::swap_row(const unsigned int source, const  unsigned int target){
    if (source >= rows_ || target >= rows_) return false; //Check if out of bounds
    else if (source == target) return true; //No need to swap if both source/target are the same

    double* temp;
    temp = data[source];
    data[source] = data[target];
    data[target] = temp;

    return true;
}

// The quarter() function creates an array of Matrix objects, with a size of 4 to represent the 
// 4 quarters/corners of the matrix. the most important part of this function is the logic behind 
// the nested for loops that are used to populate the necessary matrices. By structuring the loops
// to fill the most basic sub-matrix, the upper left quarter, we can manipulate the indices of the 
// other three matrices so they are also filled at the same time.
Matrix* Matrix::quarter(){
    Matrix* quadrants = nullptr;
    // With integer division, if the result is a decimal, it will be rounded down to the nearest
    // integer. Taking advantage of this, I add +1 before the division to help with the cases of
    // having an odd number of rows/columns
    unsigned int halfRows = (rows_+1)/ 2;
    unsigned int halfCols = (columns_+1) / 2;

    unsigned int row_check = 0; //Corner cases if any of the dimensions are equal to 1
    unsigned int col_check = 0;
    if (rows_ == 1) row_check = 1;
    if (columns_ == 1) col_check = 1;

    if (halfRows != 0 && halfCols != 0){ //Initialization/Declaration of array of 4 Matrices
        quadrants = new Matrix[4] {
            Matrix(halfRows, halfCols, 0), //UL (Upper Left)
            Matrix(halfRows, halfCols, 0), //UR (Upper Right)
            Matrix(halfRows, halfCols, 0), //LL (Lower Left)
            Matrix(halfRows, halfCols, 0)  //LR (Lower Right)
        };
        for (unsigned int i = 0; i < halfRows; i++) {
            for (unsigned int j = 0; j < halfCols; j++) {
                quadrants[0].data[i][j] = data[i][j]; //UL
                quadrants[1].data[i][j] = data[i][j+(columns_-halfCols)/2+1-col_check]; //UR
                quadrants[2].data[i][j] = data[i+(rows_-halfRows)/2+1-row_check][j]; //LL
                quadrants[3].data[i][j] = data[i+(rows_-halfRows)/2+1-row_check][j+(columns_-
                    halfCols)/2+1-col_check]; //LR
            }
        }
    }

    return quadrants;
}
// The transpose() function takes the original, 2D matrix and transposes it, where the 1st row 
// becomes the first column, 2nd row becomes 2nd column, etc. To do this, I first inversely 
// reassign the row and column counts. With these new values, I create an empty array of the
// correct size. The last key step is to iterate through the new array, populating it with the 
// numbers from the old array (note the line temp_data[i][j] = data[j][i]). Upon completion,
// delete the old array, and assign the new array to the matrix object's data.
void Matrix::transpose(){
    unsigned int temp_rows = rows_;
    rows_ = columns_;
    columns_ = temp_rows;

    double** temp_data = new double*[rows_]; //Create new array
    for (unsigned int i = 0; i < rows_; i++){
        temp_data[i] = new double[columns_];
        for (unsigned int j = 0; j < columns_; j++)
            temp_data[i][j] = data[j][i]; //Mirrored assignment
    }

    for (unsigned int i = 0; i < columns_; i++) //Destruct old array
        delete[] data[i];
    delete[] data;

    data = temp_data;
}