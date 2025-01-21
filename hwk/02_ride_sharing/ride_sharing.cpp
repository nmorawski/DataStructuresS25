#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Used for std::stoi
#include <sstream> //Includes stringstream
#include <cctype> //Used for isdigit()
#include <regex> //use for phoen number match, make sure mention in readme

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
            Rider rider_info(r_fname, r_num);
            Driver new_driver(d_fname, d_num, rating, driver_loc, vehicle_type, state, rider_info);
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
            Driver driver_info(d_fname, d_num);
            Rider new_rider(r_fname, r_num, rating, pickup_loc, dropoff_loc, vehicle_type, state,
                driver_info);
            riders_vec.push_back(new_rider);
		}

        rider_input.close();
    } else {
        std::cerr << "Unable to open driver file" <<std::endl;
        return 1;
    }

    return 0;
}

std::string verify_phone(const std::string &phone, const std::vector<Rider> &riders) {
    std::regex phone_pattern("[[:digit:]]{2}-[[:digit:]]{3}-[[:digit:]]{4}");
    if (std::regex_match(phone, phone_pattern)){
        unsigned int num_index = 0;
        std::string found_num = "";
        while (phone != found_num && num_index < riders.size()){
            found_num = riders[num_index].getPhone();
            num_index++;
        }
        if (phone == found_num)//Match found
            return std::to_string(num_index);
        return "Account does not exist.\n";
    } else {
        return "Phone number is invalid.\n";
    }
}

int ride_command(const std::string &cmd, const std::string &phone_number, std::vector<Driver> 
    &drivers_vec, std::vector<Rider> &riders_vec, std::ofstream &output) {
        //Decide how to work around cancel (both rider and driver can cancel)
    if (cmd == "request"){

    } else if (cmd == "cancel"){

    } else {
        std::cerr << "Invalid command requested." <<std::endl;
        return 1;
    }
    
    
    
    
    
    
    std::string phone_verification = verify_phone(phone_number, riders_vec, drivers_vec);

    if (phone_verification.back() == 'n'){
        Rider desired_rider = riders_vec[std::stoi(phone_verification)];
        if (cmd == "request"){

        } else if (cmd == "cancel"){

        } else {
            std::cerr << "Invalid command requested." <<std::endl;
            return 1;
        }
    } else {
        std::cout << phone_verification << std::endl;
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

    } else if (argc == 5){


    } else {
        std::cerr << "Incorrect amount of command line arguments." <<std::endl;
        return 1;
    }

    return 0;
}