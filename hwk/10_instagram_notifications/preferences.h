#ifndef USER_H
#define USER_H

#include <string>

// Used to save a given user's notification preferences
class Preferences {
public:
    bool pauseAll;
    bool likes;
    bool tags;
    bool comments;
    bool newFollowers;
    bool messageRequests;
};

#endif