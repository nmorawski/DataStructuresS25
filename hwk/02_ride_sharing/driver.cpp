#include <string>
#include <iostream>
#include "driver.h"

// Driver constructor
Driver::Driver(const std::string fname, const std::string lname, const std::string gender, 
    const int age, const std::string phone_number, double rating, Location location, 
    std::string vehicle, std::string state, std::string r_fname, std::string r_lname, 
    std::string r_num){

    location_ = location;
    fname_ = fname;
    lname_ = lname;
    gender_ = gender;
    age_ = age;
    phone_ = phone_number;
    rating_ = rating;
    vehicle_ = vehicle;
    state_ = state;
    r_fname_ = r_fname;
    r_lname_ = r_lname;
    r_num_ = r_num;
    distance_ = 0; //Used to store driver's distance from requesting rider
    }

// Function used to store all of one driver's info into a single string/line,
// which is then added to the output file that represents all driver info
std::string Driver::printDriver() {
    std::string line = fname_;
    line += " " + lname_;
    line += " " + gender_;
    line += " " + std::to_string(age_);
    line += " " + phone_;
    line += " " + remove_zeroes(rating_);
    line += " " + remove_zeroes(location_.getLatitude());
    line += " " + remove_zeroes(location_.getLongitude());
    line += " " + vehicle_;
    line += " " + state_;
    line += " " + r_fname_;
    line += " " + r_lname_;
    line += " " + r_num_;
    return line;
}