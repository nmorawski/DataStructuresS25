// WordSearchSolver.h
#ifndef WORD_SEARCH_SOLVER_H
#define WORD_SEARCH_SOLVER_H

#include "WordSearchGrid.h"
#include "WordSearchPuzzle.h"
#include <vector>

class WordSearchSolver {
private:
    WordSearchPuzzle puzzle;
    std::vector<WordSearchGrid> solutions;
    bool findAllSolutions;
    
    // Recursive function to fill the grid and find solutions
    bool fillGrid(WordSearchGrid& grid, int row, int col);
    
public:
    // Constructor
    WordSearchSolver(const WordSearchPuzzle& p, bool findAll);
    
    // Solve the puzzle
    void solve();
    
    // Get the solutions
    const std::vector<WordSearchGrid>& getSolutions() const;
};

#endif