#include <iostream>
using namespace std;
 
int countPaths(int x, int y){
    if (x == 0 || y == 0)
        return 1;
    return (countPaths(x-1, y) + countPaths(x, y-1));
}

int main(int argc, char *argv[]){
    int x = atoi(argv[argc-2]);
    int y = atoi(argv[argc-1]);
    cout << "Point: ("<<x<<","<<y<<")"<<endl;
    cout << "Number of Paths: " << countPaths(x, y)<<endl;
    return 0;
}