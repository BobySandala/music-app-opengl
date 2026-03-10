#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include "audio/audio_engine.h"

#include <filesystem>
#include <iostream>
#include <cstdlib>

const std::string folder_name = "opengl_music_player";
static std::string extractVideoId(const std::string& url);

AudioEngine::AudioEngine() {}

AudioEngine::~AudioEngine()
{
    stop();
}

bool AudioEngine::play(const std::string& youtubeUrl)
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

    // Init decoder
    if (ma_decoder_init_file(tempFile.c_str(), NULL, &decoder) != MA_SUCCESS)
    {
        std::cerr << "Decoder init failed\n";
        return false;
    }

    // Setup device
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format   = decoder.outputFormat;
    config.playback.channels = decoder.outputChannels;
    config.sampleRate        = decoder.outputSampleRate;
    config.dataCallback      = dataCallback;
    config.pUserData         = &decoder;

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
    {
        std::cerr << "Device init failed\n";
        ma_decoder_uninit(&decoder);
        return false;
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
        std::cerr << "Device start failed\n";
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return false;
    }

    playing = true;

    std::cout << "Playback started\n";
    return true;
}

void AudioEngine::stop()
{
    if (!playing)
        return;

    ma_device_stop(&device);
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);

    playing = false;

    // delete temp file
    if (!tempFile.empty() && std::filesystem::exists(tempFile))
        std::filesystem::remove(tempFile);

    tempFile.clear();
}

void AudioEngine::dataCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount)
{
    ma_decoder* decoder = (ma_decoder*)device->pUserData;
    ma_decoder_read_pcm_frames(decoder, output, frameCount, NULL);
}

void AudioEngine::setVolume(float v)
{
    if (v < 0.0f) v = 0.0f;
    if (v > 1.0f) v = 1.0f;

    volume = v;

    if (playing)
        ma_device_set_master_volume(&device, volume);
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