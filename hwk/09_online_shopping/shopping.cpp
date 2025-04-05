#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "product.h"

// Order outputs by higher rating, then cheaper cost, then alphabetically
/*
GE WB02X11544 Charcoal Filter title
5 average_rating
2 rating_number
https://m.media-amazon.com/images/I/514JsH5T-CL._AC_US75_.jpg thumb
$25 price
*/

//============================ HELPER FUNCTIONS TO AID JSON PARSING ==============================
// Helper function to extract strings
std::string extract_string(const std::string &line, const std::string &key) {
    std::string::size_type key_pos = line.find("\"" + key + "\":");
    if (key_pos == std::string::npos) return "";
    
    std::string::size_type start = line.find("\"", key_pos + key.length() + 2);
    if (start == std::string::npos) return "";
    start++;
    
    std::string::size_type end = line.find("\"", start);
    if (end == std::string::npos) return "";
    
    return line.substr(start, end - start);
}

// Helper function to extract integers
int extract_int(const std::string &line, const std::string &key) {
    std::string::size_type key_pos = line.find("\"" + key + "\":");
    if (key_pos == std::string::npos) return -1;
    
    unsigned int start = key_pos + key.length() + 3;
    std::string::size_type end = line.find(",", start);
    if (end == std::string::npos) return 0;
    
    std::string value = line.substr(start, end - start);
    return std::stoi(value);
}

// Helper function to extract doubles
double extract_double(const std::string &line, const std::string &key) {
    std::string::size_type key_pos = line.find("\"" + key + "\":");
    if (key_pos == std::string::npos) return -1;
    
    unsigned int start = key_pos + key.length() + 3;
    std::string::size_type end = line.find(",", start);
    if (end == std::string::npos) return 0;
    
    std::string value = line.substr(start, end - start);
    std::cout << value <<std::endl;
    return std::stod(value);
}
//================================================================================================

// This function takes in a line from a given JSON file as a string. It creates a Product object 
// and then stores all of the necessary values via the above helper functions.
Product parse_product(const std::string& line) {
    Product test;

    test.title = extract_string(line,"title");
    test.thumb_url = extract_string(line,"thumb");
    test.num_ratings = extract_int(line,"rating_number");
    test.rating = extract_double(line,"average_rating");
    test.price = extract_double(line,"price");
    
    return test;
}

void parse_json(const std::string &input, std::vector<Product> &products/*, BPlusTree &product_tree*/){
    
    std::ifstream input_file(input);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open JSON file." << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(input_file, line)) {
        Product product = parse_product(line);
    }
    
    input_file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Incorrect number of arguments" << std::endl;
        return 1;
    }
    
    std::string json = argv[1];
    std::string output_file = argv[2];
    double min_price = std::stod(argv[3]);
    double max_price = std::stod(argv[4]);
    
    std::vector<Product> products;
    //BPlusTree amazon_products;
    
    parse_json(json, products/*, amazon_products*/); // create product objects, create B+ tree
    
    // Perform range search
    //std::vector<Product> results = amazon_products.rangeSearch(min_price, max_price);
    
    //display_products(output_file, results);
    
    return 0;
}