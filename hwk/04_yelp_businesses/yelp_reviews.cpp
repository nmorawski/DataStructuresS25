#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <sstream>
#include <list>

#include "business.h"

std::string getStringValue(std::string& line, std::string elem){
    int start = 0;
    int key_start = line.find(elem, start);
    int value_start = key_start + elem.size()+3;

    int value_end = line.find("\"", value_start);
    int len = value_end - value_start;

    std::string name = line.substr(value_start, len);
    return name;
}

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

int getPrice(std::string& line){
    int start = 0;
    // starting from the position start, and search the string variable line,
    // to find the first RestaurantsPriceRange2 string.
    int key_start = line.find("RestaurantsPriceRange2", start);
    // if not found, return now
    if(key_start == std::string::npos){
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
    int price = stoi(line.substr(value_start, len));
    return price;
}

int getReviewCount(std::string& line){
    int start = 0;
    // starting from the position start, and search the string variable line,
    // to find the first RestaurantsPriceRange2 string.
    int key_start = line.find("review_count", start);
    // if not found, return now
    if(key_start == std::string::npos){
            return 0;
    }
    int value_start = key_start + 14;
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
    int price = stoi(line.substr(value_start, len));
    return price;
}

void parse_categories(const std::string line, std::list<std::string> &lst){
    std::stringstream ss(line);
    std::string category;

    while (getline(ss, category, ',')) {
        if (category.size() > 0){ //Don't include empty characters
            if (category[0] == ' ') category = category.substr(1,category.length());
            lst.push_back(category);
        }
    }
}

//# of reviews
int parse_file(std::ifstream &json, std::list<Business> &business_list){
    if (json.is_open()){//If the file is open
        std::string row;
        while (json.good()){//While there is text to retrieve. store each line in the vector
            getline(json, row);
            if (row != ""){
                std::cout << getStringValue(row,"name") << " " << getPrice(row) << " " << getRating(row)<<std::endl;
                //std::cout << "TEST: "<< getStringValue(row,"city") << " " << getStringValue(row,"postal_code") << std::endl;
                std::cout << "TEST: "<< getReviewCount(row) << std::endl;
                std::cout << getStringValue(row,"categories") << std::endl <<std::endl;

                std::list<std::string> category_list;
                parse_categories(getStringValue(row,"categories"),category_list);

                std::cout << "CHECK:"<<std::endl;
                std::list<std::string>::iterator itr = category_list.begin();
                while (itr != category_list.end()){
                    std::cout<<'|'<<*itr<<'|'<<std::endl;
                    itr++;
                }
                business_list.push_back(Business(getStringValue(row,"name"), getStringValue(row,"city"), 
                    getStringValue(row,"postal_code"), getRating(row), getReviewCount(row), getPrice(row), 
                    category_list));
            }
        }
    } else {
        std::cerr << "Unable to open input json file" <<std::endl;
        return 1;
    }
    return 0;
}

/*
The businesses showed in your output file should be sorted based on the rating of the business, 
with higher rated businesses being showed before lower rated businesses. For any two businesses 
which have the same rating, for example, both are 4.5, then in your output file, keep the order 
of these two as they appear in the input file.
*/

int main(int argc, char* argv[]){
    if (argc >= 5){
        std::list<Business> yelp_businesses;
        std::ifstream input(argv[1]);
        std::ofstream output(argv[2]);
        std::string zip_code(argv[3]);
        std::list<std::string> searched_categories;
        for (int i = 4; i < argc; i++)
            searched_categories.push_back(argv[i]);

        parse_file(input,yelp_businesses);
        std::list<Business>::iterator p = yelp_businesses.begin();
        int i = 0;
        while (p != yelp_businesses.end()){
            std::cout << (*p).getName() << std::endl;
            p++;
            std::cout << i << std::endl;
            i++;
        }
    } else {
        std::cerr << "Incorrect amount of command line arguments." <<std::endl;
        return 1;
    }
    // double test = 4.5;
    // for (int i = 1; i  < test; i++)
    //     std::cout << "\u2605";
    // if (int(test*2) % 2 == 1)
    //     std::cout << "\u00BD";
    // std::cout << std::endl;
    return 0;
}