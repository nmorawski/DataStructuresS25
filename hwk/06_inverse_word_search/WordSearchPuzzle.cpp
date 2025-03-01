// WordSearchPuzzle.cpp
#include "WordSearchPuzzle.h"

WordSearchPuzzle::WordSearchPuzzle(int w, int h, 
                                 const std::vector<std::string>& required,
                                 const std::vector<std::string>& forbidden) 
    : width(w), height(h), requiredWords(required), forbiddenWords(forbidden) {
}

bool WordSearchPuzzle::isValidGrid(const WordSearchGrid& grid) const {
    // Check if all required words are present
    for (const std::string& word : requiredWords) {
        if (!grid.containsWord(word)) {
            return false;
        }
    }
    
    // Check if any forbidden words are present
    for (const std::string& word : forbiddenWords) {
        if (grid.containsWord(word)) {
            return false;
        }
    }
    
    return true;
}

int WordSearchPuzzle::getWidth() const {
    return width;
}

int WordSearchPuzzle::getHeight() const {
    return height;
}