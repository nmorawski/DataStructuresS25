// inverse_word_search.cpp
#include <iostream>
#include <fstream>
#include <string>
#include "WordSearchGrid.h"
#include "WordSearchPuzzle.h"
#include "WordSearchSolver.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " input_file output_file [one_solution|all_solutions]" << std::endl;
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string solutionMode = argv[3];
    bool findAllSolutions = (solutionMode == "all_solutions");
    
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error: Could not open input file " << inputFile << std::endl;
        return 1;
    }
    
    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Error: Could not open output file " << outputFile << std::endl;
        return 1;
    }
    
    int width, height;
    inFile >> width >> height;
    
    std::vector<std::string> requiredWords;
    std::vector<std::string> forbiddenWords;
    
    char type;
    std::string word;
    
    // Read words from the input file
    while (inFile >> type >> word) {
        if (type == '+') {
            requiredWords.push_back(word);
        } else if (type == '-') {
            forbiddenWords.push_back(word);
        }
    }
    
    // Create puzzle and solver
    WordSearchPuzzle puzzle(width, height, requiredWords, forbiddenWords);
    WordSearchSolver solver(puzzle, findAllSolutions);
    
    // Solve the puzzle
    solver.solve();
    
    // Get the solutions
    const std::vector<WordSearchGrid>& solutions = solver.getSolutions();
    
    // Output results
    if (solutions.empty()) {
        outFile << "No solutions found" << std::endl;
    } else if (findAllSolutions) {
        outFile << solutions.size() << " solution(s)" << std::endl;
        for (size_t i = 0; i < solutions.size(); i++) {
            outFile << std::endl << "Solution " << i+1 << ":" << std::endl;
            solutions[i].print(outFile);
        }
    } else {
        // Just print the first solution
        solutions[0].print(outFile);
    }
    
    inFile.close();
    outFile.close();
    
    return 0;
}