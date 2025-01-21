#ifndef __RIDER_H
#define __RIDER_H

#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include "driver.h"
#include "location.h"

class Rider {
public:
Rider();
Rider(const std::string name, const std::string phone_number, const double rating, 
    Location pickup_loc, Location dropoff_loc, const std::string vehicle, 
    std::string state, Driver driver);
Rider(std::string name, std::string phone_number);
// ACCESSORS
Driver getDriver() const { return driver_; }
std::string getName() const { return name_; }
std::string getPhone() const { return phone_; }
double getRating() const { return rating_; }
Location getPickupLocation() const { return pickup_location_; }
Location getDropoffLocation() const { return dropoff_location_; }
std::string getVehicle() const { return vehicle_; }
std::string getState() const { return state_; }
// MODIFIERS
void setDriver(Driver driver) { driver_ = driver; }
void setState(std::string state) { state_ = state; }
void setStartLoc(Location location) { pickup_location_ = location; }
void setEndLoc(Location location) { dropoff_location_ = location; }

private: // REPRESENTATION (member variables)
Driver driver_;
Location pickup_location_;
Location dropoff_location_;
std::string name_;
std::string phone_;
double rating_;
std::string vehicle_;
std::string state_;

};

#endif