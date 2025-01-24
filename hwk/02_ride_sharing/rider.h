#ifndef __RIDER_H
#define __RIDER_H

#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector>
#include "location.h"

class Rider {
public:
Rider(const std::string fname, const std::string lname, const std::string gender, const int age, 
    const std::string phone_number, const double rating, Location pickup_loc, 
    Location dropoff_loc, const std::string vehicle, std::string state, std::string d_fname, 
    std::string d_lname, std::string d_num);
std::string getFName() const { return fname_; }
std::string getLName() const { return lname_; }
std::string getGender() const { return gender_; }
int getAge() const { return age_; }
std::string getPhone() const { return phone_; }
double getRating() const { return rating_; }
Location getPickupLocation() const { return pickup_location_; }
Location getDropoffLocation() const { return dropoff_location_; }
std::string getVehicle() const { return vehicle_; }
std::string getState() const { return state_; }
std::string getDriverFName() const { return d_fname_; }
std::string getDriverLName() const { return d_lname_; }
std::string getDriverNum() const { return d_num_; }
// MODIFIERS
void setState(std::string state) { state_ = state; }
void setStartLoc(Location location) { pickup_location_ = location; }
void setEndLoc(Location location) { dropoff_location_ = location; }
void setDriverFName(std::string d_fname) { d_fname_ = d_fname; }
void setDriverLName(std::string d_lname) { d_lname_ = d_lname; }
void setDriverNum(std::string d_num) { d_num_ = d_num; }

std::string printRider(); //Used for message output

private: // REPRESENTATION (member variables)
Location pickup_location_;
Location dropoff_location_;
std::string fname_;
std::string lname_;
std::string gender_;
int age_;
std::string phone_;
double rating_;
std::string vehicle_;
std::string state_;
std::string d_fname_;
std::string d_lname_;
std::string d_num_;
};

#endif