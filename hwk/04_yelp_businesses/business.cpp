#include "business.h"
#include <list>

// Constructor 
Business::Business(std::string name, std::string city, std::string zip_code, double rating, 
    unsigned int review_count, unsigned int price, std::list<std::string> categories){
    name_ = name;
    rating_ = rating;
    review_count_ = review_count;
    price_ = price;
    zip_code_ = zip_code;
    city_ = city;
    categories_ = categories;
}

// This is the overloading of the '<<' operator, which allows for customized printing to the 
// output file. All very straight forward, with minor if statements to handle the inclusion or
// omission of info. I used the fact that a double ending with '0.5',when multiplied by 2, results
// in an odd number to work the logic on when a '1/2' character should be used.
std::ostream& operator<<(std::ostream &out, Business &b){
    out << b.getName() << std::endl;

    // Star rating
    for (int i = 1; i  < b.getRating(); i++) out << "\u2605";
    if (int(b.getRating()*2) % 2 == 1) out << "\u00BD";
    else out << "\u2605";

    // # of reviews
    out << " " << b.getRating() << " (" << b.getCount() << " review";
    if (b.getCount() != 1) out << "s";
    out << ")"<<std::endl;

    // City, and if applicable, pricing scale
    out << b.getCity() << " ";
    if (b.getPrice() > 0) out << std::string(b.getPrice(), '$');
    out << std::endl;

    // Keywords for business
    std::list<std::string> temp = b.getCategories();
    std::list<std::string>::iterator itr = temp.begin();
    out << (*itr);
    ++itr;
    while(itr != temp.end()){
		out << ", " << (*itr);
		itr++;
	}
    out << std::endl;

    out << std::string(21, '=') <<std::endl;

    return out;
}

// Simple function used for sorting list, sorts list in descending order by rating
bool compareByRating(const Business& b1, const Business& b2) {
    return b1.getRating() > b2.getRating();
}

// Helper function to convert a string into all lowercase letters. Imperative for querying
// the category searches to look for subset words
std::string toLowerCase(const std::string& str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    return lower_str;
}

// Function takes in a string that represents a desired category, then searches through all of
// the listed keywords to find a match
bool Business::checkCategory(const std::string category){
    std::list<std::string>::iterator itr = categories_.begin();
    std::string query = toLowerCase(category);

    while (itr != categories_.end()){
        std::string attribute = toLowerCase(*itr);
        if (attribute.find(query) != std::string::npos) return true;
        itr++;
    }

    return false;
}
