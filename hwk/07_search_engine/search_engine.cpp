#include <string>
#include <map>
#include <set>
#include <list>
#include <regex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <numeric>

typedef std::set<std::string> Documents;
typedef std::pair<std::string, double> Scores;

// Given function to parse an HTML file and extract links to local files
std::list<std::string> extractLinksFromHTML(const std::string& fileContent) {
    std::list<std::string> links;
    // regular expression to match href attributes in anchor tags
    std::regex linkRegex("<a\\s+[^>]*href\\s*=\\s*['\"]([^'\"]+)['\"][^>]*>");
    std::smatch match;

    // search for links in the HTML content
    std::string::const_iterator start = fileContent.cbegin();
    while (std::regex_search(start, fileContent.cend(), match, linkRegex)) {
        if (match.size() > 1) {
            links.push_back(match[1].str());
        }
        start = match.suffix().first;
    }

    return links;
}

// This function extracts all the text between two given HTML tags
std::string extractTag(const std::string& content, const std::string& tagStart,
    const std::string& tagEnd) {

    int startPos = content.find(tagStart);
    if (startPos == std::string::npos) return "";
    
    startPos += tagStart.length();
    int endPos = content.find(tagEnd, startPos);
    if (endPos == std::string::npos) return "";
    
    return content.substr(startPos, endPos - startPos);
}

// Helper function to generate Title from article
std::string extractTitle(const std::string& content) {
    return extractTag(content, "<title>", "</title>");
}

// Helper function to generate URL from article
std::string extractURL(const std::string& content) {
    return extractTag(content, "<h1>", "</h1>");
}

// Helper function to generate description from article
std::string extractDescription(const std::string& content) {
    std::string tagStart = "content=\"";
    int startPos = content.find(tagStart);
    if (startPos == std::string::npos) return "";
    
    startPos += tagStart.length();
    int endPos = content.find("\">", startPos);
    if (endPos == std::string::npos) return "";
    
    return content.substr(startPos, endPos - startPos);
}

// Helper function to generate snippet from article
std::string extractSnippet(const std::string& content, const std::string& query) {
    std::string bodyText = extractTag(content, "<body>", "</body>"); // Entire body text
    std::string keyword = query;
    if (query[0] == '"') keyword = query.substr(1,query.size()-2); // Remove quotes

    int query_position = bodyText.find(keyword,0);
    while (query_position != std::string::npos) {
        if (!std::isalnum(bodyText[query_position-1]) && 
            !std::isalnum(bodyText[query_position+keyword.size()]))
            break; // If phrase match found, break
        query_position = bodyText.find(keyword,query_position+1);
    }
    // If phrase match not found, generate first keyword and redo
    if (query_position == std::string::npos) {
        std::istringstream iss(query);
        std::string word;
        std::getline(iss, word, ' ');
        query_position = bodyText.find(word,0);

        while (query_position != std::string::npos) {
            if (!std::isalnum(bodyText[query_position-1]) && 
                !std::isalnum(bodyText[query_position+word.size()]))
                break; // If regular match found, break
            query_position = bodyText.find(word,query_position+1);
        }
    }
    // Find start of sentence that contains the keyword/phrase
    int sentence_position = bodyText.rfind(".", query_position) + 1;
    while (sentence_position < bodyText.length() && std::isspace(bodyText[sentence_position]))
        sentence_position++; // In case of extra whitespace after '.', shift starting index

    std::string snippet = bodyText.substr(sentence_position, 120);
    if (snippet.size() < 120) snippet.append(120 - snippet.length(), ' ');

    return snippet;
}

// This function returns the index of the file name in a path, not important if no '/'
int find_file_name(const std::string file){
    int lastSlashPos = file.find_last_of('/');
        if (lastSlashPos != std::string::npos) return lastSlashPos+1;
    return -1;
}

// This function takes in a search query and in turn creates a map that stores all of the words that
// make up the query. If the seach query is a phrase, it removes the quotes and also adds the phrase
// to the map. Each key in the map is linked to an integer that represents the total count of that
// given word or phrase in all of the searched pages.
std::map<std::string,int> parse_query(const std::string &query) {
    std::map<std::string,int> search_query;
    std::string query_no_quotes = query;
    if (query[0] == '"') {
        query_no_quotes = query.substr(1,query.size()-2);
        search_query.insert(std::pair<std::string,int>(query_no_quotes,0));
    }
    std::istringstream iss(query_no_quotes);
    std::string word;

    // Tokenize search phrase using delimiter
    while (std::getline(iss, word, ' ')) {
        search_query.insert(std::pair<std::string,int>(word,0));
    }
    return search_query;
}

