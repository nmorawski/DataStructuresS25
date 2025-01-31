#ifndef __RECOMMENDATION_H
#define __RECOMMENDATION_H

#include <string>

class RecommendationSystem {
public:
RecommendationSystem(); //Default constructor 
RecommendationSystem(int** m1, int** m2); //Used for rider location
~RecommendationSystem();
// ACCESSORS
int** getMovieRatings() const { return userMovieRatingsMatrix; }
int** getShowRatings() const { return userShowRatingsMatrix; }
// MODIFIERS
void recommendMovies(int userId, int numRecommendations, int* recommendedMovies) const;
void recommendShows(int userId, int numRecommendations, int* recommendedShows) const;

private: // REPRESENTATION (member variables)
int** userMovieRatingsMatrix;
int** userShowRatingsMatrix;

};

#endif