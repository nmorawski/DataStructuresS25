#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> compressText(std::vector<std::string> &input){
    std::vector<std::string> output;
    if (input.size() > 0){ //Changed from output to input
        std::string current = input[0];
        std::string saved = input[0];
        int count = 0;
        for (unsigned int i =0; i < input.size(); i++) {
            current = input[i];
            if (current == saved) {
                count++;
            } else {
                if (count > 1)
                    output.push_back(saved + " "+ std::to_string(count));
                else
                    output.push_back(saved);
                count = 1;
                saved = current;
            }
        }
        if (count > 1)
            output.push_back(current + " "+ std::to_string(count));
        else
            output.push_back(current);
    }
    return output;
}

int main(){
    std::vector<std::string> test;
    test.push_back("hello");
    test.push_back("hello");
    test.push_back("world");
    test.push_back("world");
    test.push_back("world");
    test.push_back("apple");

    std::vector<std::string> output = compressText(test);

    for (unsigned int i = 0; i < output.size(); i++)
        std::cout << output[i] << std::endl;
    return 0;
}