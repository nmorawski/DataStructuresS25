#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <regex>
#include <algorithm>
#include <cctype>

#include "video.h"
#include "hashtag.h"
#include "sound.h"

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
unsigned long extract_ulong(const std::string &line, const std::string &key) {
    std::string::size_type key_pos = line.find("\"" + key + "\":");
    if (key_pos == std::string::npos) return -1;
    
    unsigned int start = key_pos + key.length() + 3;
    std::string::size_type end = line.find(",", start);
    if (end == std::string::npos) return 0;
    
    std::string value = line.substr(start, end - start);
    return std::stoul(value);
}
//================================================================================================

// Extract hashtags from text
std::vector<std::string> extractHashtags(const std::string& text) {
    std::vector<std::string> hashtags;
    const unsigned int len = text.length();
    unsigned int i = 0;

    while (i < len) {
        if (text[i] == '#') {
            unsigned int start = i + 1;
            unsigned int end = start;

            // Accept alphanumeric, underscore, and extended UTF-8 characters (googled)
            while (end < len && (std::isalnum(static_cast<unsigned char>(text[end])) 
                || text[end] == '_' || static_cast<unsigned char>(text[end]) >= 0x80)) {
                ++end;
            }

            if (end > start) {
                hashtags.emplace_back(text.substr(start, end - start));
                i = end;
                continue;
            }
        }
        i++;
    }
    return hashtags;
}

// Takes in a line from the JSOn input file and creates the desired Video object
Video* parse_video(const std::string& line) {
    std::string text = extract_string(line,"text");
    std::string videoId = extract_string(line,"id");
    std::string coverUrl = extract_string(line,"coverUrl");
    std::string webVideoUrl = extract_string(line,"webVideoUrl");
    unsigned long playCount = extract_ulong(line,"playCount");
            
    // Create Video object
    return new Video{videoId, coverUrl, webVideoUrl, playCount};
}

// Helper function to print the top 3 videos for a given hashtag or sound
void printVideos(std::ofstream &output, const std::vector<const Video*> &videos, 
    const std::unordered_set<const Video*> &repeats) {
    // Output up to 3 videos
    int videosToShow = std::min(3, static_cast<int>(videos.size()));
    int check = 0;
    for (int j = 0; j < videosToShow; ++j) {
        if (check < videosToShow){
            const Video* video = videos[j];
            output << "cover url: " << video->coverUrl << std::endl;
            output << "web video url: " << video->webVideoUrl << std::endl;
            if (repeats.find(video) != repeats.end() && j < 2) {
                output << "cover url: " << video->coverUrl << std::endl;
                output << "web video url: " << video->webVideoUrl << std::endl;
                check++;
            }
            check++;
        }
    }
}

// Function takes in the created unordered_map of hashtag/sound class objects, and stores each
// element in a priority queue. To improve memory and speed, if the priority queue is larger than 
// 20 elements, we pop the first element, which is the lowest rated of the desired class objects. 
// We then store the top 20 objects in a vector to facilitate printing the output.
template <typename T>
std::vector<T> pq_convert(const std::unordered_map<std::string, T> &trends) {
    std::priority_queue<T> trendPQ;
    for (const std::pair<std::string, T> &pair : trends) {
        if (trendPQ.size() < 20) {
            trendPQ.push(pair.second);
        } else if (trendPQ.top() < pair.second){
            trendPQ.push(pair.second);
            trendPQ.pop();
        }
    }

    // Convert priority queue to vector for easier processing
    std::vector<T> topTrends;
    while (!trendPQ.empty()) {
        topTrends.push_back(trendPQ.top());
        trendPQ.pop();
    }
    return topTrends;
}

