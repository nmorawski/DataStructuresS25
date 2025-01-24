#ifndef __LOCATION_H
#define __LOCATION_H

#include <string>

class Location {
public:
Location(); //Default constructor 
Location(std::string loc_name, double loc_lat, double loc_long); //Used for rider location
Location(double loc_lat, double loc_long); //Used for driver location
// ACCESSORS
std::string getLocationName() const { return name_; }
double getLatitude() const { return latitude_; }
double getLongitude() const { return longitude_; }
// MODIFIERS
void setLocationName(std::string &name){ name_ = name; }
void setLatitude(double latitude){ latitude_ = latitude; }
void setLongitude(double longitude){ longitude_ = longitude; }

private: // REPRESENTATION (member variables)
std::string name_;
double latitude_;
double longitude_;

};
//Helper functions
double calculateDistance(const Location &loc1, const Location &loc2);
std::string remove_zeroes(double d);

#endif