#include <iostream> //Includes functions of program
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Includes lamda functions, sort, etc.
#include <sstream> // Includes stringstream functions
#include <cctype>

//This function splits a string up by a delimiter (in this case, '#')
//Stores all possible words in a vector
std::vector<std::string> split_and_check(const std::string &s, char delim) {
    std::vector<std::string> results;
    std::stringstream ss(s);
    std::string word;

    while (getline (ss, word, delim)) {
        if (word.size() > 1)
            results.push_back(word);
    }

    return results;
}
//This fucntion uses split_and_check to pull all possible words from the crossword
//All of these words are then checked against the dictionary
//If at least one word doesnt appear in the dictionary, it is flagged as an invalid crossword
bool isCrossword(std::vector<std::string> input, std::vector<std::string> diction, std::vector<std::string> &issues, 
    std::vector<std::vector<std::string>> &answers){
    char delimiter = '#';
    std::vector<std::string> temp;

    for (unsigned int i = 0; i < input.size(); i++){ //checks words ACROSS
        temp = split_and_check(input[i],delimiter);
        answers[0].insert(answers[0].end(),temp.begin(),temp.end());
    }

    for (unsigned int i = 0; i < input[0].size(); i++){ //checks words DOWN
        std::string column = "";
        for (unsigned int j = 0; j < input.size(); j++)
            column += input[j][i];
        temp = split_and_check(column,delimiter);
        answers[1].insert(answers[1].end(),temp.begin(),temp.end());
    }

    for (std::string ans : answers[0]){//All ACROSS words
        bool inDict = false;
        for (std::string option : diction){
            if (ans == option)
                inDict = true;
        }
        if (!inDict)
            issues.push_back(ans);
    }

    for (std::string ans : answers[1]){//All DOWN words
        bool inDict = false;
        for (std::string option : diction){
            if (ans == option)
                inDict = true;
        }
        if (!inDict)
            issues.push_back(ans);
    }
    
    std::sort(issues.begin(),issues.end());//Alphabeticalizes all of the invalid words
    if (issues.size() > 0)
        return false;
    else
        return true;
}

//Creates the ASCII art to match the given crossword.
//Would need to be expanded for extra credit
void ascii_art(std::vector<std::string> input, std::vector<std::string> &ascii){
    std::string break_row = "+";
    for (unsigned int i = 0; i < input[0].size();i++)
        break_row += "----+";
    ascii.push_back(break_row);

    for (unsigned int j = 0; j < input.size(); j++){
        std::string content_row =  "|";
        for (unsigned int k = 0; k < input[j].size(); k++){
            if (isalpha(input[j][k])){
                content_row += "    |";
            } else if (input[j][k] == '#'){
                content_row += "####|";
            }
        }
        ascii.push_back(content_row);
        ascii.push_back(content_row);
        ascii.push_back(break_row);
    }
}
//This function has two main nested for loops, one for all of the ACROSS words, and one for DOWN words
//Store the starting coordinate for each word in a similarly structured vector
void find_coords(std::vector<std::string> input, std::vector<std::vector<std::string>> &answers, std::vector<std::vector<std::string>> &coords){
    for(unsigned int i = 0; i < answers[0].size(); i++){ //Loops through all ACROSS words
        for(unsigned int j = 0; j < input.size(); j++){ //Loops through each row
            for(unsigned int loc = input[j].size(); loc >= answers[0][i].size();loc--){//We cant use std::find, so I'm looping substrings through each row
                std::string substring = input[j].substr((input[j].size() - loc),answers[0][i].size());
                if (substring == answers[0][i]){
                    std::string x_coord = std::to_string(j);
                    std::string y_coord = std::to_string(input[j].size() - loc);

                    std::string coordinate = "(" + x_coord + "," + y_coord + ")";
                    coords[0].push_back(coordinate);
                }
            }
        }
    }
    std::vector<std::string> input_rotated;
    for (unsigned int i = 0; i < input[0].size(); i++){ //collects columns
        std::string column = "";
        for (unsigned int j = 0; j < input.size(); j++)
            column += input[j][i];
        input_rotated.push_back(column);
    }

    for(unsigned int i = 0; i < answers[1].size(); i++){ //Loops through all DOWN words
        for(unsigned int j = 0; j < input_rotated.size(); j++){ //Loops through each row
            for(unsigned int loc = input_rotated[j].size(); loc >= answers[1][i].size();loc--){//We cant use std::find, so I'm looping substrings through each row
                std::string substring = input_rotated[j].substr((input_rotated[j].size() - loc),answers[1][i].size());
                if (substring == answers[1][i]){
                    std::string y_coord = std::to_string(j);
                    std::string x_coord = std::to_string(input_rotated[j].size() - loc);

                    std::string coordinate = "(" + x_coord + "," + y_coord + ")";
                    coords[1].push_back(coordinate);
                 }
            }
        }
    }
}

