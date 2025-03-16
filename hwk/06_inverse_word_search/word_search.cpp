#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

typedef std::vector<std::string> Puzzle;

//Directions around a given character, starting at 12 o'clock, working clockwise
std::vector<std::pair<int,int>> directions = {
    {-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}
};

// The parse_input function takes in an input file and two vectors of strings, one
// that stores the necessary words for the puzzle, and the other stores all of the
// words that should not be included. Also, an empty puzzle is created from the given
// dimensions.
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
    Puzzle puzzle(row, std::string(column, '.'));
    return puzzle;
}

// The find function takes in a given puzzle and a word, and is tasked with locating
// the given word. First, locate the first letter of the given word. Then we check all
// possible directions to see if we can find the word.
bool find(const Puzzle& puzzle, const std::string &word) {
    for (int row = 0; row < puzzle.size(); row++) {
        for (int col = 0; col < puzzle[0].size(); col++) {
            if (puzzle[row][col] == word[0]) {
                // Check all 8 directions
                for (const std::pair<int,int>& dir : directions) {
                    int dy = dir.first, dx = dir.second;
                    bool found = true;
                    
                    // Check if whole word fits in this direction
                    for (int i = 1; i < word.size(); i++) {
                        int newRow = row + i * dy;
                        int newCol = col + i * dx;
                        
                        // Check if location passes puzzle's bounds
                        if (newRow < 0 || newRow >= puzzle.size() || newCol < 0 || 
                            newCol >= puzzle[0].size() || puzzle[newRow][newCol] != word[i]) {
                            found = false;
                            break;
                        }
                    }
                    
                    if (found) return true;
                }
            }
        }
    }
    return false;
}

// Helper function used to find the forbidden words
bool find_forbidden_words(const Puzzle& puzzle, const std::vector<std::string> &forbidden) {
    // Use the optimized find function above
    for (const std::string& word : forbidden) {
        if (find(puzzle, word)) return true;
    }
    return false;
}

// Check to see if the puzzle has any empty spots
bool is_full(const Puzzle& puzzle) {
	for (unsigned int i = 0; i < puzzle.size(); i++) {
		for (unsigned int j = 0; j < puzzle[0].size(); j++) {
			if (puzzle[i][j] == '.') return false;
		}
	}
	return true;
}

// The add_word function takes in the given puzzle, the word, the starting coords for the word, and
// the direction the word is facing. We check first to make sure the word can validly go in the
// desired direction, then add it to the puzzle
bool add_word(Puzzle& puzzle, const std::string &word, int startRow, int startCol, int dy, int dx){
    // First check if the word would fit in this direction before modifying the puzzle
    for (int i = 0; i < word.size(); i++) {
        int row = startRow + i * dy;
        int col = startCol + i * dx;
        
        if (row < 0 || row >= puzzle.size() || col < 0 || col >= puzzle[0].size() ||
            (puzzle[row][col] != '.' && puzzle[row][col] != word[i])) {
            return false;
        }
    }
    
    // Now place the word
    for (int i = 0; i < word.size(); i++) {
        puzzle[startRow + i * dy][startCol + i * dx] = word[i];
    }
    return true;
}

// Helper function to try adding a word at a specific position in all directions
std::vector<Puzzle> add_word(const Puzzle &puzzle, const std::string &word, int row, int col){
    std::vector<Puzzle> result;
    for (const std::pair<int,int>& dir : directions) {
        Puzzle new_puzzle = puzzle;
        if (add_word(new_puzzle, word, row, col, dir.first, dir.second)) {
            result.push_back(new_puzzle);
        }
    }
    return result;
}

