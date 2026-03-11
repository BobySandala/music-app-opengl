#pragma once
#include "miniaudio/miniaudio.h"
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>

enum class AudioEngineState
{
    Stopped,
    Playing,
    Paused
};

enum class AudiocommandType
{
    Download, Play, Stop, Resume, Exit, TogglePlay, Pause
};

struct AudioCommand
{
    AudiocommandType type;
    std::string url;
};

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    bool play();
    bool setURL(const std::string& youtubeUrl);
    void stop();
    void pause();
    void resume();
    void togglePlayPause();

    void setVolume(float volume);   // 0.0 → silent, 1.0 → normal
    float getVolume() const;

    bool isDownloading() const;

    void enqueue(const AudioCommand& cmd);

private:
    ma_decoder decoder{};
    ma_device device{};

    std::string tempFile;

    std::atomic <AudioEngineState> state {AudioEngineState::Stopped};
    std::atomic <float> volume {1.0f};
    std::atomic <bool> downloading {false};
    std::atomic <bool> ready {false};
    std::atomic <bool> running {true};

    std::mutex threadMutex;
    std::thread playbackThread;

    std::thread downloadThread {};

    std::queue<AudioCommand> commandQueue;

    std::mutex queueMutex;
    std::condition_variable queueCV;

    std::thread worker;

    static void dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount);
    void playbackWorker();

    void downloadTask(const std::string& youtubeUrl);

    void processQueue();
};