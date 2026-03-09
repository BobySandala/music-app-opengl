#include "youtube_api.h"

#include <cstdio>
#include <memory>
#include <sstream>
#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<SongResult> YoutubeAPI::performSearch(const std::string& query)
{
    std::vector<SongResult> localResults;

    std::string command =
        "yt-dlp \"ytsearch10:" + query + "\" --dump-json --flat-playlist 2>/dev/null";

    auto pipe = std::unique_ptr<FILE, decltype(&pclose)>(
        popen(command.c_str(), "r"),
        pclose
    );

    if (!pipe)
        return localResults;

    char buffer[1024];
    std::stringstream output;

    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr)
        output << buffer;

    std::string line;
    std::stringstream stream(output.str());

    while (std::getline(stream, line))
    {
        try
        {
            json data = json::parse(line);

            SongResult song;

            song.title = data.value("title", "");
            song.uploader = data.value("uploader", "");
            song.videoId = data.value("id", "");
            song.duration = data.value("duration", 0);

            localResults.push_back(song);
        }
        catch (...)
        {
        }
    }

    return localResults;
}

void YoutubeAPI::search(const std::string& query)
{
    if (searching)
        return;

    searching = true;
    ready = false;

    worker = std::thread([this, query]()
    {
        auto res = performSearch(query);

        {
            std::lock_guard<std::mutex> lock(resultMutex);
            results = std::move(res);
        }

        ready = true;
        searching = false;
    });

    worker.detach();
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