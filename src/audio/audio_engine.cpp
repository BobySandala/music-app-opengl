#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include "audio/audio_engine.h"

#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <chrono>

const std::string folder_name = "opengl_music_player";
static std::string extractVideoId(const std::string& url);

AudioEngine::AudioEngine() {}

AudioEngine::~AudioEngine()
{
    stop();
}

bool AudioEngine::setURL(const std::string& youtubeUrl)
{
    stop();
    ma_device_set_master_volume(&device, volume);

    std::string videoId = extractVideoId(youtubeUrl);

    // Temp directory
    std::filesystem::path tempDir =
        std::filesystem::temp_directory_path() / "opengl_music_player";

    std::filesystem::create_directories(tempDir);

    // File name = videoId.mp3
    tempFile = (tempDir / (videoId + ".mp3")).string();

    std::string cmd =
        "yt-dlp -f bestaudio "
        "--extract-audio "
        "--audio-format mp3 "
        "-o \"" + tempFile + "\" \"" + youtubeUrl + "\" --quiet";

    #ifdef _WIN32
        cmd += " 2>NUL";
    #else
        cmd += " 2>/dev/null";
    #endif


    if (!std::filesystem::exists(tempFile))
    {
        std::cout << "Downloading audio...\n";
        if (system(cmd.c_str()) != 0)
        {
            std::cerr << "Download failed\n";
            return false;
        }
    }
    else
    {
        std::cout << "Using cached audio\n";
    }

    if (!std::filesystem::exists(tempFile))
    {
        std::cerr << "Downloaded file not found\n";
        return false;
    }

    std::cout << "Download finished: " << tempFile << std::endl;
    return true;
}

bool AudioEngine::play()
{
    if (!std::filesystem::exists(tempFile))
    {
        std::cerr << "[Audio Engine] play - Audio file not found\n";
        return false;
    } 
    else
    {
        std::lock_guard<std::mutex> lock(threadMutex);
        state = AudioEngineState::Playing;
        playbackThread = std::thread(&AudioEngine::playbackWorker, this);
        playbackThread.detach();
    }
    return true;
}

void AudioEngine::playbackWorker()
{    
    // Init decoder
    if (ma_decoder_init_file(tempFile.c_str(), NULL, &decoder) != MA_SUCCESS)
    {
        std::cerr << "Decoder init failed\n";
        state = AudioEngineState::Stopped;
        return;
    }

    // Setup device
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format   = decoder.outputFormat;
    config.playback.channels = decoder.outputChannels;
    config.sampleRate        = decoder.outputSampleRate;
    config.dataCallback      = dataCallback;
    config.pUserData         = this;

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
    {
        std::cerr << "Device init failed\n";
        ma_decoder_uninit(&decoder);
        return;
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
        std::cerr << "Device start failed\n";
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return;
    }

    state = AudioEngineState::Playing;

    std::cout << "Playback started\n";
    return;
}

void AudioEngine::togglePlayPause()
{
    if (state == AudioEngineState::Playing) 
    {
        std::cout << "[Audio Engine] - toggle play: pausing\n";
        pause();
    }
    else if (state == AudioEngineState::Paused) 
    {
        std::cout << "[Audio Engine] - toggle play: resuming\n";
        resume();
    }
}

// true to pause, false to resume
void AudioEngine::pause()
{
    if (state != AudioEngineState::Playing) return;

    ma_device_stop(&device);
    state = AudioEngineState::Paused;

    std::cout << "Playback paused\n";
}

void AudioEngine::resume()
{
    if (state != AudioEngineState::Paused) return;

    ma_device_start(&device);
    state = AudioEngineState::Playing;

    std::cout << "Playback resumed\n";
}

void AudioEngine::stop()
{
    pause();

    // Uninit device and decoder
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    // delete temp file
    if (!tempFile.empty() && std::filesystem::exists(tempFile))
        std::filesystem::remove(tempFile);

    tempFile.clear();
}

// called when the device needs more samples to play
void AudioEngine::dataCallback(ma_device* pDevice, void* output, const void* input, ma_uint32 frameCount)
{
    AudioEngine* engine = static_cast<AudioEngine*>(pDevice->pUserData);
    if (!engine)
    {
        std::cerr << "[Audio Engine - dataCallback] Cast Failed";
        return;
    }

    if (engine->state == AudioEngineState::Paused)
    {
        // Fill silence
        std::memset(output, 0, frameCount * engine->decoder.outputChannels * ma_get_bytes_per_sample(engine->decoder.outputFormat));
        return;
    }

    ma_uint64 framesRead;
    ma_decoder_read_pcm_frames(&engine->decoder, output, frameCount, &framesRead);

    // Apply volume (assuming float format)
    if (engine->decoder.outputFormat == ma_format_f32)
    {
        float* out = static_cast<float*>(output);
        for(ma_uint64 i = 0; i < framesRead * engine->decoder.outputChannels; i++)
        {
            out[i] *= engine->volume.load();
        }
    }

    if (framesRead < frameCount)
    {
        size_t offset = framesRead * engine->decoder.outputChannels * ma_get_bytes_per_sample(engine->decoder.outputFormat);
        std::memset((char*)output + offset, 0, (frameCount - framesRead) * engine->decoder.outputChannels * ma_get_bytes_per_sample(engine->decoder.outputFormat));
    }
}

void AudioEngine::setVolume(float v)
{
    if (v < 0.0f) v = 0.0f;
    if (v > 1.0f) v = 1.0f;

    volume = v;
}

float AudioEngine::getVolume() const
{
    return volume;
}

static std::string extractVideoId(const std::string& url)
{
    auto pos = url.find("v=");
    if (pos == std::string::npos)
        return "song";

    std::string id = url.substr(pos + 2);

    auto end = id.find('&');
    if (end != std::string::npos)
        id = id.substr(0, end);

    return id;
}