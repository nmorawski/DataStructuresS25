// WordSearchGrid.h
#ifndef WORD_SEARCH_GRID_H
#define WORD_SEARCH_GRID_H

#include <vector>
#include <string>

class WordSearchGrid {
private:
    std::vector<std::vector<char>> grid;
    int width;
    int height;

public:
    // Constructor
    WordSearchGrid(int w, int h);
    
    // Copy constructor
    WordSearchGrid(const WordSearchGrid& other);
    
    // Accessor methods
    char getCell(int row, int col) const;
    void setCell(int row, int col, char letter);
    int getWidth() const;
    int getHeight() const;
    
    // Check if a word exists at a specific position in a given direction
    bool checkWordAtPosition(const std::string& word, int row, int col, int dRow, int dCol) const;
    
    // Check if a word exists anywhere in the grid
    bool containsWord(const std::string& word) const;
    
    // Print the grid
    void print(std::ostream& out) const;
};

#endif