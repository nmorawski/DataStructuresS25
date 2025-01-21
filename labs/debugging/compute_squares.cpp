#include <iostream>
#include <fstream>
//Write a function compute_squares that takes 3 arguments: two C-style arrays 
//(not STL vectors), a and b, of unsigned integers, and an unsigned integer, n, 
//representing the size of each of the arrays. The function should square each 
//element in the first array, a, and write each result into the corresponding slot in the second array, b.
void compute_squares(unsigned int* a, unsigned int* b, const int n){
    unsigned int *p, *q;
    q = b;
    for (p=a; p<a+n; ++p,++q)
        *q = (*p)*(*p);
}

int main(){
    const int n1 = 5;
    unsigned int a1[n1] = {2,4,6,8,10};
    unsigned int b1[n1];
    compute_squares(a1,b1,n1);

    std::cout << "Example 1:" << std::endl;
    for (int i=0; i < n1;i++)
        std::cout << a1[i] << " " << b1[i] << std::endl;

    const int n2 = 8;
    unsigned int a2[n2] = {2,2,2,2,2,2,2,2};
    unsigned int b2[n2];
    compute_squares(a2,b2,n2);

    std::cout << "Example 2:" << std::endl;
    for (int i=0; i < n2;i++)
        std::cout << a2[i] << " " << b2[i] << std::endl;
        
    return 0;
}