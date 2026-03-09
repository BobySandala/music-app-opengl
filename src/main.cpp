#include "core/window.h"
#include "network/youtube_api.h"

#include <iostream>

int main()
{
    Window window(800, 600, "OpenGL Music Player");

    if (!window.init())
        return -1;

    YoutubeAPI youtube;

    youtube.search("daft punk");

    bool printed = false;

    while (!window.shouldClose())
    {
        window.update();

        if (youtube.hasResults() && !printed)
        {
            printed = true;

            for (auto& r : youtube.getResults())
            {
                std::cout << r.title << " - " << r.uploader << std::endl;
            }
        }
    }

    window.shutdown();
}