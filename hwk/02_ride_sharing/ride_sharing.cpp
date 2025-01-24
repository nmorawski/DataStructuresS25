#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Used for std::stoi and std::sort
#include <sstream> //Includes stringstream
#include <cctype> //Used for isdigit()
#include <regex> //use for phoen number match, make sure mention in readme
#include <math.h>

#include "location.h"
#include "rider.h"
#include "driver.h"

int parse_users(std::vector<Driver> &drivers_vec, std::vector<Rider> &riders_vec, std::ifstream 
    &driver_input, std::ifstream &rider_input) {

    std::string d_fname, d_lname, gender, d_num, vehicle_type, state, r_fname, r_lname,
        r_num, start_loc, end_loc;
	int age;
	double rating, start_lat, start_long, end_lat, end_long;

    if (driver_input.is_open()){//If the file is open
	    while(driver_input >> d_fname >> d_lname >> gender >> age >> d_num >> rating >> start_lat
        >> start_long >> vehicle_type >> state >> r_fname >> r_lname >> r_num){
            Location driver_loc(start_lat, start_long);
            Driver new_driver(d_fname, d_lname, gender, age, d_num, rating, driver_loc, 
                vehicle_type, state, r_fname, r_lname, r_num);
            drivers_vec.push_back(new_driver);
		}

        driver_input.close();
    } else {
        std::cerr << "Unable to open driver file" <<std::endl;
        return 1;
    }

    if (rider_input.is_open()){//If the file is open
	    while(rider_input >> r_fname >> r_lname >> gender >> age >> r_num >> rating >> start_loc 
        >> start_lat >> start_long >> end_loc >> end_lat >> end_long >> vehicle_type >> state 
        >> d_fname >> d_lname >> d_num){
            Location pickup_loc(start_loc, start_lat, start_long);
            Location dropoff_loc(end_loc, end_lat, end_long);
            Rider new_rider(r_fname, r_lname, gender, age, r_num, rating, pickup_loc, dropoff_loc, vehicle_type, state,
                d_fname, d_lname, d_num);
            riders_vec.push_back(new_rider);
		}

        rider_input.close();
    } else {
        std::cerr << "Unable to open driver file" <<std::endl;
        return 1;
    }

    return 0;
}

int verify_phone(const std::string &phone, const std::vector<Rider> &riders) { //Riders
    std::regex phone_pattern("[[:digit:]]{3}-[[:digit:]]{3}-[[:digit:]]{4}");
    if (std::regex_match(phone, phone_pattern)){
        unsigned int num_index = 0;
        std::string found_num = "";
        while (phone != found_num && num_index < riders.size()){
            found_num = riders[num_index].getPhone();
            num_index++;
        }
        if (phone == found_num)//Match found
            return num_index-1;
        return -2; //Account doesn't exist
    } else {
        return -1; //Invalid phone number
    }
}

int verify_phone(const std::string &phone, const std::vector<Driver> &drivers) { //Drivers
    std::regex phone_pattern("[[:digit:]]{3}-[[:digit:]]{3}-[[:digit:]]{4}");
    if (std::regex_match(phone, phone_pattern)){
        unsigned int num_index = 0;
        std::string found_num = "";
        while (phone != found_num && num_index < drivers.size()){
            found_num = drivers[num_index].getPhone();
            num_index++;
        }
        if (phone == found_num)//Match found
            return num_index-1;
        return -2; //Account doesn't exist
    } else {
        return -1; //Invalid phone number
    }
}