// This function is to fill out any empty spots with all possible characters. There's a lot of
// state checking because this was the fucntion that requires the most optimization.
bool fill_spaces(Puzzle &puzzle, std::vector<Puzzle> &solutions, 
                           const std::vector<std::string> &forbidden, 
                           const std::string &command, int row, int col) {
    // If we've filled the entire puzzle
    if (row >= puzzle.size()) {
        if (!find_forbidden_words(puzzle, forbidden)) {
            if (command == "one_solution") {
                solutions.push_back(puzzle);
                return true; // We found one solution, so we can stop
            } else { // all_solutions
                std::vector<Puzzle>::iterator it = find(solutions.begin(), solutions.end(),puzzle);
                if (it == solutions.end()) solutions.push_back(puzzle);
            }
        }
        return false;
    }
    
    // Calculate next position
    int next_row = row;
    int next_col = col + 1;
    if (next_col >= puzzle[0].size()) {
        next_row++;
        next_col = 0;
    }
    
    // If this cell is already filled, move to the next one
    if (puzzle[row][col] != '.') {
        return fill_spaces(puzzle, solutions, forbidden, command, next_row, next_col);
    }
    
    // Try each letter
    for (char c = 'a'; c <= 'z'; c++) {
        puzzle[row][col] = c;
        
        // Quick check to see if we've already created a forbidden word
        bool forbidden_found = false;
        for (const std::string& word : forbidden) {
            if (word.size() <= std::max(row, col) + 1) { // Only check if word could fit so far
                // Check the paths that include our current position
                // This is a simplified check - could be made more precise
                if (find(puzzle, word)) {
                    forbidden_found = true;
                    break;
                }
            }
        }
        
        if (!forbidden_found) {
            if (fill_spaces(puzzle, solutions, forbidden, command, next_row, next_col)) {
                return true; // For one_solution
            }
        }
    }
    
    // Backtrack
    puzzle[row][col] = '.';
    return false;
}

// Recursive function used to populate and find all possible puzzles. Iterate through
// all possible allowed words, and find all possible orientations
bool generate_puzzles(Puzzle &puzzle, std::vector<Puzzle> &solutions, 
    const std::vector<std::string> &words, const std::vector<std::string> &forbidden,
    int word_index, const std::string &command) {
    
    // All words placed, now fill in remaining spaces
    if (word_index >= words.size()) {
        if (is_full(puzzle)) {
            if (!find_forbidden_words(puzzle, forbidden)) {
                if (command == "one_solution") {
                    solutions.push_back(puzzle);
                    return true;
                } else {
                    std::vector<Puzzle>::iterator it = find(solutions.begin(), solutions.end(),
                        puzzle);
                    if (it == solutions.end()) solutions.push_back(puzzle);
                }
            }
        } else {
            // Fill remaining spaces
            return fill_spaces(puzzle, solutions, forbidden, command, 0, 0);
        }
        return false;
    }
    
    const std::string &word = words[word_index];
    
    // Try placing this word at every position and direction
    for (int row = 0; row < puzzle.size(); row++) {
        for (int col = 0; col < puzzle[0].size(); col++) {
            if (puzzle[row][col] == '.' || puzzle[row][col] == word[0]) {
                std::vector<Puzzle> possible_puzzles = add_word(puzzle, word, row, col);
                
                for (Puzzle &new_puzzle : possible_puzzles) {
                    if (generate_puzzles(new_puzzle, solutions, words, 
                                                   forbidden, word_index + 1, command)) {
                        return true; // For one_solution mode
                    }
                }
            }
        }
    }
    
    return false;
}
// Print function
void print_puzzle(const Puzzle &puzzle, std::ofstream &output) {
	output << "Board: " << std::endl;
	for (unsigned int i = 0; i < puzzle.size(); i++) {
		output << "  ";
		for (unsigned int j = 0; j < puzzle[0].size(); j++)
			output << puzzle[i][j];
		output << std::endl;
	}
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << " Incorrect number of commands" << std::endl;
        return 1;
    }
    
    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);
    std::string command(argv[3]);
    
    if (!input.is_open()) {
        std::cerr << "Unable to open input file" << std::endl;
        return 1;
    }
    
    if (!output.is_open()) {
        std::cerr << "Unable to open output file" << std::endl;
        return 1;
    }
    
    std::vector<std::string> included_words, forbidden_words;
    Puzzle puzzle = parse_input(input, included_words, forbidden_words);

    // Sort included words by size, decreasing order
    std::sort(included_words.begin(), included_words.end(), 
              [](const std::string &a, const std::string &b) { 
                  return a.size() > b.size(); });
    
    std::vector<Puzzle> solutions;
    generate_puzzles(puzzle, solutions, included_words, forbidden_words, 0, command);
    
    if (solutions.empty()) {
        output << "No solutions found." << std::endl;
    } else if (command == "one_solution") {
        print_puzzle(solutions[0], output);
    } else { // all_solutions
        output << solutions.size() << " solution(s)" << std::endl;
        for (unsigned int i = 0; i < solutions.size(); i++)
            print_puzzle(solutions[i], output);
    }
    
    return 0;
}