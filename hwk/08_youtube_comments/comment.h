#ifndef __COMMENT_H
#define __COMMENT_H

#include <string>
#include <vector>
#include <algorithm>

class Comment {
public:
    Comment(); // Default constructor
    ~Comment(); // Destructor

    void addChild(Comment* child);
    void removeChild(Comment* child);

    std::string video_id;
    std::string author;
    std::string comment_id;
    int like_count;
    int reply_count;
    bool is_reply;
    std::string parent_comment_id;
    std::string published_date;
    bool is_video_owner;
    std::string comment_text; 

    Comment* parent;
    std::vector<Comment*> children; // Replies to this comment
};

#endif