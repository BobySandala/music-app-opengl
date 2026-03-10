#include "youtube_api.h"

#include <cstdio>
#include <memory>
#include <sstream>
#include <iostream>

#include <nlohmann/json.hpp>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
std::string redirectStderr = " 2>NUL";
#else
std::string redirectStderr = " 2>/dev/null";
#endif

using json = nlohmann::json;

// Overload operator<<
std::ostream& operator<<(std::ostream& os, const SongResult& song)
{
    os << "Title: " << song.title
       << ", Uploader: " << song.uploader
       << ", VideoID: " << song.videoId
       << ", Duration: " << song.duration << "s";
    return os;
}

std::vector<SongResult> YoutubeAPI::performSearch(const std::string& query)
{
    std::cout << "Performing search for: " << query << std::endl;
    std::vector<SongResult> localResults;

    std::string command =
        "yt-dlp \"ytsearch" + std::to_string(maxResults) + ":" + query + "\" "
        "--dump-json --flat-playlist" + redirectStderr;
    std::cout << "Executing command: " << command << std::endl;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return localResults;

    char buffer[8192];
    int lineCount = 0;
    while(fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        if(cancelFlag) {
            std::cout << "Search cancelled: " << query << std::endl;
            break;
        }
        
        std::string line(buffer);
        line.erase(line.find_last_not_of("\n\r") + 1);
        
        //std::cout << "\nLine output: " << line; // Debug: print raw output
        try {
            json data = json::parse(line);
            SongResult song;
            song.title = data.value("title", "");
            song.uploader = data.value("uploader", "");
            song.videoId = data.value("id", "");
            song.duration = data.value("duration", 0);

            localResults.push_back(song);
            //std::cout << "Parsed song: " << song; // Debug: print parsed song
        } catch(...) {
            // ignore parse errors
        }
    }
    std::cout << "\nSearch completed for: " << query << " with " << localResults.size() << " results." << std::endl;

    pclose(pipe);
    return localResults;
}

void YoutubeAPI::search(const std::string& query)
{
    // Cancel previous search
    if(searching)
    {
        cancelFlag = true;
        if(worker.joinable())
            worker.join(); // wait safely for old thread
    }

    cancelFlag = false;
    searching = true;
    ready = false;

    worker = std::thread([this, query]()
    {
        try
        {
            auto res = performSearch(query);

            if(!cancelFlag)
            {
                std::lock_guard<std::mutex> lock(resultMutex);
                results = std::move(res);
                ready = true;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "Search thread exception: " << e.what() << std::endl;
        }
        catch(...)
        {
            std::cerr << "Search thread unknown exception" << std::endl;
        }

        searching = false;
    });
}

bool YoutubeAPI::isSearching()
{
    return searching;
}

bool YoutubeAPI::hasResults()
{
    return ready;
}

std::vector<SongResult> YoutubeAPI::getResults()
{
    std::lock_guard<std::mutex> lock(resultMutex);
    return results;
}

// getters and setters for maxResults
void YoutubeAPI::setMaxResults(int max) { maxResults = max; }
int YoutubeAPI::getMaxResults() const { return maxResults; }

// destructor to ensure worker thread is joined
YoutubeAPI::~YoutubeAPI()
{
    cancelFlag = true;
    if(worker.joinable())
        worker.join();
}