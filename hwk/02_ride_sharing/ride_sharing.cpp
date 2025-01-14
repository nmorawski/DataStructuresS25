#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Used for std::stoi
#include <sstream> //Includes stringstream
#include <cctype> //Used for isdigit()

#include "location.h"
#include "rider.h"
#include "driver.h"

//nyride.exe drivers.txt riders.txt output0.txt output1.txt output2.txt phoneNumber request (8)
//nyride.exe drivers.txt riders.txt output0.txt output1.txt output2.txt phoneNumber cancel (8)
//nyride.exe drivers.txt out_drivers.txt remove 3.5 (removing drivers under specified rating) (5)
//nyride.exe drivers.txt riders.txt search all (5)
//nyride.exe drivers.txt riders.txt search rider_id (5)
//nyride.exe drivers.txt riders.txt search driver_id (all 3 ride monitoring) (5)

int main(int argc, char* argv[]){
    if (argc == 8){
        std::ifstream driver_list(argv[1]);
        std::ifstream rider_list(argv[2]);
        std::ofstream messages(argv[3]);
        std::ofstream updated_drivers(argv[4]);
        std::ofstream updated_riders(argv[5]);

    } else if (argc == 5){

    } else {

    }
    std::vector<Rider> riders;
    std::vector<Driver> drivers;


    return 0;
}