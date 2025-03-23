#include "comment.h"

Comment::Comment() { // Default constructor
    like_count = 0;
    reply_count = 0;
    is_reply = false;
    is_video_owner = false;
    parent = nullptr;
}

Comment::~Comment() {
    for (Comment* child : children) {
        delete child;
    }
}

// Add a child comment to this comment
void Comment::addChild(Comment* child) {
    children.push_back(child);
    child->parent = this;
}

// Remove a child comment from this comment
void Comment::removeChild(Comment* child) {
    std::vector<Comment*>::iterator it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        reply_count--;
    }
}