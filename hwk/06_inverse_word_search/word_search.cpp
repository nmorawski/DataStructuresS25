#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<char>> Puzzle;

Puzzle parse_input(std::ifstream &file_input, std::vector<std::string> &included, 
    std::vector<std::string> &forbidden) {
    int row, column;
    std::string word, signal;
    Puzzle empty_puzzle;

    if (file_input.is_open()) {//If the file is open
        file_input >> column >> row;
        while(file_input >> signal >> word) {
            if (signal == "+") included.push_back(word);
            else if (signal == "-") forbidden.push_back(word);
        }
        file_input.close();
    } else {
        std::cerr << "Unable to open input file" <<std::endl;
        return empty_puzzle;
    }
    Puzzle puzzle(row, std::vector<char>(column, '.'));
    return puzzle;
}

bool find(const Puzzle& puzzle, const std::string &word, unsigned int row, unsigned int column, unsigned int indx, int y_direct, int x_direct) {
    std::cout << indx <<std::endl;
    std::cout << row << " "<< column<<std::endl;
    std::cout << puzzle[row][column] << " "<< word[indx]<<std::endl;
    if (puzzle[row][column]==word[indx] && indx==word.size()-1){
        std::cout << "WORKED"<<std::endl;
        return true;
    }

    if (puzzle[row][column]==word[indx]){
        if ((row + y_direct) >= 0 && (column + x_direct) >= 0 && (row + y_direct) < puzzle.size() && (column + x_direct) < puzzle[0].size()) {
            std::cout << "characterchecked: "<< word[indx] <<std::endl;
            return find(puzzle, word, (row + y_direct), (column + x_direct), (indx+1), y_direct, x_direct);
        }
    }
    return false;
}

bool find(const Puzzle& puzzle, const std::string &word, unsigned int row, unsigned int column) {
    if (puzzle[row][column]==word[0]) {
        std::vector<std::pair<int,int>> directions = {{-1,0},{-1,1},{0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}}; //12 0'clock, clockwise
        for (unsigned int i = 0; i < directions.size(); i++) {
            if (find(puzzle, word, row, column, 0, directions[i].first, directions[i].second)) return true;
        }
    }
    if (row == puzzle.size()-1 && column == puzzle[0].size()-1) {
        std::cout << "END"<<std::endl;
        return false; // End of puzzle, not found
    }
    if (row == puzzle.size()-1) return find(puzzle, word, row, (column+1)); // Last row
    if (column == puzzle[0].size()-1) return find(puzzle, word, (row+1), column); // Last column
    else return find(puzzle, word, (row+1), column) || find(puzzle, word, row, (column+1)); // Traverse both
}

bool find_forbidden_words(const Puzzle& puzzle, const std::vector<std::string> &forbidden) {
    for (unsigned int i = 0; i < forbidden.size(); i++) {
		if (find(puzzle, forbidden[i], 0, 0)) return true;
	}
	return false;
}

bool is_full(const Puzzle& puzzle) {
	for (unsigned int i = 0; i < puzzle.size(); i++) {
		for (unsigned int j = 0; j < puzzle[0].size(); j++) {
			if (puzzle[i][j] == '.') return false;
		}
	}
	return true;
}

bool add_word(Puzzle& puzzle, const std::string &word, unsigned int row, unsigned int column, unsigned int indx, int x_direct, int y_direct) {
    if (indx==word.size()) {
        return true;
    }

    if ((row ) >= 0 && (column ) >= 0 && (row ) < puzzle.size() && (column ) < puzzle[0].size()) {
        if (puzzle[row][column] == '.' || puzzle[row][column] == word[indx]) {
            puzzle[row][column] = word[indx];
            return add_word(puzzle, word, (row+y_direct), (column+x_direct), (indx+1), x_direct, y_direct);
        }
    }
    return false;
}

void add_word(Puzzle &puzzle, std::vector<Puzzle> &possible_puzzles, const std::string &word, unsigned int row, unsigned int column) {
    for (unsigned int y = 0; y < 3; y++) {
        for (unsigned int x = 0; x < 3; x++) {
            Puzzle word_added = puzzle;
            if (add_word(word_added, word, row, column, 0, (x-1), (y-1))){
                possible_puzzles.push_back(word_added);
            }
        }
    }
    return;
}

