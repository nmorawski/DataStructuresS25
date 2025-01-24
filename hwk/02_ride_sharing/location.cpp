#define _USE_MATH_DEFINES //Needed for cmath's M_PI

#include <iostream>
#include <string>
#include <cmath>
#include "location.h"

Location::Location(){
    name_ = "null";
    latitude_ = 0;
    longitude_ = 0;
}

Location::Location(std::string loc_name, double loc_lat, double loc_long) {
    name_ = loc_name;
    latitude_ = loc_lat;
    longitude_ = loc_long;
}

Location::Location(double loc_lat, double loc_long) {
    name_ = "null";
    latitude_ = loc_lat;
    longitude_ = loc_long;
}
// Helper function for converting doubles into strings and removes trailing
// zeroes. Used by both the Rider and Driver class, so I implemented here.
std::string remove_zeroes(double d){
    std::string val = std::to_string(d);
    val = val.erase(val.find_last_not_of('0') + 1);
    if (val.back() == '.') { val.pop_back();}
    return val;
}

//Calculate the distance between two coordinates using Haversine formula (given function)
double calculateDistance(const Location &loc1, const Location &loc2) {
    const double radiusOfEarth = 6371.0; //Earth's radius in kilometers

    //Convert latitude and longitude from degrees to radians
    double lat1 =  loc1.getLatitude() * M_PI / 180.0;
    double lon1 =  loc1.getLongitude() * M_PI / 180.0;
    double lat2 =  loc2.getLatitude() * M_PI / 180.0;
    double lon2 =  loc2.getLongitude() * M_PI / 180.0;

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