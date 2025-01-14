#include <iostream> 
#include <fstream> //Include file streams
#include <string> //Includes strings
#include <vector> //Includes vectors
#include <algorithm> //Used for std::stoi
#include <sstream> //Includes stringstream
#include <cctype> //Used for isdigit()

/*The function parse_line takes in a string, which is a line in either
  the playlist or the actions input file. If the first character is a double
  quotation mark ('"'), then we know we are in the playlist file. If so, use that
  character as delimiter, otherwise use the space character as a delimiter. For
  the playlist file, a vector will be returned containing the song title, artist. and
  indication of currently playing, if applicable. Otherwise, it will be a vector of all
  words/characters separated by spaces, to be parsed later in the parse_command function.*/
std::vector<std::string> parse_line(std::string &line){
    std::vector<std::string> results;
    std::stringstream ss(line);
    std::string name;
    std::string band;
    std::string current_song_check = " current";

    if (line[0] == '"'){//Determines if playlist or actions parsing
        while (getline(ss, name, '"')) {
            if (name.size() > 0) //Don't include empty characters
                results.push_back(name);
        }
        band = results[1];
        //Way to check if currently playing song or not
        if (band.size() > current_song_check.size()){
            //Check to see if last part of string says current
            if (band.substr(band.length() - current_song_check.size()) == current_song_check){
                results[1] = band.substr(0, band.length() - current_song_check.size());
                results.push_back(current_song_check);
            }
        }
    } else {
        while (getline(ss, name, ' ')) {
            if (name.size() > 0)
                results.push_back(name);
        }
    }
    return results;
}

/*The function parse_playlist takes in the playlist file as an input, along with two 
  vectors of strings that represent all of the song titles from the input file, along 
  with all of the credited artists. The function goes line by line in the input file, 
  and while there is text to retrieve, parse_line is called, and the values produced 
  are saved in their respective vector of strings. The location of the currently playing
  song is also passed to store while parsing the playlist.*/
int parse_playlist(std::ifstream &input, std::vector<std::string> &songs, std::vector<std::string>
    &artists, int &current){
    if (input.is_open()){//If the file is open
        std::string row;
        std::vector<std::string> line;
        int playlist_index = 0;
        while (input.good()){//While there is text to retrieve. store each line in the vector
            getline(input, row);
            if (row.size() > 0) {
                line = parse_line(row);
                songs.push_back(line[0]);
                artists.push_back(line[1].substr(1, line[1].size()));
                if (line.size() == 3) //Currently playing song
                    current = playlist_index;
                playlist_index++;
            }
        }
        input.close();
        return 0;
    } else {
        std::cerr << "Unable to open file" <<std::endl;
        return 1;
    }
}

/*The function is_number is used solely for when the 'move' action is called. The 
  function takes in a string, which is supposed to be the number at the end of the 
  command line, and verifies the string is a legitimate number. This is done by
  verifying each character in the string is numeric.*/
bool is_number(std::string &num){
    return std::all_of(num.begin(), num.end(), ::isdigit);
}

/*The function find_song takes in two strings, the desired song title/artist pair, 
  and the vectors of strings that represent the playlist. The function loops through 
  the song_title vector, searching for the desired song title. If found, it will also 
  to confirm the artist is also correct, as there are instances of several artists
  having the same name for a song. If that also passes, the function will return the 
  song title's index. If not found, the function will return -1 to represent the failure
  to do so.*/
int find_song(std::string &title, std::string &artist, std::vector<std::string> &songs, 
    std::vector<std::string> &bands){
    unsigned int song_index = 0;
    std::string lib_title = "";
    std::string lib_band = "";
    while (title != lib_title && song_index < songs.size()){
        lib_title = songs[song_index];
        lib_band = bands[song_index];
        song_index++;
        if (title == lib_title && lib_band != artist)//Check artist match
            lib_title = "";
    }
    if (title == lib_title)//Match found
        return song_index - 1;
    return -1;
}

/*The function parse_command takes a parsed line from the actions input file and 
  concatenates the necessary strings, aminly by noting where the double quotation 
  marks are for the song title, then the remaining terms for the song artist(s), 
  unless thhe action is to move a song in the playlist, for which it also stores
  the location of where the song is to be moved.*/
std::vector<std::string> parse_command(std::vector<std::string> &cmd){
    std::vector<std::string> command;
    if (cmd.size() > 1){ //If previous/next command, skip
        std::string title = cmd[1].substr(1, cmd[1].size()); //First word of title
        int cmd_index = 1;
        if (title[title.size()-1] != '"'){
            while (cmd[cmd_index].back() != '"'){
                cmd_index++;
                title += " " + cmd[cmd_index];
            }
        }
        title.pop_back(); //Remove second '"' from song title
        command.push_back(title);
        std::string band = cmd[cmd_index+1];
        if (cmd.size() > cmd_index+2){ //If band name > 1 word
            for (int i = cmd_index+2; i < cmd.size()-1; i++)
                band += " " + cmd[i];
        }
        if (cmd[0] == "move"){
            command.push_back(band);
            command.push_back(cmd[cmd.size()-1]);
        } else if (cmd.size() > cmd_index+2){ //If band name > 1 word
            band += " " + cmd[cmd.size()-1];
            command.push_back(band);
        } else {
            command.push_back(band);
        }
    }
    return command;
}

/*The function remove_song calls find_song to locate the desired song to be deleted. If
  found, remove, otherwise throw an error.*/