void rider_request(Rider &rider, const int &rider_index, std::vector<Rider> &riders, 
    std::vector<Driver> &drivers, std::ofstream &output_file){

    std::cout << "Ride requested for rider " << rider.getFName() << ", looking for a";
    if (rider.getVehicle() =="Economy")
        std::cout << "n";
    std::cout << " "<< rider.getVehicle()<< " vehicle." << std::endl;
    std::cout << "Pick Up Location: " << rider.getPickupLocation().getLocationName() << ", Drop Off Location: " << rider.getDropoffLocation().getLocationName() << "."<<std::endl;

    std::vector<Driver> eligible_drivers;
    double distance;
    for (unsigned int i = 0; i < drivers.size(); i++){
        if (drivers[i].getVehicle() == rider.getVehicle() && drivers[i].getState() == "Available"){
            distance = trunc(calculateDistance(rider.getPickupLocation(), drivers[i].getLocation())*10)/10;//EXPLAIN
            drivers[i].setDistance(distance);
            eligible_drivers.push_back(drivers[i]);
        }
    }

    if (eligible_drivers.size() == 0)
        std::cout  << "Sorry we can not find a driver for you at this moment." << std::endl; //CHANGE THESE COUTS TO BE IN OUTPUT FILE
    else {
        std::sort(eligible_drivers.begin(), eligible_drivers.end(), [](Driver const& drvr1, Driver const& drvr2) { return drvr1.getDistance() < drvr2.getDistance(); });//NOTE IN README
        Driver selected_driver = eligible_drivers[0];
        int driver_index = verify_phone(selected_driver.getPhone(), drivers);

        riders[rider_index].setState("Driver_on_the_way");
        riders[rider_index].setDriverFName(selected_driver.getFName());
        riders[rider_index].setDriverLName(selected_driver.getLName());
        riders[rider_index].setDriverNum(selected_driver.getPhone());

        drivers[driver_index].setState("On_the_way_to_pickup");
        drivers[driver_index].setRiderFName(rider.getFName());
        drivers[driver_index].setRiderLName(rider.getLName());
        drivers[driver_index].setRiderNum(rider.getPhone());

        std::cout << "We have found the closest driver " << selected_driver.getFName() << "("<< selected_driver.getRating()<<") for you."<<std::endl;
        std::cout << selected_driver.getFName() << " is now " << selected_driver.getDistance() << " miles away from you." << std::endl;
    }
}

void rider_cancel(Rider &rider, const int &rider_index, std::vector<Rider> &riders, 
    std::vector<Driver> &drivers, std::ofstream &output_file){

    int driver_index = verify_phone(rider.getDriverNum(), drivers);

    riders[rider_index].setState("Ready_to_request");
    riders[rider_index].setDriverFName("null");
    riders[rider_index].setDriverLName("null");
    riders[rider_index].setDriverNum("null");

    drivers[driver_index].setState("Available");
    drivers[driver_index].setRiderFName("null");
    drivers[driver_index].setRiderLName("null");
    drivers[driver_index].setRiderNum("null");

    std::cout << "Ride request for rider "<< rider.getFName()<<" is now canceled by the rider."<<std::endl;
}

void driver_cancel(Driver &driver, const int &driver_index, std::vector<Rider> &riders, 
    std::vector<Driver> &drivers, std::ofstream &output_file){

    int rider_index = verify_phone(driver.getRiderNum(), riders);
    Rider selected_rider = riders[rider_index];

    riders[rider_index].setState("Ready_to_request");
    riders[rider_index].setDriverFName("null");
    riders[rider_index].setDriverLName("null");
    riders[rider_index].setDriverNum("null");
    std::cout << "Your driver "<<driver.getFName()<<" has canceled the ride request. We will now find a new driver for you."<<std::endl;
    rider_request(selected_rider, rider_index, riders, drivers, output_file);

    drivers[driver_index].setState("Available");
    drivers[driver_index].setRiderFName("null");
    drivers[driver_index].setRiderLName("null");
    drivers[driver_index].setRiderNum("null");
}