int main(int argc, char* argv[]){
    std::string row;//For parsing files
    std::vector<std::string> crossword;//To store inputted crossword
    std::vector<std::string> dict_words;//To store dictionary
    std::vector<std::string> errors;//All error words

    std::vector<std::vector<std::string>> words(2);//All words in crossword

    std::ifstream input(argv[1]);
    std::ifstream dictionary(argv[2]);

    
    if (input.is_open()){//If the file is open
        while (input.good()){//While there is text to retrieve. store each line in the vector
            getline(input,row);
            if (row != "")
                crossword.push_back(row);
        }
        input.close();
    } else {
        std::cerr << "Unable to open input file" <<std::endl;
        return 1;
    }

    if (dictionary.is_open()){//If the file is open
        while (dictionary.good()){//While there is text to retrieve. store each line in the vector
            getline(dictionary,row);
            std::for_each(row.begin(), row.end(), [](char & c) {
                c = ::tolower(c);
            });
            dict_words.push_back(row);
        }
        dictionary.close();
    } else {
        std::cerr << "Unable to open input file" <<std::endl;
        return 1;
    }

    if (argc == 3){
        if (isCrossword(crossword,dict_words,errors,words)){//Check to see if valid crossword
            std::cout << "valid crossword puzzle" << std::endl;
        } else {
            for (unsigned int i = 0; i < errors.size(); i++)
                std::cout << "'" << errors[i] << "'" << " is not a word" << std::endl;
        }
        //Content of normal run
    } else if (argv[3] == std::string("--print")){
        if (argc != 4){
            std::cerr << "Incorrect amount of command line arguments." << std::endl;
            return 1;
        } else {
           //content of --print run
            std::vector<std::string> field;
            ascii_art(crossword, field);
            for (unsigned int i = 0; i < field.size();i++)
                std::cout << field[i] << std::endl;
        }
    } else if (argv[3] == std::string("--print_coordinates")){
        if (argc != 4){
            std::cerr << "Incorrect amount of command line arguments." << std::endl;
            return 1;
        } else {
            if (isCrossword(crossword,dict_words,errors,words)){//Check to see if valid crossword
                std::vector<std::string> field;
                ascii_art(crossword, field);
                for (unsigned int i = 0; i < field.size();i++)
                    std::cout << field[i] << std::endl;

                std::vector<std::vector<std::string>> coordinates(2);
                find_coords(crossword, words, coordinates);

                std::cout << std::endl;//Print out all valid words and their coords
                for (unsigned int i = 0; i < words[0].size();i++)
                    std::cout << coordinates[0][i] << " ACROSS "<< words[0][i] << std::endl;
                for (unsigned int j = 0; j < words[1].size();j++)
                    std::cout << coordinates[1][j] << " DOWN "<< words[1][j] << std::endl;
            } else {
                for (unsigned int i = 0; i < errors.size(); i++)
                    std::cout << "'" << errors[i] << "'" << " is not a word" << std::endl;
            }
        }
    } else if (argv[3] == std::string("--print_numbered")){//E.C.
        if (argc != 4){
            std::cerr << "Incorrect amount of command line arguments." << std::endl;
            return 1;
        } else {
           //content of --print_numbered run
           std::cout << "To be continued..." << std::endl;
        }
    } else {
        std::cerr << "Invalid operation given." << std::endl;
        return 1;
    }
}