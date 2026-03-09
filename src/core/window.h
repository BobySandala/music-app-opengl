#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
private:
    int width;
    int height;
    const char* title;

    GLFWwindow* window;

public:
    Window(int w, int h, const char* t);

    bool init();
    void update();
    bool shouldClose();

    void shutdown();
};