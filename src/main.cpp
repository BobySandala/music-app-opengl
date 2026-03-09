#include "core/window.h"

int main()
{
    Window window(800, 600, "OpenGL Music Player");

    if (!window.init())
        return -1;

    while (!window.shouldClose())
    {
        window.update();
    }

    window.shutdown();
}