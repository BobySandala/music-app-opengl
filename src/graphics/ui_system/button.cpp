#include "button.h"

void Button::onClick(float mx, float my)
{
    if (UIElement::isHover(mx, my))
    {
        std::cout << "Click\n";
    }
}

void Button::setPos(float x, float y)
{
    quad.x = x;
    quad.y = y;
}