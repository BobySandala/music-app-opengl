#include "audio/audio_engine.h"
#include <iostream>

int main()
{
    AudioEngine audio;

    std::string url = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";

    if (!audio.play(url))
    {
        std::cout << "Playback failed\n";
        return -1;
    }

    std::cout << "Set volume (0.0 - 1.0): ";
    float v;
    std::cin >> v;
    audio.setVolume(v);
    std::cin.ignore(); // clear newline

    std::cout << "Press ENTER to stop\n";
    std::cin.get();

    audio.stop();
}