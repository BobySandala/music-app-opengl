#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>
#include "event/events.h"
#include "event/event_bus.h"

class Input
{
public:
    Input(EventBus& bus);

    void init(GLFWwindow* window);

    // Optional: check current key state
    bool isKeyPressed(int key) const;

private:
    GLFWwindow* window{nullptr};
    EventBus& bus;

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