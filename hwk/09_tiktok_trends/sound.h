#ifndef __SOUND_H
#define __SOUND_H

#include <string>
#include <vector>
#include "video.h"

class Sound {
public:
    std::string musicId;
    std::string musicName;
    std::string musicAuthor;
    unsigned long totalViews;
    std::unordered_set<const Video*> videos;
    std::unordered_set<const Video*> repeats;

    bool operator<(const Sound& other) const;
};

// Custom comparator for sound priority queue
bool Sound::operator<(const Sound& other) const {
    if (totalViews != other.totalViews) {
        return totalViews > other.totalViews;
    }
    return musicId < other.musicId;
}

#endif