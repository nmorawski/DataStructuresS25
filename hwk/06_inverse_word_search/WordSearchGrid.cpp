// WordSearchGrid.cpp
#include "WordSearchGrid.h"
#include <iostream>

WordSearchGrid::WordSearchGrid(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<char>(width, ' '));
}

WordSearchGrid::WordSearchGrid(const WordSearchGrid& other) : width(other.width), height(other.height) {
    grid = other.grid;
}

char WordSearchGrid::getCell(int row, int col) const {
    return grid[row][col];
}

void WordSearchGrid::setCell(int row, int col, char letter) {
    grid[row][col] = letter;
}

int WordSearchGrid::getWidth() const {
    return width;
}

int WordSearchGrid::getHeight() const {
    return height;
}

bool WordSearchGrid::checkWordAtPosition(const std::string& word, int row, int col, int dRow, int dCol) const {
    // Check if the word extends beyond the grid boundaries
    if (row + (word.length() - 1) * dRow < 0 || row + (word.length() - 1) * dRow >= height ||
        col + (word.length() - 1) * dCol < 0 || col + (word.length() - 1) * dCol >= width) {
        return false;
    }
    
    // Check if the word matches at this position with the given direction
    for (size_t i = 0; i < word.length(); i++) {
        if (grid[row + i * dRow][col + i * dCol] != word[i]) {
            return false;
        }
    }
    
    return true;
}

bool WordSearchGrid::containsWord(const std::string& word) const {
    // Check all possible directions (horizontal, vertical, diagonal)
    const int directions[][2] = {
        {0, 1}, {1, 0}, {1, 1}, {1, -1}, // right, down, down-right, down-left
        {0, -1}, {-1, 0}, {-1, -1}, {-1, 1} // left, up, up-left, up-right
    };
    
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            for (int d = 0; d < 8; d++) {
                if (checkWordAtPosition(word, row, col, directions[d][0], directions[d][1])) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

void WordSearchGrid::print(std::ostream& out) const {
    for (const auto& row : grid) {
        for (char c : row) {
            out << c;
        }
        out << std::endl;
    }
}