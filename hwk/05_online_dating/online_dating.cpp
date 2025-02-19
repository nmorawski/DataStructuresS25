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
            likes.push_back(phoneNumber);
        }
    }
    return likes;
 }

// Lisa 25 Female 318-125-5013 Undisclosed Undisclosed 41.5833 -83.9274 false 37 45 135 Both 663-979-6253_953-451-3708_410-750-5502_750-260-3152_688-574-6330_915-954-4073
int parse_users(std::ifstream &users_input, User* &head){
    std::string name, gender, phone_number, profession, school, isPremium, gender_pref, likes;
	int age, minAge_pref, maxAge_pref, max_dist;
	double lat, lon; 

    if (users_input.is_open()){//If the file is open
	    while(users_input >> name >> age >> gender >> phone_number >> profession >> school >> lat >> lon
        >> isPremium >> minAge_pref >> maxAge_pref >> max_dist >> gender_pref >> likes){
            Location user_loc(lat, lon); //location stored in location class
            std::vector<std::string> user_likes = parse_likes(likes);//Turn string into vector
            User* new_user = new User(name, age, gender, phone_number, profession, school, user_loc, (isPremium == "true"), 
               minAge_pref, maxAge_pref, max_dist, gender_pref, user_likes);
            
            if (head == nullptr) {
                head = new_user;
            } else {
                User* current = head;
                while (current->next_ != nullptr) {
                    current = current->next_;
                }
                current->next_ = new_user;
            }
		}
        users_input.close();
    } else {
        std::cerr << "Unable to open input file" <<std::endl;
        return 1;
    }
    return 0;
}

// Function to find user by phone number
User* find_user(User* head, const std::string &phone) {
    User* current = head;
    while (current != nullptr) {
        if (current->phone_number_ == phone) return current;
        current = current->next_;
    }
    return nullptr;
}

void user_profile(const std::string &phone, User* &head, std::ofstream &users_output){
    User* given_user = find_user(head, phone);

    bool one_match = false;
    User* current = head;
    
    while (current != nullptr) {
        if (current->phone_number_ != phone && given_user->matchesPreferences(*current) &&
        !(given_user->isBlocked(current->phone_number_)) && !(current->isBlocked(phone))) {
            if (one_match) users_output << std::endl;
            if (!one_match) one_match = true;
            users_output << *current << std::endl;
        }
        current = current->next_;
    }
    if (!one_match) users_output << "There are no users matching with your preference at this moment." << std::endl;
}

void user_match(const std::string &phone, User* &head, std::ofstream &users_output){
    User* given_user = find_user(head, phone);
    
    std::vector<User*> matches;
    User* current = head;
    while (current != nullptr) {
        if (current->phone_number_ != phone && given_user->hasLiked(current->phone_number_) && 
            current->hasLiked(phone)){
            matches.push_back(current);
        }
        current = current->next_;
    }
    if (matches.empty()) {
        users_output << "You do not have any matches at this moment." << std::endl;
    } else {
        // Sort matches by phone number
        std::sort(matches.begin(), matches.end(), [](User* a, User* b) { return a->phone_number_ < b->phone_number_; });
        for (unsigned int i = 0; i < matches.size(); i++){
            if (i > 0) users_output << std::endl;
            users_output << *matches[i] << std::endl;
        }
    }
}

void user_like(const std::string &phone, User* &head, std::ofstream &users_output){
    User* given_user = find_user(head, phone);

    if (!given_user->isPremium_)
        users_output << "Only premium users can view who liked you." << std::endl;
    else {
        bool one_match = false;
        User* current = head;
        
        while (current != nullptr) {
            if (current->hasLiked(phone)) {
                if (one_match) users_output << std::endl;
                if (!one_match) one_match = true;
                users_output << *current << std::endl;
            }
            current = current->next_;
        }
        if (!one_match) users_output << "You have not received any likes so far." << std::endl;
        }
}

void users_unmatch(const std::string &phone, User* &head, std::ofstream &users_output, const std::string &other_phone){
    User* given_user = find_user(head, phone);
    User* other_user = find_user(head, other_phone);

    given_user->likes_.erase(std::find(given_user->likes_.begin(), given_user->likes_.end(), other_phone));
    users_output << given_user->name_ << "'s match list:"<<std::endl <<std::endl;
    user_match(phone, head, users_output);

    users_output << std::endl << "======" << std::endl << std::endl;
    other_user->likes_.erase(std::find(other_user->likes_.begin(), other_user->likes_.end(), phone));
    users_output << other_user->name_ << "'s match list:"<<std::endl <<std::endl;
    user_match(other_user->phone_number_, head, users_output);
}

void user_block(const std::string &phone, User* &head, std::ofstream &users_output, const std::string &blocked_phone){
    User* given_user = find_user(head, phone);
    User* other_user = find_user(head, blocked_phone);

    given_user->blockUser(blocked_phone);
    users_output << "profiles shown to "<< given_user->name_ << ":"<<std::endl <<std::endl;
    user_profile(phone, head, users_output);

    users_output << std::endl << "======" << std::endl << std::endl;
    other_user->likes_.erase(std::find(other_user->likes_.begin(), other_user->likes_.end(), phone));
    users_output << "profiles shown to "<< other_user->name_ << ":"<<std::endl <<std::endl;
    user_profile(other_user->phone_number_, head, users_output);
}

int main(int argc, char* argv[]){
    std::ifstream users(argv[1]);
    std::ofstream output(argv[2]);

    User* head = nullptr;

    if (argc == 5 || argc == 6){
        parse_users(users, head);
        std::string user_number(argv[3]);
        std::string command(argv[4]);

        if (command == "profile")
            user_profile(user_number, head, output);
        else if (command == "match")
            user_match(user_number, head, output);
        else if (command == "like")
            user_like(user_number, head, output);
        else if (command == "unmatch")
            users_unmatch(user_number, head, output, argv[5]);
        else if (command == "block")
            user_block(user_number, head, output, argv[5]);
        else {
            std::cerr << "Invalid command called." <<std::endl;
        return 1;
        }

    } else {
        std::cerr << "Incorrect amount of command line arguments." <<std::endl;
        return 1;
    }
    return 0;
}