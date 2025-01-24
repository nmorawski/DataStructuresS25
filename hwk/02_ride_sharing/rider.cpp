#include <string>
#include <iostream>

#include "rider.h"

Rider::Rider(const std::string fname, const std::string lname, const std::string gender, const int age, 
    const std::string phone_number, const double rating, Location pickup_loc, Location dropoff_loc,
    const std::string vehicle, std::string state, std::string d_fname, std::string d_lname, 
    std::string d_num) {

    pickup_location_ = pickup_loc;
    dropoff_location_ = dropoff_loc;
    fname_ = fname;
    lname_ = lname;
    gender_ = gender;
    age_ = age;
    phone_ = phone_number;
    rating_ = rating;
    vehicle_ = vehicle;
    state_ = state;
    d_fname_ = d_fname;
    d_lname_ = d_lname;
    d_num_ = d_num;
}

std::string Rider::printRider() {
    std::string line = fname_;
    line += " " + lname_;
    line += " " + gender_;
    line += " " + std::to_string(age_);
    line += " " + phone_;
    line += " " + remove_zeroes(rating_);
    line += " " + pickup_location_.getLocationName();
    line += " " + remove_zeroes(pickup_location_.getLatitude());
    line += " " + remove_zeroes(pickup_location_.getLongitude());
    line += " " + dropoff_location_.getLocationName();
    line += " " + remove_zeroes(dropoff_location_.getLatitude());
    line += " " + remove_zeroes(dropoff_location_.getLongitude());
    line += " " + vehicle_;
    line += " " + state_;
    line += " " + d_fname_;
    line += " " + d_lname_;
    line += " " + d_num_;
    return line;
}