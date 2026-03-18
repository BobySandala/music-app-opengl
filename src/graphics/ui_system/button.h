#include "ui_element.h"

class Button : public UIElement
{
public:
    Button(float x1, float y1, float x2, float y2,
              float r, float g, float b) :
              UIElement {x1, y1, x2, y2, r, g, b}{};

    void onLeftClick() override
    {
        std::cout << "Left Button Clicked" << std::endl;
    }
    
    void onRightClick() override
    {
        std::cout << "Right Button Clicked" << std::endl;
    }

    void onHover() override
    {
        setColor(0.8f, 0.3f, 0.9f);
    }
    void onUnhover() override
    {
        setColor(0.4f, 1.0f, 0.6f);
    }
};