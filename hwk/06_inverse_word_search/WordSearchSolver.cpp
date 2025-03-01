// WordSearchSolver.cpp
#include "WordSearchSolver.h"

WordSearchSolver::WordSearchSolver(const WordSearchPuzzle& p, bool findAll) 
    : puzzle(p), findAllSolutions(findAll) {
}

bool WordSearchSolver::fillGrid(WordSearchGrid& grid, int row, int col) {
    int width = puzzle.getWidth();
    int height = puzzle.getHeight();
    
    // If we've filled the entire grid, check if it's valid
    if (row == height) {
        if (puzzle.isValidGrid(grid)) {
            // Save this solution
            solutions.push_back(grid);
            return true;
        }
        return false;
    }
    
    // Calculate next position
    int nextCol = (col + 1) % width;
    int nextRow = row + (nextCol == 0 ? 1 : 0);
    
    // Try each possible letter in the current cell
    for (char letter = 'a'; letter <= 'z'; letter++) {
        grid.setCell(row, col, letter);
        
        // Recursively fill the rest of the grid
        bool foundSolution = fillGrid(grid, nextRow, nextCol);
        
        // If we only need one solution and found it, return
        if (foundSolution && !findAllSolutions) {
            return true;
        }
    }
    
    return false;
}

void WordSearchSolver::solve() {
    WordSearchGrid grid(puzzle.getWidth(), puzzle.getHeight());
    fillGrid(grid, 0, 0);
}

const std::vector<WordSearchGrid>& WordSearchSolver::getSolutions() const {
    return solutions;
}