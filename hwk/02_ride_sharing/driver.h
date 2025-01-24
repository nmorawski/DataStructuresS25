#ifndef __DRIVER_H
#define __DRIVER_H

#include <iostream> 
#include <fstream>
#include <string> 
#include <vector>
#include "location.h"

class Driver {
public:
Driver(const std::string fname, const std::string lname, const std::string gender, 
    const int age, const std::string phone_number, double rating, Location location, 
    std::string vehicle, std::string state, std::string r_fname, std::string r_lname, 
    std::string r_num);
// ACCESSORS
std::string getFName() const { return fname_; }
std::string getLName() const { return lname_; }
std::string getGender() const { return gender_; }
int getAge() const { return age_; }
std::string getPhone() const { return phone_; }
double getRating() const { return rating_; }
Location getLocation() const { return location_; }
std::string getVehicle() const { return vehicle_; }
std::string getState() const { return state_; }
double getDistance() const { return distance_; }
std::string getRiderFName() const { return r_fname_; }
std::string getRiderLName() const { return r_lname_; }
std::string getRiderNum() const { return r_num_; }
// MODIFIERS
void setState(std::string state) { state_ = state; }
void setLocation(Location location) { location_ = location; }
void setDistance(double distance) { distance_ = distance; }
void setRiderFName(std::string r_fname) { r_fname_ = r_fname; }
void setRiderLName(std::string r_lname) { r_lname_ = r_lname; }
void setRiderNum(std::string r_num) { r_num_ = r_num; }

std::string printDriver(); //Used for message output

private: // REPRESENTATION (member variables)
std::string fname_;
std::string lname_;
std::string gender_;
int age_;
std::string phone_;
double rating_;
Location location_;
std::string vehicle_;
std::string state_;
double distance_;
std::string r_fname_;
std::string r_lname_;
std::string r_num_;
};

#endif