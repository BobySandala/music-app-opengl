#include "window.h"
#include <glad/glad.h>
#include <iostream>

Window::Window(int w, int h, const char* t)
{
    width = w;
    height = h;
    title = t;
}

bool Window::init()
{
    if (!glfwInit())
    {
        std::cout << "GLFW init failed\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window)
    {
        std::cout << "Window creation failed\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD failed\n";
        return false;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    return true;
}

void Window::update()
{
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::shutdown()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}