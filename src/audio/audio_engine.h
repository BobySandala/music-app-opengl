#pragma once
#include "miniaudio/miniaudio.h"
#include <string>
#include <atomic>
#include <mutex>
#include <thread>

enum class AudioEngineState
{
    Stopped,
    Playing,
    Paused
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

private:
    ma_decoder decoder{};
    ma_device device{};

    std::string tempFile;

    std::atomic <AudioEngineState> state {AudioEngineState::Stopped};
    std::atomic <float> volume {1.0f};

    std::mutex threadMutex;
    std::thread playbackThread;

    static void dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount);
    void playbackWorker();
};