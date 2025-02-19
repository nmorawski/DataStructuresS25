#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <iostream> 
#include <fstream>
#include <algorithm>
#include "location.h"

class User {
public:
    User(const std::string &name, int age, const std::string &gender, const std::string &phone, 
        const std::string &profession, const std::string &school, const Location &loc, 
        bool premium, int minAge, int maxAge, int maxDist, const std::string &genderPref, 
        const std::vector<std::string> &likes);

	User* next_;

	std::string name_;
	int age_;
	std::string gender_;
	std::string phone_number_;
	std::string profession_;
	std::string school_;
	Location location_;
	bool isPremium_;
	int minAge_pref_;
	int maxAge_pref_;
	int maxDist_;
	std::string gender_pref_;
	std::vector<std::string> likes_;
	std::vector<std::string> blocked_;

    bool matchesPreferences(const User &other) const;
    bool hasLiked(const std::string &phone_number) const;
	bool isBlocked(const std::string &phone_number) const;
    
	void blockUser(const std::string &phone_number) { blocked_.push_back(phone_number);}
};

std::ostream& operator<<(std::ostream &out, User &u);

#endif