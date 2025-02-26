#include <iostream>
 
int countPaths(int x, int y){
    if (x == 0 || y == 0)
        return 1;
    return (countPaths(x-1, y) + countPaths(x, y-1));
}

int main(int argc, char *argv[]){
    int x = std::atoi(argv[argc-2]);
    int y = std::atoi(argv[argc-1]);

    std::cout << "Point: ("<<x<<","<<y<<")"<< std::endl;
    std::cout << "Number of Paths: " << countPaths(x, y)<< std::endl;
    
    return 0;
}