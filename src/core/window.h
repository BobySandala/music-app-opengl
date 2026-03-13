#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const char* title);
    Window();

    bool init();
    void update();

    bool shouldClose() const;
    void shutdown();

    GLFWwindow* getNativeWindow() const;

    int getWidth() { return width; };
    int getHeight() { return height; };

private:
    int width{0};
    int height{0};
    const char* title{nullptr};

    GLFWwindow* window{nullptr};
};