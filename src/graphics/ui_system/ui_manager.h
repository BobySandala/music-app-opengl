#pragma once
#include <vector>
#include <memory>
#include "graphics/ui_system/ui_element.h"   // base class for UI elements

class Renderer; // forward declaration

class UIManager
{
public:
    // Constructor takes a Renderer reference
    UIManager(Renderer& r);

    // Add a UI element (preserves derived types, no slicing)
    void add_element(std::unique_ptr<UIElement> element);

    // Convenience template for creating and adding elements
    template<typename T, typename... Args>
    void add_element(Args&&... args)
    {
        elements.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    // Update UI elements: call hover/click based on mouse
    void update(bool mousePressed = false);

    // Set the current mouse position
    void setMousePos(float x, float y);

    void mouseClick(int mouseBtn);
private:
    float mx{0}, my{0};                      // mouse position
    Renderer& renderer;                       // reference to renderer
    std::vector<std::unique_ptr<UIElement>> elements; // UI elements
    UIElement* hoveredElement;
};