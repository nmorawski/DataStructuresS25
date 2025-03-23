#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "comment.h"
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

// Helper function to extract integers
int extract_int(const std::string &line, const std::string &key) {
    std::string::size_type key_pos = line.find("\"" + key + "\":");
    if (key_pos == std::string::npos) return -1;
    
    unsigned int start = key_pos + key.length() + 3;
    std::string::size_type end = line.find(",", start);
    if (end == std::string::npos) return 0;
    
    std::string value = line.substr(start, end - start);
    return std::stoi(value);
}

// Helper function to extract booleans
bool extract_bool(const std::string &line, const std::string &key) {
    std::string::size_type key_pos = line.find("\"" + key + "\":");
    if (key_pos == std::string::npos) return false;
    
    unsigned int start = key_pos + key.length() + 4;
    std::string::size_type end = line.find(",", start);
    if (end == std::string::npos) return false;
    
    std::string value = line.substr(start, end - start);
    return value == "true";
};

// Extract comment text which may contain special characters
std::string extract_comment(const std::string &line) {
    std::string::size_type key_pos = line.find("\"comment\":");
    if (key_pos == std::string::npos) return "";
    
    std::string::size_type start = line.find("\"", key_pos + 10);
    if (start == std::string::npos) return "";
    start++;
    
    // Find the last quote in the string, as comment is always the last field
    std::string::size_type end = line.find_last_of("\"");
    if (end == std::string::npos || end <= start) return "";
    
    return line.substr(start, end - start);
};
//================================================================================================

// This function takes in a line from a given JSON file as a string. It creates a Comment object on
// the heap, and then stores all of the necessary values via the above helper functions.
Comment* parse_comment(const std::string& line) {
    Comment* comment = new Comment();

    comment->video_id = extract_string(line,"video_id");
    comment->author = extract_string(line,"author");
    comment->comment_id = extract_string(line,"comment_id");
    comment->like_count = extract_int(line,"like_count");
    comment->reply_count = extract_int(line,"reply_count");
    comment->is_reply = extract_bool(line,"is_reply");
    comment->parent_comment_id = extract_string(line,"parent_comment_id");
    comment->published_date = extract_string(line,"published_date");
    comment->is_video_owner = extract_bool(line,"is_video_owner");
    comment->comment_text = extract_comment(line);
    
    return comment;
}

// This function takes in the JSON input file, the vector of top-level comments, and the map of all
// comments. We loop through the JSON file and create a Comment object, which is then stored in the
// map. If the object is a top_level comment, we add it to the vector, otherwise we find its parent
// comment and store a pointer to it in the parent comment's children.
void parse_json(const std::string &input, std::map<std::string, Comment*> &comments,
    std::vector<Comment*> &video_replies) {
    
    std::ifstream input_file(input);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open JSON file." << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(input_file, line)) {
        Comment* comment = parse_comment(line);
        comments[comment->comment_id] = comment; // Store in map

        // If it's a top-level comment (direct response to video)
        if (!comment->is_reply) {
            video_replies.push_back(comment);
        }
        // If it's a reply to another comment
        else if (!comment->parent_comment_id.empty()) {
            std::map<std::string,Comment*>::iterator itr=comments.find(comment->parent_comment_id);
            if (itr != comments.end()) itr->second->addChild(comment);
        }
    }
    
    input_file.close();
}

// This function creates a new Comment object, and since it is a top_level comment, it is stored in
// both the map and the top-level comments vector.
void reply_video(const std::string& comment_id, const std::string& author,
    const std::string& comment_text, std::map<std::string, Comment*> &comments,
    std::vector<Comment*> &video_replies) {

    Comment* node = new Comment();
    node->comment_id = comment_id;
    node->author = author;
    node->comment_text = comment_text;
    node->parent_comment_id = "";
    node->published_date = "0 seconds ago";
    
    comments[comment_id] = node;
    video_replies.push_back(node);
}

// This function creates a new Comment object and stores it in the comments map. Next, we find this
// new comment's parent, and then store a pointer to this object as the parent's child.
void reply_comment(const std::string& parent_id, const std::string& comment_id, 
    const std::string& author, const std::string& comment_text,
    std::map<std::string, Comment*> &comments) {

    std::map<std::string, Comment*>::iterator parent_itr = comments.find(parent_id);
    if (parent_itr == comments.end()) {
        std::cerr << "Parent comment not found: " << parent_id << std::endl;
        return;
    }

    Comment* node = new Comment();
    node->comment_id = comment_id;
    node->author = author;
    node->comment_text = comment_text;
    node->is_reply = true;
    node->parent_comment_id = parent_id;
    node->published_date = "0 seconds ago";

    comments[comment_id] = node;
    parent_itr->second->addChild(node);
    parent_itr->second->reply_count++;
}

// This function finds a comment via id, then increases the like count by 1.
void like_comment(const std::string& comment_id, std::map<std::string, Comment*> &comments) {
    std::map<std::string, Comment*>::iterator itr = comments.find(comment_id);
    if (itr != comments.end()) {
        itr->second->like_count++;
    }
}

