#ifndef __BUSINESS_H
#define __BUSINESS_H

#include <iostream> 
#include <fstream> 
#include <string> 
#include <list>
#include <algorithm> // for std::transform
#include <cctype> 

class Business {
public:
// Constructor
Business(std::string name, std::string city, std::string zip_code, double rating, 
    unsigned int review_count, unsigned int price, std::list<std::string> categories);
// Accessors
std::string getName() const { return name_; }
std::string getCity() const { return city_; }
unsigned int getCount() const { return review_count_; }
unsigned int getPrice() const { return price_; }
std::string getZipCode() const { return zip_code_; }
double getRating() const { return rating_; }
std::list<std::string> getCategories() const { return categories_; }

bool checkCategory(const std::string category);

private: // REPRESENTATION (member variables)
std::string name_;
double rating_;
unsigned int review_count_;
unsigned int price_;
std::string zip_code_;
std::string city_;
std::list<std::string> categories_;
};

std::ostream& operator<<(std::ostream& out, Business& b);

bool compareByRating(const Business& b1, const Business& b2);

#endif

