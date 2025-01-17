#ifndef __ANIMAL_H
#define __ANIMAL_H

#include <iostream>
#include <vector>
#include <string>

class Animal {
public:
Animal(const std::string name); //Constructor
// ACCESSORS
std::string getName() const { return name_; }
float getWeight() const { return weight_; }
bool eatMeat() const { return meat_; }
bool eatPlants() const { return plants_; }
bool surviveLand() const { return land_; }
bool surviveWater() const { return water_; }
bool isOmnivore() const { return meat_ && plants_; } //Returns true if both are true
bool isAmphibious() const { return land_ && water_; } //Returns true if both are true
// MODIFIERS
void setName(std::string name);
void setWeight(float weight);
void setEatMeat(bool meat);
void setEatPlants(bool plants);
void setLiveLand(bool land);
void setLiveWater(bool water);

private: // REPRESENTATION (member variables)
std::string name_;
float weight_;
bool meat_;
bool plants_;
bool land_;
bool water_;
bool isOmnivore_;

};

#endif