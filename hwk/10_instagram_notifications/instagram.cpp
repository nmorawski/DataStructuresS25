#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cctype>

#include "notification.h"
#include "preferences.h"

//============================ HELPER FUNCTIONS TO AID JSON PARSING ==============================
// Helper function to extract strings
std::string extract_string(const std::string &line, const std::string &key) {
    std::string::size_type key_pos = line.find("\"" + key + "\":");
    if (key_pos == std::string::npos) return "";
    
    std::string::size_type start = line.find("\"", key_pos + key.length() + 2);
    if (start == std::string::npos) return "";
    start++;
    
    std::string::size_type end = line.find("\"", start);
    if (end == std::string::npos) return "";
    
    return line.substr(start, end - start);
}

bool extract_bool(const std::string &line, const std::string &key) {
    std::string value = extract_string(line, key);
    return value == "true";
};
//================================================================================================

void extract_preferences(const std::string& json, Preferences& prefs) { 
    // Parse each preference
    prefs.pauseAll = extract_bool(json, "pauseAll");
    prefs.likes = extract_bool(json, "likes");
    prefs.tags = extract_bool(json, "tags");
    prefs.comments = extract_bool(json, "comments");
    prefs.newFollowers = extract_bool(json, "newFollowers");
    prefs.messageRequests = extract_bool(json, "messageRequests");
}

Preferences parse_users(const std::string& file, const std::string& user) {
    std::ifstream jsonFile(file);
    
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open the JSON file: " << file << std::endl;
        exit(1);
    }
    
    std::string line;
    while (std::getline(jsonFile, line)) {
        std::string username = extract_string(line,"username");
        if (user == username) {
            Preferences prefs;
            extract_preferences(line, prefs);
            return prefs;
        }
    }
    jsonFile.close();
    std::cerr << "Failed to find user." << std::endl;
    exit(1);
}

void parse_posts(const std::string& file, const std::string& user, std::unordered_set<std::string> &posts) {
    std::ifstream jsonFile(file);
   
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open the JSON file: " << file << std::endl;
        exit(1);
    }
   
    std::string line;
    while (std::getline(jsonFile, line)) {
        std::string ownerUsername = extract_string(line,"ownerUsername");
        if (ownerUsername == user) {
            std::string id = extract_string(line,"id");
            posts.emplace(id);
        }
    }
    jsonFile.close();
}

void parse_events(const std::string& file, const std::string& user,
    std::vector<Notification*> &notifs, const std::unordered_set<std::string> &posts,
    const Preferences &prefs) {
    std::ifstream events(file);
    if (!events.is_open()) {
        std::cerr << "Failed to open event file." << std::endl;
        exit(1);
    }
    
    std::string current_event = "";
    unsigned int current_count = 0;
    
    std::string sender, event, recipient;
    while(events >> sender >> event >> recipient) {
        std::unordered_set<std::string>::const_iterator itr = posts.find(recipient);
        int vec_size = notifs.size();
            
        if (recipient == user || itr != posts.end()) {
            Notification* notif = Notification::createNotification(event, sender, prefs);
            
            if (notif) {
                if (event != current_event) {
                    if (current_count > 3) {
                        // Create aggregated notification for previous event type
                        Notification* agg_notif = Notification::createAggNotification(
                            notifs[vec_size-1], 
                            notifs[vec_size-2], 
                            current_count);
                            
                        // Remove the individual notifications and delete them to prevent memory leaks
                        for (int i = vec_size - current_count; i < vec_size; i++) {
                            delete notifs[i];
                        }
                        std::vector<Notification*> temp_vec(notifs.begin(), notifs.end() - current_count);
                        notifs.swap(temp_vec);
                        
                        // Add the aggregated notification
                        if (agg_notif)
                            notifs.push_back(agg_notif);
                    }
                    
                    // Reset counter for new event type
                    current_count = 1;
                    current_event = event;
                } else {
                    // Same event type, increment counter
                    current_count++;
                }
                
                // Add the new notification
                notifs.push_back(notif);
            }
        }
    }
    
    // Don't forget to check for aggregation at the end for the last event type
    if (current_count > 3) {
        Notification* agg_notif = Notification::createAggNotification(
            notifs[notifs.size()-1], 
            notifs[notifs.size()-2], 
            current_count);
            
        // Delete notifications that will be replaced by the aggregated one
        for (size_t i = notifs.size() - current_count; i < notifs.size(); i++) {
            delete notifs[i];
        }
        std::vector<Notification*> temp_vec(notifs.begin(), notifs.end() - current_count);
        notifs.swap(temp_vec);
        
        if (agg_notif)
            notifs.push_back(agg_notif);
    }
    
    events.close();
}

void print_notifs(const std::string& file, const std::vector<Notification*> &notifs) {
    std::ofstream output(file);

    if (!output.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        exit(1);
    }

    for (std::vector<Notification*>::const_reverse_iterator itr = notifs.crbegin(); itr != notifs.crend(); ++itr) { // Iterate backwards
        if ((*itr)->getAggCheck())
            output << (*itr)->getAggMessage() << std::endl;
        else
            output << (*itr)->getMessage() << std::endl;
    }
    //output.close();
}

void cleanup(std::vector<Notification*> &notifs) {
    // Delete all Notification objects
    for (Notification* notif : notifs)
        delete notif;
    notifs.clear();
}

// nynotifications.exe posts.json users.json events_medium.txt output.txt taylorswift
int main(int argc, char* argv[]) {
    if (argc != 6) {
        std::cerr << "Invalid number of arguments." << std::endl;
        return 1;
    }
    
    std::string postsJson = argv[1];
    std::string usersJson = argv[2];
    std::string events = argv[3];
    std::string output = argv[4];
    std::string user = argv[5];

    std::unordered_set<std::string> posts;
    std::vector<Notification*> notifs;

    Preferences settings = parse_users(usersJson, user);
    parse_posts(postsJson, user, posts);

    parse_events(events, user, notifs, posts, settings);
    if (notifs.size() > 100) {
        //temp, fix later
        for (int i = 0; i < notifs.size() - 100; i++)
            delete notifs[i];

        std::vector<Notification*> temp_vec(notifs.end() - 100, notifs.end());
        notifs.swap(temp_vec);
    }

    print_notifs(output, notifs);

    cleanup(notifs);

    return 0;
}
