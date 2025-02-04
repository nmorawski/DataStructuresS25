HOMEWORK 3: MATRIX CLASS


NAME:  Nicolas Morawski


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

cppreference.com
TA Alek

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  8-10



ORDER NOTATION:
For each of the functions below, write the order notation O().
Write each answer in terms of m = the number of rows and n = the
number of columns.  You should assume that calling new [] or delete []
on an array will take time proportional to the number of elements in
the array.

get O(1)

set O(1)

num_rows O(1)

get_column O(2m) 

operator<< O(mn)

quarter O(n+mn) = O(mn) (n for declaring array on the heap, mn for storing appropriate values)

operator== best case O(1), avg. case O(mn)

operator!= best case O(1), avg. case O(mn)

swap_rows O(1)

rref (provided in matrix_main.cpp) O(mn+m(mn+m+n+mn)) = O(2m^2n+2mn+m^2) = O(2m^2n)



TESTING & DEBUGGING STRATEGY: 
Discuss your strategy for testing & debugging your program.  
What tools did you use (gdb/lldb/Visual Studio debugger,
Valgrind/Dr. Memory, std::cout & print, etc.)?  How did you test the
"corner cases" of your Matrix class design & implementation?

I was actively using Valgrind to debug my code. Upon my initial completion of the simple tests,
I ran Valgrind again and found out I had some memory issues, which was due to me calling a double
delete() somewhere. Remedying this issue took most of the time for me, as I could not figure out
why the issue was occuring. To fix the issue, I extensively used the std::cout and the overloaded
'<<' operator, as well as slowly and methodically stepping through each test in matrix_main.cpp,
making sure to note that the number of allocs and frees on the heap (according to the memory
debugger) remained the same.



