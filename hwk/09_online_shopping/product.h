#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
public:
    std::string title;
    double rating;
    int num_ratings;
    std::string thumb_url;
    double price;

    Product(); // Default constructor
    //~Product(); // Destructor
    bool operator<(const Product& other) const;
};

#endif