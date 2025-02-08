#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>

#include "business.h"

// The getStringValue() is just a modification to the getName() function we were given. I changed
// the function to accept any string as a query value, and it will accurately return the 
// desired information.
std::string getStringValue(std::string& line, std::string elem){
    int start = 0;
    int key_start = line.find(elem, start);
    int value_start = key_start + elem.size()+3;

    int value_end = line.find("\"", value_start);
    int len = value_end - value_start;

    std::string name = line.substr(value_start, len);
    return name;
}

// The given getRating() function
double getRating(std::string& line){
    int start = 0;
    // starting from the position start, and search the string variable line,
    // to find the first stars string.
    int key_start = line.find("stars", start);
    int value_start = key_start + 7;
    // remember to use escape.
    int value_end = line.find("\"", value_start);
    // -1 here because this is not a string.
    int len = value_end - value_start - 1;
    // go from start to end, but exclude the character at end.
    // when we use the substr(start, length) function on a std::string,
    // the substring includes the character at the start position,
    // and the length of the substring is length.
    // It does not include the character at the position start + length.
    // convert this string to a double
    double stars = stod(line.substr(value_start, len));
    return stars;
}

// The given getPrice() function. other than 1 small modification; I noticed that in some
// instances, the value retrieved is "None", rather than a number. This messes with the called
//std::stoi, so I included an if statement to check
int getPrice(std::string& line){
    int start = 0;
    // starting from the position start, and search the string variable line,
    // to find the first RestaurantsPriceRange2 string.
    int key_start = line.find("RestaurantsPriceRange2", start);
    // if not found, return now
    if(static_cast<size_t>(key_start) == std::string::npos){
        return 0;
    }
    int value_start = key_start + 25;
    // remember to use escape.
    int value_end = line.find("\"", value_start);
    // -1 here because this is not a string.
    int len = value_end - value_start;
    // go from start to end, but exclude the character at end.
    // when we use the substr(start, length) function on a std::string,
    // the substring includes the character at the start position,
    // and the length of the substring is length.
    // It does not include the character at the position start + length.
    // convert this string to an integer
    std::string val = line.substr(value_start, len);
    if (val=="None") return 0;
    int price = stoi(val);
    return price;
}

// The given getPrice() function, just slightly modified to return the number of reviews listed
int getReviewCount(std::string& line){
    int start = 0;

    int key_start = line.find("review_count", start);
    // if not found, return now
    if(static_cast<size_t>(key_start) == std::string::npos){
        return 0;
    }
    int value_start = key_start + 14;
    // remember to use escape.
    int value_end = line.find("\"", value_start);
    // -1 here because this is not a string.
    int len = value_end - value_start-1;
    // go from start to end, but exclude the character at end.
    // when we use the substr(start, length) function on a std::string,
    // the substring includes the character at the start position,
    // and the length of the substring is length.
    // It does not include the character at the position start + length.
    // convert this string to an integer
    int price = stoi(line.substr(value_start, len));
    return price;
}

// The function parse_categories() takes in the string of categories retrieved from the input
// file, along with an empty list. It parses the string by a delimiter, and stores each result
// in the list. If the category string has a space character in front, it is removed.
void parse_categories(const std::string line, std::list<std::string> &lst){
    std::stringstream ss(line);
    std::string category;

    while (getline(ss, category, ',')) { // Delimit the string by the ',' character
        if (category.size() > 0){ // Don't include empty characters
            if (category[0] == ' ') category = category.substr(1,category.length());
            lst.push_back(category);
        }
    }
}

// Takes in the input file, retrives the necessary information, and stores the info into a 
// Business class object. The class object is then added to the list
int parse_file(std::ifstream &json, std::list<Business> &business_list){
    if (json.is_open()){//If the file is open
        std::string row;
        while (json.good()){//While there is text to retrieve. store each line in the list
            getline(json, row);
            if (row != ""){
                std::list<std::string> category_list; //Categories
                parse_categories(getStringValue(row,"categories"),category_list);
                business_list.push_back(Business(getStringValue(row,"name"), getStringValue(row,
                    "city"), getStringValue(row,"postal_code"), getRating(row),getReviewCount(row),
                    getPrice(row), category_list));
            }
        }
    } else {
        std::cerr << "Unable to open input json file" <<std::endl;
        return 1;
    }
    return 0;
}

// Function used to assist in the performance of the filter_businesses() function. Takes in a 
// Business class object and a list of categories, then returns a boolean if at least one of the
// categories exist in th class object.
bool check_category_match(Business &business, std::list<std::string> &query_lst){
    std::list<std::string>::iterator q_itr = query_lst.begin();
    while (q_itr != query_lst.end()){
        if (business.checkCategory((*q_itr)))
            return true;
        q_itr++;
    }
    return false;
}

// The filter_businesses() function takes in the list of all Yelp businesses and filters by two
// details: (1) if the zip code matches, and (2) if one of the desired categories applies to the
// business. If one of these two parameters are not met, the business is removed from the list
void filter_businesses(std::list<Business> &lst, const std::string zip_code, 
    std::list<std::string> &categories){
    std::list<Business>::iterator p = lst.begin();
    while (p != lst.end()){
        if (p->getZipCode() != zip_code)
            p = lst.erase(p);
        else if (!check_category_match(*p,categories))
            p = lst.erase(p);
        else
            ++p;
    }
}

int main(int argc, char* argv[]){
    if (argc >= 5){
        std::list<Business> yelp_businesses;
        std::ifstream input(argv[1]);
        std::ofstream output(argv[2]);
        std::string zip_code(argv[3]);
        std::list<std::string> searched_categories;
        for (int i = 4; i < argc; i++)
            searched_categories.push_back(argv[i]);

        parse_file(input,yelp_businesses); //Parses input
        
        //Determines related businesses, removes inadequate
        filter_businesses(yelp_businesses, zip_code, searched_categories);

        yelp_businesses.sort(compareByRating); //Sort by rating, descending order

        if (output.is_open()){ //Output
            if (yelp_businesses.begin() == yelp_businesses.end()) //List is empty, aka, no matches
                output << "Sorry, we couldn't find any results"<<std::endl;
            else {
                output << std::string(21, '=') << std::endl;
                int i = 1;
                std::list<Business>::iterator itr = yelp_businesses.begin();
                while (itr != yelp_businesses.end()) {
                    output << i << ". " << *itr;
                    i++;
                    itr++;
                }
            }
            output.close();
        } else {
            std::cerr << "Unable to open output file" <<std::endl;
            return 1;
        }
    } else {
        std::cerr << "Incorrect amount of command line arguments." <<std::endl;
        return 1;
    }
    return 0;
}