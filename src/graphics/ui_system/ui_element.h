#pragma once
#include "graphics/renderer.h"

class UIElement
{
public:
    UIElement(float x, float y, float w, float h,
              float r, float g, float b)
    {
        quad = {x, y, w, h, r, g, b};
    }
    const Quad& getQuad() const
    {
        return quad;
    }

    void setPosition(float x, float y)
    {
        quad.x = x;
        quad.y = y;
    }
    void setColor (float r, float g, float b)
    {
        quad.r = r; quad.g = g; quad.b = b;
    }
    bool isHover(float mx, float my) const
    {
        return mx >= quad.x &&
            mx <= quad.x + quad.w &&
            my >= quad.y &&
            my <= quad.y + quad.h;
    }


protected:
    Quad quad;
};