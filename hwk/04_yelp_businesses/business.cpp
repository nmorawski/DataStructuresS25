#include "business.h"

Business::Business(std::string name, std::string city, std::string zip_code, double rating, 
    unsigned int review_count, unsigned int price, std::list<string> categories){
    name = name_;
    rating = rating_;
    review_count = review_count_;
    price = price_;
    zip_code = zip_code_;
    city = city_;
    categories = categories_;
    category_match_ = false;
}

bool Business::checkCategory(const std::string category){
    std::list<Business>::iterator itr = categories_.begin();
    while(itr != categories_.end()){
		if (*itr2 == category) return true;
		itr++;
	}
    return false;
}

// This is the overloading of the '<<' operator, which allows for customized printing to the 
// command line.
std::ostream& operator<<(std::ostream &out, Business &b){
    out << b.getName() << std::endl;

    for (int i = 1; i  < b.getRating(); i++) out << "\u2605";
    if (int(b.getRating()*2) % 2 == 1) out << "\u00BD";

    out << " " << b.getRating() << " (" << b.getCount() << " review";
    if (b.getCount() != 1) out << "s";
    out << ")"<<std::endl;

    std::string price_info(b.getPrice(), '$');
    out << b.getCity();
    if (price_info.size() > 0) out << " " << price_info;
    out << std::endl;

    std::list<Business>::iterator itr = b.getCategories().begin();
    ++itr;
    out << *itr;
    while(itr != b.getCategories().end()){
		out << ", " << *itr;
		itr++;
	}
    out << std::endl;

    std::string end_info(21, '='); 
    out << end_info <<std::endl;

    return out;
}
/*
===================== >> ignore, also implement number outside of function
1. AMC Plymouth Meeting Mall 12
★★½ 2.5 (169 reviews)
Plymouth Meeting 
Arts & Entertainment, Cinema
=====================
*/

