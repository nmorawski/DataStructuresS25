#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Used for std::sort
#include <sstream> //Includes stringstream

#include "location.h"
#include "user.h"

// The parse_likes function is a helper function for the parse_users function. It takes in a string
// of phone numbers, separated by underscores, and stores all of the found phone numbers in a 
// vector. This vector is stored in a User class object as the user's likes.
std::vector<std::string> parse_likes(std::string &line){
    std::vector<std::string> likes;
    if (line == "null") // No likes, return empty vector
        return likes;
    else {
        std::istringstream iss(line);
        std::string phoneNumber;

        // Tokenize the long string using the underscore delimiter
        while (std::getline(iss, phoneNumber, '_')) {
            likes.push_back(phoneNumber);
        }
    }
    return likes;
 }

// The function parse_users takes in the user input file that stores all of the dating app user 
// information, and stores the information in a linked list. First, a line in the input is parsed
// and the desired values are stored in the correct value types. Then, an object of the User class
// is created with these stored values, then the object is stored at the end of the linked list.
// This happens until the file is complete.
int parse_users(std::ifstream &users_input, User* &head){
    std::string name, gender, phone_number, profession, school, isPremium, gender_pref, likes;
	int age, minAge_pref, maxAge_pref, max_dist;
	double lat, lon; 
    User* current; // Helper pointer

    if (users_input.is_open()){//If the file is open
	    while(users_input >> name >> age >> gender >> phone_number >> profession >> school >> lat
        >> lon >> isPremium >> minAge_pref >> maxAge_pref >> max_dist >> gender_pref >> likes){
            Location user_loc(lat, lon); // Location stored in location class
            std::vector<std::string> user_likes = parse_likes(likes);// Turn string into vector
            User* new_user = new User(name, age, gender, phone_number, profession, school, 
                user_loc, (isPremium == "true"), minAge_pref, maxAge_pref, max_dist, gender_pref,
                user_likes); // User object, note that the isPremium variable is converted to bool
            
            if (head == nullptr) { // For empty linked list
                head = current = new_user;
            } else {
                current->next_ = new_user;
                current = new_user;
            }
		}
        users_input.close();
    } else {
        std::cerr << "Unable to open input file" <<std::endl;
        return 1;
    }
    return 0;
}

// The find_user function is a helper function that takes in the singly-linked list and a phone
// number, and returns a pointer to where the desired user exists in the list. If not found, return
// a nullptr, as a precaution.
User* find_user(User* head, const std::string &phone) {
    User* current = head;
    while (current != nullptr) {
        if (current->phone_number_ == phone) return current;
        current = current->next_;
    }
    return nullptr;
}

// The user_profile function is used for the "profile" command. The function takes in a desired
// user's phone number, the singly-linked list, and the output file. First, the desired user is 
// is found in the list, and pointer to the user is saved. Next, the list is traversed, and each
// user is checked to see if they fit the preferences of the desired user (age and location in
// desired range, gender preferences match, and if one of the users is blocked). If all preferences
// are met, print the user's info.
void user_profile(const std::string &phone, User* &head, std::ofstream &users_output){
    User* given_user = find_user(head, phone);

    bool one_match = false; // Used for printing
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
    if (!one_match) users_output << 
        "There are no users matching with your preference at this moment." << std::endl;
}

// The user_match function is used for the "match" command. The function takes in a desired
// user's phone number, the singly-linked list, and the output file. First, the desired user is 
// is found in the list, and pointer to the user is saved. Next, the list is traversed, and each
// user is checked to see if (1) the desired user liked them, and (2) said user also liked the
// desired user. If both conditions are met, save a pointer to the user's info in a vector.
// When all of the user pointers are saved, sort the users by the phone number, ascending order,
// and print the users's info.
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
        std::sort(matches.begin(), matches.end(), [](User* a, User* b) { return a->phone_number_ 
            < b->phone_number_; });
        for (unsigned int i = 0; i < matches.size(); i++){
            if (i > 0) users_output << std::endl;
            users_output << *matches[i] << std::endl;
        }
    }
}

