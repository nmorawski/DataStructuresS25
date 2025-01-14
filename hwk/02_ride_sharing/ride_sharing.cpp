#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Used for std::stoi
#include <sstream> //Includes stringstream
#include <cctype> //Used for isdigit()
#include <cmath>
#include "rider.cpp"
#include "driver.cpp"

//Calculate the distance between two coordinates using Haversine formula (given function)
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    const double radiusOfEarth = 6371.0; //Earth's radius in kilometers

    //Convert latitude and longitude from degrees to radians
    lat1 *= M_PI / 180.0;
    lon1 *= M_PI / 180.0;
    lat2 *= M_PI / 180.0;
    lon2 *= M_PI / 180.0;

    //Haversine formula
    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;
    double a = sin(dLat / 2.0) * sin(dLat / 2.0) + cos(lat1) * cos(lat2) * sin(dLon / 2.0) * sin(dLon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    //Distance in kilometers
    double distanceKM = radiusOfEarth * c;
    //Convert it to distance in miles
    double distanceMiles = distanceKM * 0.621371;

    return distanceMiles;
}

//nyride.exe drivers.txt riders.txt output0.txt output1.txt output2.txt phoneNumber request (8)
//nyride.exe drivers.txt riders.txt output0.txt output1.txt output2.txt phoneNumber cancel (8)
//nyride.exe drivers.txt out_drivers.txt remove 3.5 (removing drivers under specified rating) (5)
//nyride.exe drivers.txt riders.txt search all
//nyride.exe drivers.txt riders.txt search rider_id
//nyride.exe drivers.txt riders.txt search driver_id (all 3 ride monitoring) (5)

int main(int argc, char* argv[]){
    std::vector<Rider> riders;
    std::vector<Driver> drivers;


    return 0;
}