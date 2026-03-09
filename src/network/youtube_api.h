#pragma once

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

struct SongResult
{
    std::string title;
    std::string uploader;
    std::string videoId;
    int duration;
};

class YoutubeAPI
{
private:
    std::vector<SongResult> results;

    std::atomic<bool> searching = false;
    std::atomic<bool> ready = false;

    std::mutex resultMutex;

    std::thread worker;

    std::vector<SongResult> performSearch(const std::string& query);

public:
    void search(const std::string& query);

    bool isSearching();
    bool hasResults();

    std::vector<SongResult> getResults();
};