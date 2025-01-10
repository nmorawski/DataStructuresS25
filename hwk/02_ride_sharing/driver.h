#ifndef __DRIVER_H
#define __DRIVER_H

#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include "rider.h"

//Sandra Huang Female 25 853-977-5304 3.1 40.4269 -73.0753 Standard On_the_way_to_pickup Michael Richard 445-915-1645
//William David Male 37 324-571-7028 3.8 40.2445 -73.5073 Premium Available null null null
/*
Driver's first name
- Driver's last name
- Driver's gender
- Driver's age
- Driver's phone number
- Driver's rating
- Driver's current latitude
- Driver's current longitude
- Driver's vehicle type
- Driver's current state
- Rider's first name
- Rider's last name
- Rider's phone number
*/
class Driver {
public:
Driver(std::string const name, std::string  const &phone_number, double &rating, 
    std::vector<double> location, std::string &vehicle, std::string state, Rider rider);
Driver(std::string const name, std::string  const &phone_number);
// ACCESSORS
Rider& getRider() const { return rider_; }
std::string getName() const { return name_; }
std::string getPhone() const { return phone_; }
double getRating() const { return rating_; }
std::vector<double> getLocation() const { return location_; }
std::string getVehicle() const { return vehicle_; }
std::string getState() const { return state_; }
// MODIFIERS
void setRider(Rider rider);
void setState(std::string state);
void setLocation(std::vector<double> location);

private: // REPRESENTATION (member variables)
Rider rider_;
std::string name_;
std::string phone_;
double rating_;
std::vector<double> location_;
std::string vehicle_;
std::string state_;

};

#endif