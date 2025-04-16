#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "preferences.h"

class Notification {
protected:
    std::string sender;
    std::string otherSender;
    int count;
    bool aggCheck;

public:
    // Constructor
    Notification(const std::string& sender);
    Notification(const std::string& s1, const std::string& s2, int ct);
    
    virtual ~Notification();
    
    // get notification message
    virtual std::string getMessage() const = 0;
    virtual std::string getAggMessage() const = 0;
    
    // Getter methods
    std::string getSender() const { return sender; }
    bool getAggCheck() const { return aggCheck; }
    
    // create a notification based on event type
    static Notification* createNotification(const std::string& eventType, 
        const std::string& sender, const Preferences &user);

    static Notification* createAggNotification(const Notification* recent, 
        const Notification* next, int count);
};

// Like class for when someone likes a post
class Like : public Notification {
public:
    Like(const std::string& sender);
    Like(const std::string& s1, const std::string& s2, int ct);
    
    std::string getMessage() const override;
    std::string getAggMessage() const override;
};

// Tag class for when someone tags a user
class Tag : public Notification {
public:
    Tag(const std::string& sender);
    Tag(const std::string& s1, const std::string& s2, int ct);
    
    std::string getMessage() const override;
    std::string getAggMessage() const override;
};

// Comment class for when someone comments on a post
class Comment : public Notification {
public:
    Comment(const std::string& sender);
    Comment(const std::string& s1, const std::string& s2, int ct);
    
    std::string getMessage() const override;
    std::string getAggMessage() const override;
};

// Follow class for when someone follows a user
class Follow : public Notification {
public:
    Follow(const std::string& sender);
    Follow(const std::string& s1, const std::string& s2, int ct);
    
    std::string getMessage() const override;
    std::string getAggMessage() const override;
};

// MessageRequest class for when someone sends a message request
class MessageRequest : public Notification {
public:
    MessageRequest(const std::string& sender);
    MessageRequest(const std::string& s1, const std::string& s2, int ct);
    
    std::string getMessage() const override;
    std::string getAggMessage() const override;
};

#endif 