int ride_command(const std::string &cmd, const std::string &phone_number, std::vector<Driver> 
    &drivers_vec, std::vector<Rider> &riders_vec, std::ofstream &output) {
    
    int phone_verification;
    if (cmd == "request"){
        phone_verification = verify_phone(phone_number, riders_vec);
        if (phone_verification == -1){
            std::cout  << "Phone number is invalid." << std::endl; //CHANGE THESE COUTS TO BE IN OUTPUT FILE
        } else if (phone_verification == -2){
            std::cout  << "Account does not exist." << std::endl; //CHANGE THESE COUTS TO BE IN OUTPUT FILE
        } else {
            Rider found_rider = riders_vec[phone_verification];
            std::cout << found_rider.getPhone() << " " << found_rider.getState() <<std::endl;
            if (found_rider.getState() == "Driver_on_the_way")
                std::cout  << "You have already requested a ride and your driver is on the way to the pickup location." << std::endl; //CHANGE THESE COUTS TO BE IN OUTPUT FILE
            else if (found_rider.getState() == "During_the_trip")
                std::cout  << "You can not request a ride at this moment as you are already on a trip." << std::endl; //CHANGE THESE COUTS TO BE IN OUTPUT FILE
            else if (found_rider.getState() == "Ready_to_request"){
                rider_request(found_rider, phone_verification, riders_vec, drivers_vec, output);
            }
        }
        
    } else if (cmd == "cancel"){
        phone_verification = verify_phone(phone_number, riders_vec);
         if (phone_verification == -1){
            std::cout  << "Phone number is invalid." << std::endl; //CHANGE THESE COUTS TO BE IN OUTPUT FILE
        } else if (phone_verification == -2){
            phone_verification = verify_phone(phone_number, drivers_vec);
            if (phone_verification == -2)
                std::cout  << "Account does not exist." << std::endl; //CHANGE THESE COUTS TO BE IN OUTPUT FILE
            else {
                Driver found_driver = drivers_vec[phone_verification];
                std::cout << found_driver.getState() <<std::endl;
                if (found_driver.getState() != "On_the_way_to_pickup")
                    std::cout  << "You can only cancel a ride request if you are currently on the way to the pickup location." << std::endl;
                else
                    driver_cancel(found_driver, phone_verification, riders_vec, drivers_vec, output);
            }
        } else {
            Rider found_rider = riders_vec[phone_verification];
            std::cout << found_rider.getPhone() << " " << found_rider.getState() <<std::endl;
            if (found_rider.getState() != "Driver_on_the_way")
                std::cout  << "You can only cancel a ride request if your driver is currently on the way to the pickup location." << std::endl; //CHANGE THESE COUTS TO BE IN OUTPUT FILE
            else
                rider_cancel(found_rider, phone_verification, riders_vec, drivers_vec, output);
        }
    } else {
        std::cerr << "Invalid command requested." <<std::endl;
        return 1;
    }

    return 0;
}

//nyride.exe drivers.txt riders.txt output0.txt output1.txt output2.txt phoneNumber request (8)
//nyride.exe drivers.txt riders.txt output0.txt output1.txt output2.txt phoneNumber cancel (8)

int main(int argc, char* argv[]){
    std::ifstream driver_list(argv[1]);
    std::ifstream rider_list(argv[2]);

    std::vector<Driver> drivers;
    std::vector<Rider> riders;

    parse_users(drivers, riders, driver_list, rider_list);

    if (argc == 8){
        std::ofstream messages(argv[3]);
        std::ofstream updated_drivers(argv[4]);
        std::ofstream updated_riders(argv[5]);

        ride_command(argv[7],argv[6], drivers, riders, messages);

        if (updated_drivers.is_open()){
            for (unsigned int i = 0; i < drivers.size(); i++){
                updated_drivers << drivers[i].printDriver() <<std::endl;
            }
            updated_drivers.close();
        } else {
            std::cerr << "Unable to open file" <<std::endl;
            return 1;
        }

        if (updated_riders.is_open()){
            for (unsigned int i = 0; i < riders.size(); i++){
                updated_riders << riders[i].printRider() <<std::endl;
            }
            updated_riders.close();
        } else {
            std::cerr << "Unable to open file" <<std::endl;
            return 1;
        }
    } else {
        std::cerr << "Incorrect amount of command line arguments." <<std::endl;
        return 1;
    }

    return 0;
}