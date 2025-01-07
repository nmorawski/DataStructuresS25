#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[]){
    std::vector<int> integers;

    for (int i = 1; i < argc;i++){
        if (!isalpha(argv[i][0]))
            integers.push_back(atoi(argv[i]));
    }
    //Product Calculation
    int product = 1;
    for(int i = 0; i < integers.size(); i++)
        product *= integers[i];
    std::cout << "product of integers: "<< product << std::endl;

}