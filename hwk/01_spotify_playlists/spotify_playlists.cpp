#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Used for std::stoi
#include <sstream> //includes stringstream
#include <cctype> //Used for isdigit()


std::vector<std::string> parse_line(const std::string &line){
    std::vector<std::string> results;
    std::stringstream ss(line);
    std::string name;

    while (getline(ss, name, '"')) {
        if (name.size() > 0)
            results.push_back(name);
    }

    return results;
}

int parse_file(std::ifstream& input, std::vector<std::string>& song_list, std::vector<std::string>& band_list){
    if (input.is_open()){//If the file is open
        std::string row;
        std::vector<std::string> line;
        while (input.good()){//While there is text to retrieve. store each line in the vector
            getline(input, row);
            if (row.size() > 0) {
                line = parse_line(row);
                song_list.push_back(line[0]);
                band_list.push_back(line[1].substr(1, line[1].size()));
            }
        }
        input.close();
        return 0;
    } else {
        std::cerr << "Unable to open file" <<std::endl;
        return 1;
    }
}

int find_song(const std::string title, std::vector<std::string> &songs){
    unsigned int song_index = 0;
    std::string lib_title = "";
    while (title != lib_title && song_index < songs.size()){
        lib_title = songs[song_index];
        song_index++;
    }
    if (title == lib_title)
        return song_index - 1;
    return -1;
}

bool is_number(const std::string num){
    return std::all_of(num.begin(), num.end(), ::isdigit);
}

int main(int argc, char* argv[]){
    std::string song_name; //For parsing files
    std::string artist; //For parsing files
    std::vector<std::string> library_songs; //To store
    std::vector<std::string> library_artists; //To store
    std::vector<std::string> playlist_songs; //To store
    std::vector<std::string> playlist_artists; //To store 

    std::ifstream playlist_input(argv[1]);
    std::ifstream lib_input(argv[2]);
    std::ofstream output(argv[3]);

    parse_file(lib_input, library_songs, library_artists);

    parse_file(playlist_input, playlist_songs, playlist_artists);

    if (argv[4] == std::string("add")){
        if (argc != 6){
            std::cerr << "Incorrect amount of command line arguments." << std::endl;
            return 1;
        } else {
           //c
            song_name = argv[5];
            int song_loc = find_song(song_name,library_songs);
            if (song_loc != -1){
                playlist_songs.push_back(library_songs[song_loc]);
                playlist_artists.push_back(library_artists[song_loc]);
            } else {
                std::cerr << "Requested song could not be found." << std::endl;
                return 1;
            }
        }
    } else if (argv[4] == std::string("remove")){
        if (argc != 6){
            std::cerr << "Incorrect amount of command line arguments." << std::endl;
            return 1;
        } else {
           //c
            song_name = argv[5];
            int song_loc = find_song(song_name,playlist_songs);
            if (song_loc != -1){
                playlist_songs.erase(playlist_songs.begin() + song_loc);
                playlist_artists.erase(playlist_artists.begin() + song_loc);
            } else {
                std::cerr << "Requested song could not be found." << std::endl;
                return 1;
            }
        }
    } else if (argv[4] == std::string("move")){
        if (argc != 7){
            std::cerr << "Incorrect amount of command line arguments." << std::endl;
            return 1;
        } else {
           //c
            song_name = argv[5];
            int song_loc = find_song(song_name,playlist_songs);
            if (song_loc != -1){
                if (is_number(argv[6]) && std::stoi(argv[6]) <= playlist_songs.size()){
                    int playlist_loc = std::stoi(argv[6]);
                    std::string selected_artist = playlist_artists[song_loc];
                    
                    playlist_songs.erase(playlist_songs.begin() + song_loc);
                    playlist_songs.insert(playlist_songs.begin() + (playlist_loc - 1), song_name);

                    playlist_artists.erase(playlist_artists.begin() + song_loc);
                    playlist_artists.insert(playlist_artists.begin() + (playlist_loc - 1), selected_artist);
                } else {
                   std::cerr << "Move request could not be made." << std::endl;
                    return 1; 
                }
            } else {
                std::cerr << "Requested song could not be found." << std::endl;
                return 1;
            }
        }
    } else {
        std::cerr << "Invalid command given." << std::endl;
        return 1;
    }

    if (output.is_open()){//If the file is open
        for (unsigned int i = 0; i < playlist_songs.size(); i++)
            output << '"' << playlist_songs[i] << "\" " << playlist_artists[i] << "\n";
        output.close();
    } else {
        std::cerr << "Unable to open file" <<std::endl;
        return 1;
    }

    return 0;
}