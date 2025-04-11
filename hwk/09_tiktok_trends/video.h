#ifndef __VIDEO_H
#define __VIDEO_H

#include <string>

class Video {
public:
    std::string id;
    std::string coverUrl;
    std::string webVideoUrl;
    unsigned long playCount;
    unsigned long fileIndex;
};

bool compareVideos(const Video& v1, const Video& v2) {
    if (v1.playCount != v2.playCount)
        return v1.playCount > v2.playCount;
    else if (v1.id != v2.id)
        return v1.id < v2.id; // If play counts are equal, sort by ID
    return v1.fileIndex < v2.fileIndex; // otherwise, location in input file
}

bool compareVideoPtrs(const Video* v1, const Video* v2) {
    return compareVideos(*v1, *v2);
}

#endif