int remove_song(std::vector<std::string> &cmd, std::vector<std::string> &song_list, 
    std::vector<std::string> &artist_list, int &loc){
    int song_loc = find_song(cmd[0], cmd[1], song_list, artist_list);
    if (song_loc != -1){//If song found
        song_list.erase(song_list.begin() + song_loc);
        artist_list.erase(artist_list.begin() + song_loc);
        if (song_loc <= loc) //Song removed before currently playing, shift current index
            loc--;
    } else {
        std::cerr << "Requested song could not be found." << std::endl;
        return 1;
    }
    return 0; //MOVE PLACEMENT
}

/*The function move_song calls find_song to locate the desired song to be moved. If
  found, check if the new location is a valid number and position, then execute.*/
int move_song(std::vector<std::string> &cmd, std::vector<std::string> &song_list, 
    std::vector<std::string> &artist_list, int &loc){
    int song_loc = find_song(cmd[0], cmd[1], song_list, artist_list);
    if (song_loc != -1){//If song found
        std::string is_num = cmd[2];
        if (is_number(is_num) && std::stoi(is_num) <= song_list.size()){
            int new_loc = std::stoi(is_num) - 1;
                    
            song_list.erase(song_list.begin() + song_loc);
            song_list.insert(song_list.begin() + new_loc, cmd[0]);

            artist_list.erase(artist_list.begin() + song_loc);
            artist_list.insert(artist_list.begin() + new_loc, cmd[1]);

            //Logic required for shifting currently playing index. need to change index when:
            //- Currently playing song is moved to new location
            //- Moved song goes from being after currently playing song to before it
            //- Moved song goes from being before currently playing song to after it
            if (song_loc == loc)
                loc = new_loc;
            else if (new_loc <= loc && loc < song_loc)
                loc++;
            else if (new_loc >= loc && loc > song_loc)
                loc--;
            return 0;
        } else {
            std::cerr << "Move request could not be made." << std::endl;
            return 1; 
        }
    } else {
        std::cerr << "Requested song could not be found." << std::endl;
        return 1;
    }
}

/*The function skip_song takes one of the previous/next commands, the current index of the
  playing song, as well as number of songs in the playlist. The index changes based on the
  the command called, and is moved in accordance to boundary conflicts. */
void skip_song(const std::string &cmd, int &loc, const int &song_count){
    if (cmd == "previous"){
        loc--;
        if (loc < 0)
            loc = song_count - 1;
    } else if (cmd == "next"){
        loc++;
        if (loc >= song_count)
            loc = 0;
    }
}

/*The function perform-actions takes in the actions file as an input, along with two 
  vectors of strings that represent all of the song titles from the input file, along 
  with all of the credited artists. The function goes line by line in the input file, 
  and while there is text to retrieve, parse_line is called, then parse_command to get 
  a simple vector of 3-4 elements long, stored as [command, song title, artist(s), move
  location, if necessary]. Depending on the command, one of four actions will be performed
  with the add command simply adding song and artist(s) to the playlist vectors.*/
int perform_actions(std::ifstream &input, std::vector<std::string> &songs, std::vector<std::string>
    &artists, int &current){
    if (input.is_open()){//If the file is open
        std::string row;
        std::vector<std::string> line;
        while (input.good()){//While there is text to retrieve, store each line in the vector
            getline(input, row);
            if (row.size() > 0) {
                line = parse_line(row);
                std::vector<std::string> action = parse_command(line);

                if (line[0] == "add"){
                    songs.push_back(action[0]);
                    artists.push_back(action[1]);
                } else if (line[0] == "remove"){
                    remove_song(action, songs, artists, current);
                } else if (line[0] == "move"){
                    move_song(action, songs, artists, current);
                } else if (line[0] == "previous" || line[0] == "next"){
                    skip_song(line[0], current, songs.size());
                } else {
                    std::cerr << "Invalid action requested." <<std::endl;
                    return 1;
                }
                std::cout << current <<std::endl;
            }
        }
        input.close();
        return 0;
    } else {
        std::cerr << "Unable to open file" <<std::endl;
        return 1;
    }
}

//./nyplaylists.exe playlist_tiny1.txt actions1.txt output.txt
int main(int argc, char* argv[]){
    if (argc == 4){
        std::ifstream playlist(argv[1]);
        std::ifstream actions(argv[2]);
        std::ofstream output(argv[3]);

        std::vector<std::string> playlist_songs; //Song titles from playlist input
        std::vector<std::string> playlist_artists; //Respective artist(s) from playlist input
        int current_song; //Currently playing index

        parse_playlist(playlist, playlist_songs, playlist_artists, current_song);

        perform_actions(actions, playlist_songs, playlist_artists, current_song);

        /*Open the desired output file and loop through the playlist song vector. Both the
        song and artist vectors are the same size, and each index matches, so we only need
        one for loop. Print one song-artist combination per line until completion.*/
        if (output.is_open()){
            for (unsigned int i = 0; i < playlist_songs.size(); i++){
                output << '"' << playlist_songs[i] << "\" " << playlist_artists[i];
                if (i == current_song)//If currently played song
                    output << " current";
                output << "\n";
            }
            output.close();
        } else {
            std::cerr << "Unable to open file" <<std::endl;
            return 1;
        }

    } else {
        std::cerr << "Incorrect amount of command line arguments." <<std::endl;
        return 1;
    }
}