#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Used for std::stoi and std::sort
#include <sstream> //Includes stringstream

#include "location.h"
#include "user.h"

 std::vector<std::string> parse_likes(std::string &line){
    std::vector<std::string> likes;
    if (line == "null")
        return likes;
    else {
        std::istringstream iss(line);
        std::string phoneNumber;

        // tokenize the long string using the underscore delimiter
        while (std::getline(iss, phoneNumber, '_')) {
            std::cout << phoneNumber << std::endl;
        }
    }
    return likes;
 }

// Lisa 25 Female 318-125-5013 Undisclosed Undisclosed 41.5833 -83.9274 false 37 45 135 Both 663-979-6253_953-451-3708_410-750-5502_750-260-3152_688-574-6330_915-954-4073
int parse_users(std::ifstream &input, User* &head) {
    std::string name, gender, phone_number, profession, school, gender_pref, likes;
	bool isPremium;
	//std::vector<std::string> likes;
	int age, minAge_pref, maxAge_pref, max_dist;
	double lat, lon; 
    // Parses driver info
    if (input.is_open()){//If the file is open
    std::string test;
    getline(input, test);
    std::cout << test << std::endl;
	    while(input >> name >> age >> gender >> phone_number >> profession >> school >> lat >> lon >> isPremium >> minAge_pref >> maxAge_pref >> max_dist >> gender_pref >> likes){
            std::cout <<likes;
            Location user_loc(lat, lon); //location stored in location class
            std::vector<std::string> user_likes = parse_likes(likes);//Turn string into vector
            std::cout <<"test2";
            //User user_info(name, age, gender, phone_number, profession, school, user_loc, isPremium, 
            //    minAge_pref, maxAge_pref, max_dist, gender_pref, user_likes);
		}
        input.close();
    } else {
        std::cerr << "Unable to open input file" <<std::endl;
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]){
    std::ifstream users(argv[1]);
    std::ofstream output(argv[2]);

    User* head = nullptr;

    if (argc == 5 || argc == 6){
        parse_users(users, head);

        std::string user_number(argv[3]);
        std::string command(argv[4]);

    } else {
        std::cerr << "Incorrect amount of command line arguments." <<std::endl;
        return 1;
    }
    return 0;
}