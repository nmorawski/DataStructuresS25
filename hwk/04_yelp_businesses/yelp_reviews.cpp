#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <sstream>
#include <list>

//#include "business.h"

int main(int argc, char* argv[]){
    // if (argc >= 5){
        
    // } else {
    //     std::cerr << "Incorrect amount of command line arguments." <<std::endl;
    //     return 1;
    // }
    double test = 4.5;
    for (int i = 1; i  < test; i++)
        std::cout << "\u2605";
    if (int(test*2) % 2 == 1)
        std::cout << "\u00BD";
    std::cout << std::endl;
    return 0;
}