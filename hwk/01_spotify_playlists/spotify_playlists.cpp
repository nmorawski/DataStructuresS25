#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Used for std::stoi
#include <sstream> //Includes stringstream
#include <cctype> //Used for isdigit()

/*The function parse_line takes in a string, which is a line in either
  the library or the playlist input file. It then uses the double 
  quotation character, '"', as a delimiter. The function then returns a
  vector of size 2, which includes the song title and the artist(s) credited.*/
std::vector<std::string> parse_line(const std::string &line){
    std::vector<std::string> results;
    std::stringstream ss(line);
    std::string name;

    while (getline(ss, name, '"')) {
        if (name.size() > 0) //Don't include empty characters
            results.push_back(name);
    }

    return results;
}

/*The function parse_file takes in an input file, along with two vectors of strings
  that represent all of the song titles from the input file, along with all of the 
  credited artists. The function goes line by line in the input file, and while there
  is text to retrieve, parse_line is called, and the values produced are saved in their
  respective vector of strings. This function is used interchangeablywith the playlist
  and library input files.*/
int parse_file(std::ifstream& input, std::vector<std::string>& song_list, 
        std::vector<std::string>& band_list){
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

/*The function find_song takes in a string, the desired song title, and the vector
  of strings that represents the desired vector of song titles (either the library
  vector, or the playlist vector). The function loops through the vector, searching
  for the desired song title. If found, the function will return the song title's index.
  If not found, the function will return -1 to represent the failure to do so.*/
int find_song(const std::string title, std::vector<std::string> &songs){
    unsigned int song_index = 0;
    std::string lib_title = "";
    while (title != lib_title && song_index < songs.size()){
        lib_title = songs[song_index];
        song_index++;
    }
    if (title == lib_title)//Match found
        return song_index - 1;
    return -1;
}

/*The function is_number is used solely for when the 'move' action is called. The 
  function takes in a string, which is supposed to be the number at the end of the 
  command line, and verifies the string is a legitimate number. This is done by
  verifying each character in the string is numeric.*/
bool is_number(const std::string num){
    return std::all_of(num.begin(), num.end(), ::isdigit);
}

int main(int argc, char* argv[]){
    std::string song_name;
    std::string artist;

    //Chose to have two vectors of string to represent an input file instead
    //of a 2-D vector (std::vector< std::vector<std::string>>) to reduce complexity.
    std::vector<std::string> library_songs; //Song titles from library input
    std::vector<std::string> library_artists; //Artists from library input
    std::vector<std::string> playlist_songs; //Song titles from playlist input
    std::vector<std::string> playlist_artists; //Artists from playlist input

    std::ifstream playlist_input(argv[1]);
    std::ifstream lib_input(argv[2]);
    std::ofstream output(argv[3]);

    parse_file(lib_input, library_songs, library_artists);//Parsed Library
    parse_file(playlist_input, playlist_songs, playlist_artists);//Parsed playlist

    if (argv[4] == std::string("add")){//Add command
        if (argc != 6){
            std::cerr << "Incorrect amount of command line arguments." << std::endl;
            return 1;
        } else {
           //Search for song title in library. If found, add the respective title and
           //artists to the end of the playlist (the two playlist vectors).
            song_name = argv[5];
            int song_loc = find_song(song_name,library_songs);
            if (song_loc != -1){//If song found
                playlist_songs.push_back(library_songs[song_loc]);
                playlist_artists.push_back(library_artists[song_loc]);
            } else {
                std::cerr << "Requested song could not be found." << std::endl;
                return 1;
            }
        }
    } else if (argv[4] == std::string("remove")){//Remove command
        if (argc != 6){
            std::cerr << "Incorrect amount of command line arguments." << std::endl;
            return 1;
        } else {
           //Search for song title in playlist. If found, remove the respective title and
           //artists from their current location in the playlist (the two playlist vectors).
            song_name = argv[5];
            int song_loc = find_song(song_name,playlist_songs);
            if (song_loc != -1){//If song found
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
           /*Search for song title in playlist. If found, remove the respective title and
             artists from their current location in the playlist (the two playlist vectors)
             and re-insert back in the playlist in the desired position (desired placement
             minus 1 to determine vector index).*/
            song_name = argv[5];
            int song_loc = find_song(song_name,playlist_songs);
            if (song_loc != -1){//If song found
                if (is_number(argv[6]) && std::stoi(argv[6]) <= playlist_songs.size()){
                    int playlist_loc = std::stoi(argv[6]);
                    std::string selected_artist = playlist_artists[song_loc];
                    
                    playlist_songs.erase(playlist_songs.begin() + song_loc);
                    playlist_songs.insert(playlist_songs.begin() + (playlist_loc - 1), song_name);

                    playlist_artists.erase(playlist_artists.begin() + song_loc);
                    playlist_artists.insert(playlist_artists.begin() + (playlist_loc - 1), 
                        selected_artist);
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
    /*Open the desired output file and loop through the playlist song vector. Both the
      song and artist vectors are the same size, and each index matches, so we only need
      one for loop. Print one song-artist combination per line until completion.*/
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