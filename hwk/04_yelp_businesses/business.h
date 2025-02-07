#ifndef __BUSINESS_H
#define __BUSINESS_H

#include <iostream> 
#include <fstream> 
#include <string> 
#include <list>

class Business {
public:
Business(std::string name, std::string city, std::string zip_code, double rating, 
    unsigned int review_count, unsigned int price, std::list<std::string> categories);
std::string getName() const { return name_; }
std::string getCity() const { return city_; }
unsigned int getCount() const { return review_count_; }
unsigned int getPrice() const { return price_; }
std::string getPhone() const { return zip_code_; }
double getRating() const { return rating_; }
std::list<std::string> getCategories() const { return categories_; }

bool checkCategory(const std::string category);
// MODIFIERS
void setMatchState() { category_match_ = !category_match_; }

private: // REPRESENTATION (member variables)
std::string name_;
double rating_;
unsigned int review_count_;
unsigned int price_;
std::string zip_code_;
std::string city_;
std::list<std::string> categories_;
bool category_match_;
};

std::ostream& operator<<(std::ostream& out, Business& b);
bool compareByRating(const Business& b1, const Business& b2);

#endif

/*
bool Matrix::operator==(const Matrix& m2){ //Check if two matrices are the same
    if (rows_ != m2.rows_ || columns_ != m2.columns_) //If dimensions differ
        return false;
    else {
        for (unsigned int i = 0; i < rows_; i++){ //Iterate over each cell, look for differences
            for (unsigned int j = 0; j < columns_; ++j){
                if (data[i][j] != m2.data[i][j])
                    return false;
            }
        }
    }
    return true;
}

*/