// The function parses each line of the JSON file, creating Video objects for each line. For each
// video, we then parse all of the listed hashtags and store the info in a map. This map is then
// converted into a priority queue, then converts the top 20 into a vector, which is then printed
void processHashtags(const std::string &input, const std::string& outputFile) {
    std::ifstream input_file(input);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open JSON file." << std::endl;
        exit(1);
    }
    std::unordered_map<std::string, Hashtag> hashtagMap;
    std::string line;
    unsigned long fileValue = 1;
    // Process each line (post) in the JSON file
    while (std::getline(input_file, line)) {
        // Create Video object
        Video* video = parse_video(line);
        video->fileIndex = fileValue++;
        std::string text = extract_string(line,"text");

        // Extract and process hashtags
        std::vector<std::string> hashtags = extractHashtags(text);
        for (const std::string& tag : hashtags) {
            // Update hashtag info
            Hashtag& tagRef = hashtagMap[tag];
            tagRef.name = tag;
            tagRef.usageCount++;
            tagRef.totalViews += video->playCount;
            if (tagRef.videos.find(video) != tagRef.videos.end())
                tagRef.repeats.emplace(video);
            else
                tagRef.videos.emplace(video);
        }
    }
    input_file.close();
    
    std::vector<Hashtag> topHashtags = pq_convert(hashtagMap);
    
    // Output to file
    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        return;
    }
    
    outFile << "trending hashtags:\n\n";
    for (int i = static_cast<int>(topHashtags.size()-1); i >= 0; --i) { // Iterate backwards
        outFile << "========================"<<std::endl;
        Hashtag& hashtag = topHashtags[i];
        std::vector<const Video*> sortedVideos(hashtag.videos.begin(), hashtag.videos.end());
        std::sort(sortedVideos.begin(), sortedVideos.end(), compareVideoPtrs);
        outFile <<'#'<< hashtag.name << std::endl << "used "<< hashtag.usageCount 
            <<" times" << std::endl;
        outFile << hashtag.totalViews << " views\n" <<std::endl;
        printVideos(outFile, sortedVideos, hashtag.repeats);
        outFile << "========================"<<std::endl;
    }
    
    outFile.close();
}

// The function parses each line of the JSON file, creating Video objects for each line. For each
// video, we then parse all of the given sounds and store the info in a map. This map is then
// converted into a priority queue, then converts the top 20 into a vector, which is then printed
void processSounds(const std::string &input, const std::string& outputFile) {
    std::ifstream input_file(input);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open JSON file." << std::endl;
        exit(1);
    }
    
    std::unordered_map<std::string, Sound> soundMap;
    std::string line;
    unsigned long fileValue = 1;

    // Process each line (post) in the JSON file
    while (std::getline(input_file, line)) {
        Video* video = parse_video(line);
        video->fileIndex = fileValue++;
        
        std::string musicId = extract_string(line,"musicId");
        std::string musicName = extract_string(line,"musicName");
        std::string musicAuthor = extract_string(line,"musicAuthor");
        
        // Update sound info
        Sound& tagRef = soundMap[musicId];
        tagRef.musicId = musicId;
        tagRef.musicName = musicName;
        tagRef.musicAuthor = musicAuthor;
        tagRef.totalViews += video->playCount;
        tagRef.videos.emplace(video);
    }
    input_file.close();
    
    std::vector<Sound> topSounds = pq_convert(soundMap);
    
    // Output to file
    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        return;
    }
    
    outFile << "trending sounds:\n\n";
    for (int i = static_cast<int>(topSounds.size()-1); i >= 0; --i) { // Iterate backwards
        outFile << "========================"<<std::endl;
        Sound& sound = topSounds[i];
        std::vector<const Video*> sortedVideos(sound.videos.begin(), sound.videos.end());
        std::sort(sortedVideos.begin(), sortedVideos.end(), compareVideoPtrs);
        outFile << sound.musicName << std::endl << sound.totalViews << " views" << std::endl;
        outFile << sound.musicAuthor << std::endl << "music id: "<<sound.musicId 
            << std::endl << std::endl;
        printVideos(outFile, sortedVideos, sound.repeats);
        outFile << "========================"<<std::endl;
    }
    
    outFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "invalid number of arguments." << std::endl;
        return 1;
    }
    
    std::string jsonFile = argv[1];
    std::string outputFile = argv[2];
    std::string command = argv[3];
    
    if (command == "hashtag") {
        processHashtags(jsonFile, outputFile);
    } else if (command == "sound") {
        processSounds(jsonFile, outputFile);
    } else {
        std::cerr << "Invalid command." << std::endl;
        return 1;
    }
    
    return 0;
}