void fill_spaces(Puzzle &default_puzzle, std::vector<Puzzle> &puzzles, const std::vector<std::string> &forbidden, const std::string &command) {
    std::vector<char> alphabet= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

    if (is_full(default_puzzle)) {
        if (!(find_forbidden_words(default_puzzle, forbidden))){
            std::vector<Puzzle>::iterator it = find(puzzles.begin(), puzzles.end(), default_puzzle);
            if (it == puzzles.end()) puzzles.push_back(default_puzzle);
        }

        return ;
    }

    for (unsigned int i = 0; i < alphabet.size(); i++) {
        for (unsigned int j = 0; j < default_puzzle.size(); j++){
            for (unsigned int k = 0; k < default_puzzle[0].size(); k++){
                if (default_puzzle[j][k] == '.') {
                    Puzzle new_puzzle = default_puzzle;
                    new_puzzle[j][k] = alphabet[i];
                    fill_spaces(new_puzzle, puzzles, forbidden, command);
                }
            }
        }
    }
}

void generate_puzzles(Puzzle &default_puzzle, std::vector<Puzzle> &puzzles, const std::vector<std::string> &included, 
    const std::vector<std::string> &forbidden, int indx, const std::string &command) {
    
    if (indx == included.size()) {
        if (find_forbidden_words(default_puzzle, forbidden)) {
            std::cout << "gotcha";
            return;
        } else if (is_full(default_puzzle)) {
            if (command == "all_solutions") puzzles.push_back(default_puzzle);
            else if (command == "one_solution") {
                if (puzzles.size() == 0) puzzles.push_back(default_puzzle);
            }
            return;
        } else {
            fill_spaces(default_puzzle, puzzles, forbidden, command);
            //puzzles.push_back(default_puzzle);
            return;
        }
    }
    for (unsigned int i = 0; i < default_puzzle.size(); i++) {
        for (unsigned int j = 0; j < default_puzzle[0].size(); j++){
            if (default_puzzle[i][j]=='.' || default_puzzle[i][j]==included[indx][0]) {//Possible start
                Puzzle new_puzzle = default_puzzle;
                std::vector<Puzzle> possible_puzzles; 
                add_word(default_puzzle, possible_puzzles, included[indx], i,j);
                //std::cout << possible_puzzles.size() << std::endl;
                for (unsigned int k = 0; k < possible_puzzles.size(); k++) {
                    generate_puzzles(possible_puzzles[k], puzzles, included, forbidden, indx+1, command);
                }
            }
        }
    }
}

void print_puzzle(const Puzzle &puzzle, std::ofstream &output) {
	output << "Board: " << std::endl;
	for (unsigned int i = 0; i < puzzle.size(); i++) {
		output << "  ";
		for (unsigned int j = 0; j < puzzle[0].size(); j++)
			output << puzzle[i][j];
		output << std::endl;
	}
}

int main(int argc, char* argv[]){
    if (argc == 4){
        std::ifstream input(argv[1]); // Input
        std::ofstream output(argv[2]); // Output
        std::string command(argv[3]); // Desired result
        std::vector<std::string> included_words, forbidden_words;

        Puzzle default_puzzle = parse_input(input, included_words, forbidden_words);
        std::vector<Puzzle> puzzles;

        generate_puzzles(default_puzzle, puzzles, included_words, forbidden_words, 0, command);

        if (output.is_open()) {//If the file is open
            if (puzzles.size() == 0) output << "No solutions found." << std::endl;
            else if (command == "one_solution") print_puzzle(puzzles[0], output);
            else if (command == "all_solutions") {
                output << puzzles.size() << " solution(s)" << std::endl;
                for (unsigned int i = 0; i < puzzles.size(); i++) {
                    print_puzzle(puzzles[i], output);
                }
            }
            output.close();
        } else {
            std::cerr << "Unable to open output file" <<std::endl;
            return 1;
        }


    } else {
        std::cerr << "Incorrect amount of command line arguments." <<std::endl;
        return 1;
    }
    return 0;
}