// The user_like function is used for the "like" command. The function takes in a desired
// user's phone number, the singly-linked list, and the output file. First, the desired user is 
// is found in the list, and pointer to the user is saved. Next, the user is checked to see if
// they are a premium user. If they are, proceed. We loop through the list of all users, and if
// the given user's phone number exists in a user's liked users vector, print their info.
void user_like(const std::string &phone, User* &head, std::ofstream &users_output){
    User* given_user = find_user(head, phone);

    if (!given_user->isPremium_)
        users_output << "Only premium users can view who liked you." << std::endl;
    else {
        bool one_match = false; // Used for printing
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

// The users_unmatch function is used for the "unmatch" command. The function takes in a desired
// user's phone number, the singly-linked list, the output file, and the user that the desired user
// wants to unmatch. First, the desired user and desired unmatch are found in the list, and 
// pointers to the users are saved. Next, the phone numbers of the match are removed from both 
// users likes. Finally, the user_match() function is called for both users, which takes care of 
// the necessary info printing.
void users_unmatch(const std::string &phone, User* &head, std::ofstream &users_output,
    const std::string &other_phone){
    User* given_user = find_user(head, phone);
    User* other_user = find_user(head, other_phone);

    given_user->likes_.erase(std::find(given_user->likes_.begin(), given_user->likes_.end(), 
        other_phone));
    users_output << given_user->name_ << "'s match list:"<<std::endl <<std::endl;
    user_match(phone, head, users_output);

    users_output << std::endl << "======" << std::endl << std::endl;
    other_user->likes_.erase(std::find(other_user->likes_.begin(), other_user->likes_.end(), 
        phone));
    users_output << other_user->name_ << "'s match list:"<<std::endl <<std::endl;
    user_match(other_user->phone_number_, head, users_output);
}

// The user_block function is used for the "block" command. The function takes in a desired
// user's phone number, the singly-linked list, the output file, and the user that the desired user
// wants to block. First, the desired user and desired unmatch are found in the list, and 
// pointers to the users are saved. Next, the phone number of the blocked user is saved to the 
// desired user's blocked-user vector, and the phone numbers are removed from both users likes, 
// just in case. Finally, the user_profile() function is called for both users, which takes care of
// the necessary info printing.
void user_block(const std::string &phone, User* &head, std::ofstream &users_output,
    const std::string &blocked_phone){
    User* given_user = find_user(head, phone);
    User* other_user = find_user(head, blocked_phone);

    given_user->blockUser(blocked_phone);
    if (given_user->hasLiked(blocked_phone))
        given_user->likes_.erase(std::find(given_user->likes_.begin(), given_user->likes_.end(), 
            blocked_phone));
    users_output << "profiles shown to "<< given_user->name_ << ":"<<std::endl <<std::endl;
    user_profile(phone, head, users_output);

    users_output << std::endl << "======" << std::endl << std::endl;
    if (other_user->hasLiked(phone))
        other_user->likes_.erase(std::find(other_user->likes_.begin(), other_user->likes_.end(),
            phone));
    users_output << "profiles shown to "<< other_user->name_ << ":"<<std::endl <<std::endl;
    user_profile(other_user->phone_number_, head, users_output);
}

// Function used to free allocated memory. Iterates through linked list, deleting one pointer at
// a time. Upon completion, set head pointer to null.
void DeleteList(User* &head) {
    User* current = head;
    while (current) {
        User* next = current->next_;
        delete current;
        current = next;
    }
    head = nullptr;
}

int main(int argc, char* argv[]){
    std::ifstream users(argv[1]); // Input
    std::ofstream output(argv[2]); // output

    User* head = nullptr;

    if (argc == 5 || argc == 6){
        parse_users(users, head);
        std::string user_number(argv[3]); // Desired user
        std::string command(argv[4]); // command

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

        DeleteList(head); // Delete singly-linked list

    } else {
        std::cerr << "Incorrect amount of command line arguments." <<std::endl;
        return 1;
    }
    return 0;
}