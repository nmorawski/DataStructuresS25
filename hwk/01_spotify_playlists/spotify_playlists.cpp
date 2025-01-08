#include <iostream> //Includes functions of program
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Includes lamda functions, sort, etc.
#include <cctype>


void parse_file(std::ifstream& input, std::string& song, std::string& band, 
    std::vector<std::string>& song_list, std::vector<std::string>& band_list){
    if (input.is_open()){//If the file is open
        while (input >> song){//While there is text to retrieve. store each line in the vector
            input >> band;
            song_list.push_back(song);
            band_list.push_back(band);
        }
        input.close();
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
    return std::all_of(num.begin(), num.end(), ::isdigit)
}

int main(int argc, char* argv[]){
    std::string song_name; //For parsing files
    std::string artist; //For parsing files
    std::vector<std::string> library_songs; //To store
    std::vector<std::string> library_artists; //To store
    std::vector<std::string> playlist_songs; //To store
    std::vector<std::string> playlist_artists; //To store 

    std::ifstream lib_input(argv[1]);
    std::ifstream playlist_input(argv[2]);
    std::ofstream output(argv[3]);

    parse_file(lib_input, song_name, artist, library_songs, library_artists);

    if (playlist_input.is_open()){//If the file is open
        while (playlist_input >> song_name){//While there is text to retrieve. store each line in the vector
            playlist_input >> artist;
            playlist_songs.push_back(song_name);
            playlist_artists.push_back(artist);
        }
        playlist_input.close();
    } else {
        std::cerr << "Unable to open playlist file" <<std::endl;
        return 1;
    }

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
                if (is_number(argv[6]) && stoi(argv[6]) <= playlist_songs.size()){
                    int playlist_loc = stoi(argv[6]);
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
}