// This function takes in the given file name, the content of said file, and two maps, one which
// stores the total count of each keyword, and another that stores the file names that contain all
// of the desired keywords. The function iterates through the keywords, and tallies the number of
// times the given keyword/phrase was found. The total is also stored in a vector. If any of these
// counts equal zero, then we do not save the file name. Since the keys in the queries map are
// sorted, via the nature of maps, the elements in the vectors stored as values in the urls map are
// naturally sorted to the order of the keywords, which greatly helps in score determination later.
bool find_all_instances(const std::string &url, const std::string &file, 
    std::map<std::string,int> &queries, std::map<std::string,std::vector<int>> &urls) {
    
    bool flag = true; // If true, all keywords exist in the given page
    std::vector<int> count;
    std::map<std::string,int>::iterator itr = queries.begin();
    for (; itr != queries.end(); itr++) {
        int occurrences = 0;
        std::string word = itr->first;
        int position = file.find(word,0);
        while (position != std::string::npos) { // Counts all instances of keyword(s) in file
            if (!std::isalnum(file[position-1]) && !std::isalnum(file[position+word.size()])) {
                occurrences++;
                itr->second++;
            }
            position = file.find(word,position+1);
        }
        count.push_back(occurrences);
        if (occurrences == 0) flag = false; // Keyword not found
    }
    if (flag) urls.insert(std::pair<std::string,std::vector<int>>(url, count));
    return flag;
}

// This function is the main function used to navigate through the various given pages, starting
// with the index.html page. The function calls find_all_instances to locate all of the keywords and
// phrases in the given file, and if it passes true, store the file name in the inverted index.
// Next, the function extracts the outgoing links. If a given link is not in the inverted index and
// has not been travelled to yet, the function is recursively called. This function also stores the
// total length og the file, as well as keeping tracking of its outgoing links and what it is an
// ingoing link to.
void webcrawl(const std::string &query, std::map<std::string,Documents> &inverted_index, 
    std::map<std::string,int> &count, std::map<std::string,std::vector<int>> &url_count,
    std::string url, std::set<std::string> &travelled, std::vector<int> &all_lengths,
    std::map<std::string,Documents> &outgoing, std::map<std::string,Documents> &ingoing,bool flag){

    std::ifstream fileStream(url);
    if (fileStream.is_open()) {
        std::string fileContent((std::istreambuf_iterator<char>(fileStream)),
            std::istreambuf_iterator<char>());
        std::string path = extractURL(fileContent); // Get page's URL
        std::string shortened_file = path.substr(find_file_name(path)); // Only the file name
        std::string path_to = path.substr(0,find_file_name(path)); // Only the file path
        travelled.insert(shortened_file); // Store file name, so we don't revisit
        if (!flag) all_lengths.push_back(fileContent.size());

        if (find_all_instances(path, fileContent, count, url_count)) // Check all keywords exist
            inverted_index[query].insert(shortened_file);

        std::list<std::string> links = extractLinksFromHTML(fileContent); // Get outgoing links
        if (!links.empty()) {
            for (std::list<std::string>::iterator itr = links.begin(); itr != links.end(); itr++) {
                std::string alt_file = *itr; // Get the URL's file name
                if (find_file_name(*itr) != -1) alt_file = (*itr).substr(find_file_name(*itr));

                if (!flag) {
                    ingoing[alt_file].insert(url);
                    outgoing[url].insert(alt_file);
                } // Check if page has already been visited
                if (inverted_index[query].find(alt_file) == inverted_index[query].end() && 
                    travelled.find(alt_file) == travelled.end()) {
                    webcrawl(query, inverted_index, count, url_count, (path_to+(*itr)), travelled,
                        all_lengths, outgoing, ingoing, flag);
                }
            }
        } else return;
    } else {
        std::cerr << "Unable to open HTML file" << url << std::endl;
        return;
    }
}

// This function takes in the file name and search query, then prints the desired info we want into
// the output file. This info is the page's title, url, description, and a 120 character snippet.
void print_data(const std::string &link, const std::string &query, std::ofstream &output) {
    std::ifstream fileStream(link);
    if (fileStream.is_open()) {
        std::string fileContent((std::istreambuf_iterator<char>(fileStream)),
            std::istreambuf_iterator<char>());
        output << std::endl;
        output << "Title: " << extractTitle(fileContent) << std::endl;
        output << "URL: " << link << std::endl;
        output << "Description: " << extractDescription(fileContent) << std::endl;
        output << "Snippet: " << extractSnippet(fileContent, query) << std::endl;
    }
}

 // This function takes in a vector of all of the files that include the search query and each
 // files' score, then determines what to output, given the number of files found.
void output_print(const std::vector<Scores> &files,const std::string &query,std::ofstream &output){
    if (files.size() == 0) output << "Your search - "<< query <<" - did not match any documents." 
        << std::endl;
    else {
        output << "Matching documents: " <<std::endl;
        for (unsigned int i = 0; i < files.size(); i++) {
            print_data(files[i].first, query, output);
        }
    }
}

