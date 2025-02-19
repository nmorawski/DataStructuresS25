#include "user.h"

User::User(const std::string &name, int age, const std::string &gender, const std::string &phone, 
    const std::string &profession, const std::string &school, const Location &loc, bool premium, 
    int minAge, int maxAge, int maxDist, const std::string &genderPref, 
    const std::vector<std::string> &likes){
    
    name_ = name;
	age_ = age;
	gender_ = gender;
	phone_number_ = phone;
	profession_ = profession;
	school_ = school;
	location_ = loc;
	isPremium_ = premium;
	minAge_pref_ = minAge;
	maxAge_pref_ = maxAge;
	maxDist_ = maxDist;
	gender_pref_ = genderPref;
	likes_ = likes;
    next_ = nullptr;
}

bool User::matchesPreferences(const User &other) const {
    // Check age preferences
    if (other.age_ < minAge_pref_ || other.age_ > maxAge_pref_) return false;
    
    // Check gender preferences
    if (gender_pref_ != "Both" && gender_pref_ != other.gender_) return false;
    
    // Check distance
    double distance = calculateDistance(location_, other.location_);
    if (distance > maxDist_) return false;

    return true;
}

bool User::hasLiked(const std::string &phone_number) const {
    return std::find(likes_.begin(), likes_.end(), phone_number) != likes_.end();
}

bool User::isBlocked(const std::string &phone_number) const {
    return std::find(blocked_.begin(), blocked_.end(), phone_number) != blocked_.end();
}

std::ostream& operator<<(std::ostream &out, User &u){
    out << u.name_ << " " << u.age_;

    if (u.profession_ != "Undisclosed"){
        std::replace(u.profession_.begin(), u.profession_.end(), '_', ' ');
        out << std::endl << u.profession_;
    }
    if (u.school_ != "Undisclosed"){
        std::replace(u.school_.begin(), u.school_.end(), '_', ' ');
        out << std::endl << u.school_;
    }

    return out;
}

// void User::removeLike(const std::string& phone_number) {
//     likes.erase(std::remove(likes.begin(), likes.end(), phone_number), likes.end());
// }