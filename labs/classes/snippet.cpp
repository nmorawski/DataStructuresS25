#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors

int main(int argc, char* argv[]){
    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);


	if (!inputFile.is_open()) {
		std::cerr << "Failed to open the user data file." << std::endl;
		exit(1);
	}

	std::string name, gender, number, profession, school;
	int age;
	double latitude, longitude;
    std::vector<std::string> importantInfo;
	while(inputFile >> name
		>> age
		>> gender
		>> number
		>> profession
		>> school
		>> latitude
		>> longitude){
            //Realistically would include class, store all info there
            importantInfo.push_back(name);
            importantInfo.push_back(number);
		}
    
    if (outputFile.is_open()){
        for (unsigned int i = 0; i < importantInfo.size(); i+=2)
            outputFile << importantInfo[i] << "," << importantInfo[i+1] << "\n";
        outputFile.close();
    } else {
        std::cerr << "Unable to open output file" <<std::endl;
        return 1;
    }
	return 0;
}