// This function is used to calculate the scores/rankings for all of the files/pages that relate to
// the given search query. First, the function checks if the query is a phrase, and if so, it takes
// note of the index in the url_count map's values that corresponds to the full phrase. Next, we
// loop through each file name, and calculate the following: for each keyword, the total keyword
// density, and then the keyword density for the given file, adding all of the densities found for
// each keyword. Next, the backlinks score is found, and then the final score and ranking for the
// file. The file name is stored alongside its score in a apir, which is then stored in a vector.
// This vector is then sorted by score, descending order, then returned to be printed in the output
// file. 
std::vector<Scores> generate_scores(const std::map<std::string,std::vector<int>> &url_count, 
    const std::map<std::string,int> &query_count, const std::string &query, int sum, 
    std::map<std::string,Documents> &outgoing, std::map<std::string,Documents> &ingoing) {
    
    std::string full_query = query; //Remove quotes from query
    if (query[0] == '"') full_query = query.substr(1,query.size()-2);

    int do_not_include = -1; // Flag for phrase matches, don't include full phrase
    int iterator = 0;
    std::map<std::string,int>::const_iterator keyword_itr = query_count.begin();
    for (; keyword_itr != query_count.end(); keyword_itr++) { // Find index of full phrase for phrase matches
        if (query_count.size() == 1 || keyword_itr->first != full_query) iterator++;
        else do_not_include = iterator;
    }


    std::vector<Scores> scored_pages;
    std::map<std::string,std::vector<int>>::const_iterator file_itr = url_count.begin();
    for (; file_itr != url_count.end(); file_itr++) { // Loop through pages for given search
        double keyword_density = 0; // Keyword Density for given search
        std::ifstream fileStream(file_itr->first);
        if (fileStream.is_open()) {
            std::string fileContent((std::istreambuf_iterator<char>(fileStream)),
                std::istreambuf_iterator<char>());
            int total_length = fileContent.size(); // Page size
            iterator = 0;
            keyword_itr = query_count.begin();
            for (; keyword_itr != query_count.end(); keyword_itr++) {
                if (iterator != do_not_include) {
                    double total_keyword_density = (keyword_itr->second*1.0) / sum;
                    double keyword_score = (file_itr->second[iterator]*1.0) / ((total_length) * 
                        1.0 * total_keyword_density);
                    keyword_density += keyword_score;
                }
                iterator++;
            }

            //Backlinks Score
            double backlinks_score = 0.0;
            std::string file_name = file_itr->first.substr(find_file_name(file_itr->first));
            std::set<std::string> pages_pointing_to = ingoing[file_name];
            std::set<std::string>::iterator set_itr = pages_pointing_to.begin();
            for (; set_itr != pages_pointing_to.end(); set_itr++) {
                double backlink = 1.0 / (1+outgoing[*set_itr].size()+0.0);
                backlinks_score += backlink;
            }
            // Final Score Calculation
            double final_score = (0.5 * keyword_density + 0.5 * backlinks_score);
            scored_pages.push_back(Scores(file_itr->first,final_score));
        }
    }
    // Sort pages/articles by generated score
    std::sort(scored_pages.begin(), scored_pages.end(), [](const Scores& a, const Scores& b) {
        return a.second > b.second;
    });

    return scored_pages;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << " Incorrect number of commands" << std::endl;
        return 1;
    }
    
    std::string seed(argv[1]);
    std::ifstream input(argv[2]);
    
    if (!input.is_open()) {
        std::cerr << "Unable to open input file" << std::endl;
        return 1;
    }
    
    std::map<std::string,Documents> inverted_index; // Inverted Index
    std::map<std::string,Documents> outgoing_links; // File Name -> Links it contains
    std::map<std::string,Documents> ingoing_links; // File Name -> Links pointing to it
    std::vector<int> total_length; // Vector of all file lengths
    bool infoFlag = false; // Boolean flag, so prev 3 lines aren't generated multipl times
    std::string query;
    
    int output_count = 1; // Used for output file generation
    while (input.good()) {
        getline(input, query); // Line by line, get query from input file
        if (query.size() > 0) {
            std::string output_file = "out"+std::to_string(output_count)+".txt";
            std::ofstream output(output_file);

            Documents references;
            inverted_index.insert(std::pair<std::string,Documents>(query,references));
            std::map<std::string,int> query_count = parse_query(query); // Keyword -> Total
            std::map<std::string,std::vector<int>> url_count; // URL -> Sum of each Keyword
            std::set<std::string> travelled_pages;

            webcrawl(query, inverted_index, query_count, url_count, seed, travelled_pages,
                total_length, outgoing_links, ingoing_links, infoFlag);
            infoFlag = true;

            int sum = std::accumulate(total_length.begin(), total_length.end(), 0);
            std::vector<std::pair<std::string,double>> sorted_articles = generate_scores(url_count,
                query_count, query, sum, outgoing_links, ingoing_links);
            output_print(sorted_articles, query, output);
        }
        output_count++;
    }
    return 0;
}