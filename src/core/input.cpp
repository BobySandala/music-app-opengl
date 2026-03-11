#include "core/input.h"
#include <iostream>

// Static instance
Input* Input::instance = nullptr;

Input::Input(EventBus& busRef)
    : bus(busRef)
{
    instance = this;

    // Example key bindings
    keyBindings[GLFW_KEY_SPACE] = [this](){ bus.emit(TogglePlayEvent{}); };
    keyBindings[GLFW_KEY_A]     = [this](){ bus.emit(PlayEvent{}); };
    keyBindings[GLFW_KEY_S]     = [this](){ bus.emit(PauseEvent{}); };
    keyBindings[GLFW_KEY_D]     = [this](){ bus.emit(ResumeEvent{}); };
    keyBindings[GLFW_KEY_F]     = [this](){ bus.emit(StopEvent{}); };
    keyBindings[GLFW_KEY_ESCAPE]= [this](){ bus.emit(ExitEvent{}); };
}

void Input::init(GLFWwindow* win)
{
    if(!win)
    {
        std::cerr << "[Input] Window is null in init\n";
        return;
    }

    window = win;

    // Set GLFW callback
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

bool Input::isKeyPressed(int key) const
{
    if(key < 0 || key >= 512) return false;
    return keys[key];
}

// GLFW static callback
void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(instance)
        instance->handleKey(key, action);
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(instance)
        instance->handleMouseButton(button, action, mods);
}

// Internal mouse button handler
void Input::handleMouseButton(int button, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        if(action == GLFW_PRESS)
        {
            // get the mouse position
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            // Emit mouse click event
            MouseClickEvent event{button, x, y};
            bus.emit(event);
        }
    }
}

// Internal keyboard handler
void Input::handleKey(int key, int action)
{
    if(key < 0 || key >= 512)
        return;

    if(action == GLFW_PRESS)
    {
        keys[key] = true;

        // Emit mapped events
        auto it = keyBindings.find(key);
        if(it != keyBindings.end())
            it->second();
    }
    else if(action == GLFW_RELEASE)
    {
        keys[key] = false;
    }
}