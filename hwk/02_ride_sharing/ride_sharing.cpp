#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Used for std::stoi and std::sort
#include <sstream> //Includes stringstream
#include <cctype> //Used for isdigit()
#include <regex> //use for phone number match
#include <math.h>

#include "location.h"
#include "rider.h"
#include "driver.h"

/*The function parse_users takes in the two input files that store all of the original rider and
  driver information, and store the information. First, a line in the input is parsed and the 
  desired values are stored in the correct value types. Then, an object of either the Rider or 
  Driver class is created with these stored values, then the object is stored in its respective
  vector. This happens until the file is complete.*/
int parse_users(std::vector<Driver> &drivers_vec, std::vector<Rider> &riders_vec, std::ifstream 
    &driver_input, std::ifstream &rider_input) {

    std::string d_fname, d_lname, gender, d_num, vehicle_type, state, r_fname, r_lname,
        r_num, start_loc, end_loc;
	int age;
	double rating, start_lat, start_long, end_lat, end_long; 

    // Parses driver info
    if (driver_input.is_open()){//If the file is open
	    while(driver_input >> d_fname >> d_lname >> gender >> age >> d_num >> rating >> start_lat
        >> start_long >> vehicle_type >> state >> r_fname >> r_lname >> r_num){
            Location driver_loc(start_lat, start_long); //location stored in location class
            Driver new_driver(d_fname, d_lname, gender, age, d_num, rating, driver_loc, 
                vehicle_type, state, r_fname, r_lname, r_num); //Driver object
            drivers_vec.push_back(new_driver);
		}
        driver_input.close();
    } else {
        std::cerr << "Unable to open driver file" <<std::endl;
        return 1;
    }
    // Parses driver info
    if (rider_input.is_open()){//If the file is open
	    while(rider_input >> r_fname >> r_lname >> gender >> age >> r_num >> rating >> start_loc 
        >> start_lat >> start_long >> end_loc >> end_lat >> end_long >> vehicle_type >> state 
        >> d_fname >> d_lname >> d_num){
            Location pickup_loc(start_loc, start_lat, start_long); //Pickup location stored
            Location dropoff_loc(end_loc, end_lat, end_long); //Dropoff location stored
            Rider new_rider(r_fname, r_lname, gender, age, r_num, rating, pickup_loc, dropoff_loc,
            vehicle_type, state, d_fname, d_lname, d_num); //Rider object
            riders_vec.push_back(new_rider);
		}
        rider_input.close();
    } else {
        std::cerr << "Unable to open driver file" <<std::endl;
        return 1;
    }
    return 0;
}
/*The function verify_phone takes in a phone number and checks it among all of the stored
  riders. First, we chek to see if the phone number is in the coorect, desired format 
  (xxx-xxx-xxxx). If so, then we while loop through the vector of Riders, looking for a match. If
  a match is found, the rider's index in the vector will be returned. Otherwise, we return -1 to
  indicate a failure occured.*/
