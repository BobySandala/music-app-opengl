#pragma once
#include "miniaudio/miniaudio.h"
#include <string>

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    bool play(const std::string& youtubeUrl);
    void stop();

    void setVolume(float volume);   // 0.0 → silent, 1.0 → normal
    float getVolume() const;

private:
    ma_decoder decoder{};
    ma_device device{};

    std::string tempFile;
    bool playing {false};

    float volume {1.0f};

    static void dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount);
};