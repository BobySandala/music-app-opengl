#include "ui_element.h"

UIElement::UIElement(float x, float y, float w, float h,
                     float r, float g, float b)
{
    quad = { x, y, w, h, r, g, b };
}

const Quad& UIElement::getQuad() const
{
    return quad;
}