// This function first finds the desired comment in the comments map. If this comment is a
// sub-comment, meaning it has a parent, we first remove this comment from the parent's vector of
// children. If the comment is a top-level comment, we remove it from the vector of video replies.
// From there, we delete the given tree and remove the comment tree from the map.
void delete_comment(std::string& comment_id, std::map<std::string, Comment*> &comments,
    std::vector<Comment*> &video_replies) {

    std::map<std::string, Comment*>::iterator itr = comments.find(comment_id);
    if (itr == comments.end()) {
        return;
    }
    
    Comment* node = itr->second;
    
    // If it has a parent, remove it from parent's children
    if (node->parent) node->parent->removeChild(node);
    // If it's a top-level comment, remove from video_replies
    else {
        std::vector<Comment*>::iterator replies_itr = std::find(video_replies.begin(), 
            video_replies.end(), node);
        if (replies_itr != video_replies.end()) video_replies.erase(replies_itr);
    }


    delete node; // Deletes node and its subsequent children
    node = nullptr;

    comments.erase(comment_id);
}

// This is the required recursive function. We take in a pointer to the desired node as well as an
// integer to represent the depth level into the tree. This depth level is used to indent comments
// in the output to correctly represent the tree. For a given comment, we output the desired
// comment, then loop through its children, recursively calling the function until completion.
void recursive_display_comment(Comment* node, std::ofstream& output, int depth) {
    if (!node) return;
    
    // Create indentation (4 spaces per level)
    std::string indent(depth * 4, ' ');

    // Format the comment for display
    output << indent << node->author << " " << node->published_date << std::endl;
    output << indent << node->comment_text << std::endl;
    output << indent << "\U0001F44D " << node->like_count << std::endl;
    if (node->reply_count > 0){
        output << indent << node->reply_count;
        if (node->reply_count == 1) output << " reply" << std::endl;
        else output << " replies" << std::endl;
    }
    
    // Display all children recursively with increased indentation
    for (Comment* child : node->children) {
        recursive_display_comment(child, output, depth + 1);
    }
}

// This function first finds the desired Comment object in the tree. Next, the above recursive
// function is called to print the given comment and all of its children. If the comment is an original
// comment, then everything proceeds as normal. However, if the given comment is a new comment, we
// instead print the comment tree from the new comment's parent;
void display_comment(const std::string& comment_id, std::ofstream& output,
    std::map<std::string, Comment*> &comments) {
    
    std::map<std::string, Comment*>::iterator itr = comments.find(comment_id);
    if (itr == comments.end()) return;
    
    Comment* node = itr->second;
    
    // If displaying a top-level comment
    if (!node->is_reply) {
        // Display the comment and its descendants
        recursive_display_comment(node, output, 0);
    } else {
        if (node->published_date == "0 seconds ago") {
            recursive_display_comment(node->parent, output, 0);
        } else {
            // Just display this specific comment subtree
            recursive_display_comment(node, output, 0);
        }
    }
}

// This function's purpose is to go line-by-line in the given command input file, then parse each
// line so we know what action we need to perform for a given comment.
void process_commands(const std::string& input, const std::string& output, 
    std::map<std::string, Comment*> &comments, std::vector<Comment*> &video_replies) {
    
    std::ifstream inFile(input);
    std::ofstream outFile(output);
    
    if (!inFile.is_open()) {
        std::cerr << "Failed to open command file." << std::endl;
        exit(1);
    }
    
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        exit(1);
    }
    
    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string operation;
        iss >> operation;
        
        if (operation == "reply_to_video") {
            std::string comment_id, author, comment_text;
            iss >> comment_id >> author;
            
            iss >> std::ws; // Skip whitespace
            
            // Read the comment text (enclosed in quotes)
            if (iss.peek() == '"') {
                iss.get();
                std::getline(iss, comment_text, '"');
            }
            
            reply_video(comment_id, author, comment_text, comments, video_replies);
        }
        else if (operation == "reply_to_comment") {
            std::string parent_id, comment_id, author, comment_text;
            iss >> parent_id >> comment_id >> author;
            
            iss >> std::ws; // Skip whitespace
            
            // Read the comment text (enclosed in quotes)
            if (iss.peek() == '"') {
                iss.get();
                std::getline(iss, comment_text, '"');
            }
            
            reply_comment(parent_id, comment_id, author, comment_text, comments);
        }
        else if (operation == "like_comment") {
            std::string comment_id;
            iss >> comment_id;
            like_comment(comment_id, comments);
        }
        else if (operation == "delete_comment") {
            std::string comment_id;
            iss >> comment_id;
            delete_comment(comment_id, comments, video_replies);
        }
        else if (operation == "display_comment") {
            std::string comment_id;
            iss >> comment_id;
            display_comment(comment_id, outFile, comments);
        } else {
            std::cerr << "Invalid command." << std::endl;
            exit(1);
        }
    }
    
    inFile.close();
    outFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Incorrect number of arguments" << std::endl;
        return 1;
    }
    
    std::string jsonFile = argv[1];
    std::string commands = argv[2];
    std::string output = argv[3];
    
    std::map<std::string, Comment*> comments; // Store all comments by ID for quick lookup
    std::vector<Comment*> top_level_comments; // Direct responses to video, no parent
    
    // Load comments from JSON file
    parse_json(jsonFile, comments, top_level_comments);

    // Process commands and generate output
    process_commands(commands, output, comments, top_level_comments);

    // Cleanup remaining comments
    for (unsigned int i = 0; i < top_level_comments.size(); i++) delete top_level_comments[i];
    top_level_comments.clear();

    return 0;
}