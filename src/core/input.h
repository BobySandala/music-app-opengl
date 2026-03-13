#pragma once

#include <unordered_map>
#include <functional>
#include "event/events.h"
#include "event/event_bus.h"
#include <GLFW/glfw3.h>

class Input
{
public:
    Input(EventBus& bus);

    void init(GLFWwindow* window);

    // Optional: check current key state
    bool isKeyPressed(int key) const;

    void getMousePos(double* x, double* y);
    void getRelMousePos(double* x, double* y, int width, int height);

    void update();

private:
    GLFWwindow* window{nullptr};
    EventBus& bus;
    
    // Mouse position
    double mx{0};
    double my{0};

    // Key state tracking
    bool keys[512]{};

    // Map keys to event emitters
    std::unordered_map<int, std::function<void()>> keyBindings;

    // GLFW callback
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    // Internal instance pointer for static callback
    static Input* instance;

    void handleKey(int key, int action);
    void handleMouseButton(int button, int action, int mods);
};