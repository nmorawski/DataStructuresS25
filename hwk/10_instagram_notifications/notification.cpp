#include "notification.h"

// Base class implementation
Notification::Notification(const std::string& sender) : sender(sender), aggCheck(false) {}

Notification::Notification(const std::string& s1, const std::string& s2, int ct) : sender(s1), 
    otherSender(s2), count(ct), aggCheck(true) {}

Notification::~Notification() = default;

Notification* Notification::createNotification(const std::string& eventType,
    const std::string& sender, const Preferences &user) {
    if (!(user.pauseAll)) {
        if (eventType == "likes" && user.likes)
            return new Like(sender);
        else if (eventType == "tags" && user.tags)
            return new Tag(sender);
        else if (eventType == "comments_on" && user.comments)
            return new Comment(sender);
        else if (eventType == "follows" && user.newFollowers)
            return new Follow(sender);
        else if (eventType == "messageRequests" && user.messageRequests)
            return new MessageRequest(sender);
        else
            return nullptr;
    }
    return nullptr;
}

Notification* Notification::createAggNotification(const Notification* recent, 
    const Notification* next, int count) {
    // Determine which type of notification to create
    if (dynamic_cast<const Like*>(recent))
        return new Like(recent->getSender(), next->getSender(), count-2);
    else if (dynamic_cast<const Tag*>(recent))
        return new Tag(recent->getSender(), next->getSender(), count-2);
    else if (dynamic_cast<const Comment*>(recent))
        return new Comment(recent->getSender(), next->getSender(), count-2);
    else if (dynamic_cast<const Follow*>(recent))
        return new Follow(recent->getSender(), next->getSender(), count-2);
    else if (dynamic_cast<const MessageRequest*>(recent))
        return new MessageRequest(recent->getSender(), next->getSender(), count-2);
    
    return nullptr;
}

// Like implementation
Like::Like(const std::string& sender)
    : Notification(sender) {}

Like::Like(const std::string& s1, const std::string& s2, int ct)
    : Notification(s1, s2, ct) {}

std::string Like::getMessage() const {
    return sender + " liked your post.";
}

std::string Like::getAggMessage() const {
    return sender + ", " + otherSender +" and "+ std::to_string(count) +" others liked your post.";
}

// Tag implementation
Tag::Tag(const std::string& sender)
    : Notification(sender) {}

Tag::Tag(const std::string& s1, const std::string& s2, int ct)
    : Notification(s1, s2, ct) {}

std::string Tag::getMessage() const {
    return sender + " tagged you in a post.";
}

std::string Tag::getAggMessage() const {
    return sender + ", " + otherSender +" and "+ std::to_string(count) +" others tagged you in a post.";
}

// Comment implementation
Comment::Comment(const std::string& sender)
    : Notification(sender) {}

Comment::Comment(const std::string& s1, const std::string& s2, int ct)
    : Notification(s1, s2, ct) {}

std::string Comment::getMessage() const {
    return sender + " commented on your post.";
}

std::string Comment::getAggMessage() const {
    return sender + ", " + otherSender +" and "+ std::to_string(count) +" others commented on your post.";
}

// Follow implementation
Follow::Follow(const std::string& sender)
    : Notification(sender) {}

Follow::Follow(const std::string& s1, const std::string& s2, int ct)
    : Notification(s1, s2, ct) {}

std::string Follow::getMessage() const {
    return sender + " started following you.";
}

std::string Follow::getAggMessage() const {
    return sender + ", " + otherSender +" and "+ std::to_string(count) +" others started following you.";
}

// MessageRequest implementation
MessageRequest::MessageRequest(const std::string& sender)
    : Notification(sender) {}

MessageRequest::MessageRequest(const std::string& s1, const std::string& s2, int ct)
    : Notification(s1, s2, ct) {}

std::string MessageRequest::getMessage() const {
    return sender + " wants to send you a message.";
}

std::string MessageRequest::getAggMessage() const {
    return sender + ", " + otherSender +" and "+ std::to_string(count) +" others wants to send you a message.";
}