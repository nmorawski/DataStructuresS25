#include <map>
#include <vector>
#include <iostream>


int main(int argc, char* argv[]){
    std::vector<int> inputs;
    for (int i = 1; i < argc; i++){
        int num = std::atoi(argv[i]);
        inputs.push_back(num);
    }
    std::map<int, int> modeCount;
    int inputs_size = inputs.size();
 
    for(int i = 0; i < inputs_size; ++i)
        modeCount[inputs[i]]++;

    unsigned int max_mode_value = 0;

    for (std::map<int, int>::const_iterator itr = modeCount.begin(); itr != modeCount.end(); itr++){
        unsigned int map_value = itr->second;
        if (map_value > max_mode_value){
            max_mode_value = itr->second;
        }
    }
    std::cout << "Modes: ";
    for (std::map<int, int>::const_iterator itr = modeCount.begin(); itr != modeCount.end(); itr++){
        unsigned int map_value = itr->second;
        if (map_value == max_mode_value){
            std::cout << itr->first<<std::endl;
        }
    }
    return 0;
}