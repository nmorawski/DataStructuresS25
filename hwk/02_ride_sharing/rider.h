#ifndef __RIDER_H
#define __RIDER_H

#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include "driver.h"

//Driver view
//Sandra Huang Female 25 853-977-5304 3.1 40.4269 -73.0753 Standard On_the_way_to_pickup Michael Richard 445-915-1645
//William David Male 37 324-571-7028 3.8 40.2445 -73.5073 Premium Available null null null

//Rider view
//Isabella Richard Female 39 301-144-6533 3.2 Top_of_the_Rock 40.7593 -73.979 Gowanus 40.6733 -73.99 Economy Ready_to_request null null null
//Juan James Male 47 717-480-4710 3.2 Forest_Hills 40.7196 -73.8448 Park_Slope 40.6728 -73.9778 Standard Driver_on_the_way Melissa Kim 435-773-6289
/*
Rider's first name
- Rider's last name
- Rider's gender
- Rider's age
- Rider's phone number
- Rider's rating
- The name of the rider's pickup location
- The latitude of the rider's pickup location
- The longitude of the rider's pickup location
- The name of the rider's dropoff location
- The latitude of the rider's dropoff location
- The longitude of the rider's dropoff location
- Rider's vehicle preference
- Rider's current state
- Driver's first name
- Driver's last name
- Driver's phone number
*/
class Rider {
public:
Rider(std::string const name, std::string &phone_number, double &rating, 
    std::string pickup_name, std::vector<double> pickup_loc, std::string dropoff_name, 
    std::vector<double> dropoff_loc,std::string &vehicle, std::string state, Driver &driver);
Rider(std::string const name, std::string  const &phone_number);
// ACCESSORS
Driver& getDriver() const { return driver_; }
std::string getName() const { return name_; }
std::string getPhone() const { return phone_; }
double getRating() const { return rating_; }
std::vector<double> getLocation() const { return location_; }
std::string getVehicle() const { return vehicle_; }
std::string getState() const { return state_; }
// MODIFIERS
void setDriver(Driver driver);
void setState(std::string state);
void setLocation(std::vector<double> location);

private: // REPRESENTATION (member variables)
Driver driver_;
std::string name_;
std::string phone_;
double rating_;
std::vector<double> location_;
std::string vehicle_;
std::string state_;

};

#endif