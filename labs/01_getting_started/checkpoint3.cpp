#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]){
    std::ifstream input(argv[1]);
    std::ifstream output(argv[2]);
    std::string row;

    if (input.is_open()){//If the file is open
        while (input.good()){//While there is text to retrieve. store each line in the vector
            getline(input,row);
            std::cout << row <<"\n"<< std::endl;
        }
        input.close();
    } else {
        std::cerr << "Unable to open input file" <<std::endl;
        return 1;
    }

}