int verify_phone(const std::string &phone, const std::vector<Rider> &riders) {
    std::regex phone_pattern("[[:digit:]]{3}-[[:digit:]]{3}-[[:digit:]]{4}");
    if (std::regex_match(phone, phone_pattern)){ //Correct phone # format
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

// This function is idnetical to the above, except it accesses the vector of all Drivers. This
// overloading of the function was done because both vectors, of different objects, require the
// same searching feature to be done.
int verify_phone(const std::string &phone, const std::vector<Driver> &drivers) {
    std::regex phone_pattern("[[:digit:]]{3}-[[:digit:]]{3}-[[:digit:]]{4}");
    if (std::regex_match(phone, phone_pattern)){ //Correct phone # format
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

/*The function rider_request takes in a chosen rider and performs the desired "request" command.
  The driver and rider vectors are passed, as we need to update the status of the rider/driver, as
  well as store the basic info (first name, last name, phone number) after a successful request
  completion. The messages output file is passed to print the necessary commands.*/
void rider_request(Rider &rider, const int &rider_index, std::vector<Rider> &riders, 
    std::vector<Driver> &drivers, std::ofstream &output_file){

    output_file << "Ride requested for rider " << rider.getFName() << ", looking for a";
    if (rider.getVehicle() =="Economy") //Account for consonant/vowels
        output_file << "n";
    output_file << " "<< rider.getVehicle()<< " vehicle." << std::endl;
    output_file << "Pick Up Location: " << rider.getPickupLocation().getLocationName() << 
        ", Drop Off Location: " << rider.getDropoffLocation().getLocationName() << "."<<std::endl;

    std::vector<Driver> eligible_drivers;
    double distance;
    // For loop to search through all drivers and see if they are "compatible" with the rider
    // Compatible = vehicle make matches request and is in an Available state
    for (unsigned int i = 0; i < drivers.size(); i++){
        if (drivers[i].getVehicle() == rider.getVehicle() && drivers[i].getState() == "Available"){
            // Distance is a double, and therefore has lots of decimal places. For our output, we
            // want it to have one decimal place. To do this, we multiply the value by 10, truncate
            // the number, which removes all decimal places, then divide by ten.
            distance = trunc(calculateDistance(rider.getPickupLocation(), 
                drivers[i].getLocation())*10)/10;
            drivers[i].setDistance(distance); //Calculate distance from rider
            eligible_drivers.push_back(drivers[i]);
        }
    }
    // If none of the drivers listed are compatible
    if (eligible_drivers.size() == 0)
        output_file  << "Sorry we can not find a driver for you at this moment." << std::endl;
    else {
        // This std::sort looks complicated, but is rather simple. I am sorting all of the eligible
        // drivers by their distance away from the rider, in ascending order. I therefore take the
        // first listed Driver, who would be the closest to the rider
        std::sort(eligible_drivers.begin(), eligible_drivers.end(), [](Driver const& drvr1, 
            Driver const& drvr2) { return drvr1.getDistance() < drvr2.getDistance(); });
        Driver selected_driver = eligible_drivers[0];
        int driver_index = verify_phone(selected_driver.getPhone(), drivers);

        // Update rider's state & assigned driver
        riders[rider_index].setState("Driver_on_the_way");
        riders[rider_index].setDriverFName(selected_driver.getFName());
        riders[rider_index].setDriverLName(selected_driver.getLName());
        riders[rider_index].setDriverNum(selected_driver.getPhone());

        // Update driver's state & assigned rider
        drivers[driver_index].setState("On_the_way_to_pickup");
        drivers[driver_index].setRiderFName(rider.getFName());
        drivers[driver_index].setRiderLName(rider.getLName());
        drivers[driver_index].setRiderNum(rider.getPhone());

        output_file << "We have found the closest driver " << selected_driver.getFName() << "("<< 
            selected_driver.getRating()<<") for you."<<std::endl;
        output_file << selected_driver.getFName() << " is now " << selected_driver.getDistance() <<
            " miles away from you." << std::endl;
    }
}

/*The function rider_cancel takes in a chosen rider and performs the desired "cancel" command.
  The driver and rider vectors are passed, as we need to update the status of the rider/driver, as
  well as store the basic info (first name, last name, phone number) after a successful request
  completion. The messages output file is passed to print the necessary commands.*/
void rider_cancel(Rider &rider, const int &rider_index, std::vector<Rider> &riders, 
    std::vector<Driver> &drivers, std::ofstream &output_file){

    int driver_index = verify_phone(rider.getDriverNum(), drivers);

    // Update rider's state & assigned driver
    riders[rider_index].setState("Ready_to_request");
    riders[rider_index].setDriverFName("null");
    riders[rider_index].setDriverLName("null");
    riders[rider_index].setDriverNum("null");

    // Deassign driver
    drivers[driver_index].setState("Available");
    drivers[driver_index].setRiderFName("null");
    drivers[driver_index].setRiderLName("null");
    drivers[driver_index].setRiderNum("null");

    output_file << "Ride request for rider "<< rider.getFName()<<" is now canceled by the rider."
        <<std::endl;
}

/*The function driver_cancel takes in a chosen driver and performs the desired "cancel" command.
  The driver and rider vectors are passed, as we need to update the status of the rider/driver, as
  well as store the basic info (first name, last name, phone number) after a successful request
  completion. The messages output file is passed to print the necessary commands. An important
  feature of this function is that when a driver cancels, the rider is assigned another driver. 
  However, it is important to make sure the driver that just cancelled isnt reassigned to the same
  rider. To do this, we update the driver's info once the rider has already found a new driver,
  (which is the second closest driver).*/
void driver_cancel(Driver &driver, const int &driver_index, std::vector<Rider> &riders, 
    std::vector<Driver> &drivers, std::ofstream &output_file){

    int rider_index = verify_phone(driver.getRiderNum(), riders);
    Rider selected_rider = riders[rider_index];

    // Update rider's state & assigned driver
    riders[rider_index].setState("Ready_to_request");
    riders[rider_index].setDriverFName("null");
    riders[rider_index].setDriverLName("null");
    riders[rider_index].setDriverNum("null");
    output_file << "Your driver "<<driver.getFName()<<
        " has canceled the ride request. We will now find a new driver for you."<<std::endl;
    // rider_request called to assign new driver to cancelled-on rider
    rider_request(selected_rider, rider_index, riders, drivers, output_file);

    // Update driver's state & now de-assign rider
    drivers[driver_index].setState("Available");
    drivers[driver_index].setRiderFName("null");
    drivers[driver_index].setRiderLName("null");
    drivers[driver_index].setRiderNum("null");
}

/*The function ride_command is used by the main function to help execute the desired actions. Very
  extensive error checking is done to make sure everything runs smoothly.*/
int ride_command(const std::string &cmd, const std::string &phone_number, std::vector<Driver> 
    &drivers_vec, std::vector<Rider> &riders_vec, std::ofstream &output) {
    
    int phone_verification;
    if (cmd == "request"){ // Request command
        phone_verification = verify_phone(phone_number, riders_vec); //Check phone among riders
        if (phone_verification == -1) // Invalid format
            output  << "Phone number is invalid." << std::endl;
        else if (phone_verification == -2) // Number not found
            output  << "Account does not exist." << std::endl; 
        else {
            Rider found_rider = riders_vec[phone_verification]; // Found, store info
            if (found_rider.getState() == "Driver_on_the_way")
                output  << 
                    "You have already requested a ride and your driver is on the way to the pickup location."
                    << std::endl;
            else if (found_rider.getState() == "During_the_trip")
                output  << 
                "You can not request a ride at this moment as you are already on a trip." 
                << std::endl;
            else if (found_rider.getState() == "Ready_to_request")
                rider_request(found_rider, phone_verification, riders_vec, drivers_vec, output);
        }
    } else if (cmd == "cancel"){ // Cancel Command
        phone_verification = verify_phone(phone_number, riders_vec); //Check phone among riders
         if (phone_verification == -1) // Invalid format
            output  << "Phone number is invalid." << std::endl;
        else if (phone_verification == -2){ // Number not found among riders
            phone_verification = verify_phone(phone_number, drivers_vec); //Check driver phones
            if (phone_verification == -2) // Number not found among drivers
                output  << "Account does not exist." << std::endl; 
            else {
                Driver found_driver = drivers_vec[phone_verification];
                if (found_driver.getState() != "On_the_way_to_pickup")
                    output  <<
                    "You can only cancel a ride request if you are currently on the way to the pickup location."
                    << std::endl;
                else
                    driver_cancel(found_driver, phone_verification, riders_vec, 
                        drivers_vec, output);
            }
        } else {
            Rider found_rider = riders_vec[phone_verification];
            if (found_rider.getState() != "Driver_on_the_way")
                output  <<
                "You can only cancel a ride request if your driver is currently on the way to the pickup location."
                << std::endl;
            else
                rider_cancel(found_rider, phone_verification, riders_vec, drivers_vec, output);
        }
    } else {
        std::cerr << "Invalid command requested." <<std::endl;
        return 1;
    }
    return 0;
}

// Main function, takes in cmd line, performs necessary functions
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

        // Write updated driver info
        if (updated_drivers.is_open()){
            for (unsigned int i = 0; i < drivers.size(); i++){
                updated_drivers << drivers[i].printDriver() <<std::endl;
            }
            updated_drivers.close();
        } else {
            std::cerr << "Unable to open file" <<std::endl;
            return 1;
        }
        // Write updated rider info
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