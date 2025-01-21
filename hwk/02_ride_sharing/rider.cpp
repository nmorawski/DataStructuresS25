#include "rider.h"
#include "driver.h"
#include "location.h"

Rider::Rider(const std::string name, const std::string phone_number, const double rating, 
    Location pickup_loc, Location dropoff_loc, const std::string vehicle, 
    std::string state, Driver driver) {

    driver_ = driver;
    pickup_location_ = pickup_loc;
    dropoff_location_ = dropoff_loc;
    name_ = name;
    phone_ = phone_number;
    rating_ = rating;
    vehicle_ = vehicle;
    state_ = state;
}