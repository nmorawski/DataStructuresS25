#ifndef __LOCATION_H
#define __LOCATION_H

class Location {
public:
Location(); //Default constructor 
Location(double loc_lat, double loc_long);
// ACCESSORS
double getLatitude() const { return latitude_; }
double getLongitude() const { return longitude_; }
// MODIFIERS
void setLatitude(double latitude){ latitude_ = latitude; }
void setLongitude(double longitude){ longitude_ = longitude; }

private: // REPRESENTATION (member variables)
double latitude_;
double longitude_;

};
//Helper functions
double calculateDistance(const Location &loc1, const Location &loc2);
std::string remove_zeroes(double d);

#endif