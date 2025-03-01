// WordSearchPuzzle.h
#ifndef WORD_SEARCH_PUZZLE_H
#define WORD_SEARCH_PUZZLE_H

#include "WordSearchGrid.h"
#include <vector>
#include <string>

class WordSearchPuzzle {
private:
    int width;
    int height;
    std::vector<std::string> requiredWords;
    std::vector<std::string> forbiddenWords;
    
public:
    // Constructor
    WordSearchPuzzle(int w, int h, 
                   const std::vector<std::string>& required,
                   const std::vector<std::string>& forbidden);
    
    // Check if a grid satisfies all constraints
    bool isValidGrid(const WordSearchGrid& grid) const;
    
    // Getters
    int getWidth() const;
    int getHeight() const;
};

#endif