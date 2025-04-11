#ifndef __HASHTAG_H
#define __HASHTAG_H

#include <string>
#include <vector>
#include "video.h"

class Hashtag {
public:
    std::string name;
    int usageCount;
    unsigned long totalViews;
    std::unordered_set<const Video*> videos;
    std::unordered_set<const Video*> repeats;

    bool operator<(const Hashtag& other) const;
};

// Custom comparator for hashtag priority queue
bool Hashtag::operator<(const Hashtag& other) const {
    if (usageCount != other.usageCount) {
        return usageCount > other.usageCount;
    }
    if (totalViews != other.totalViews) {
        return totalViews > other.totalViews;
    }
    return name < other.name;
}

#endif