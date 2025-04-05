#include "product.h"

Product::Product() { // Default constructor
    rating = 0;
    num_ratings = 0;
    price = 0;
}

// Comparison operator for sorting
bool Product::operator<(const Product& other) const {
    if (rating != other.rating)
        return rating > other.rating;
    
    if (price != other.price)
        return price < other.price;

    return title < other.title;
}