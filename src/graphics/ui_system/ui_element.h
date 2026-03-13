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

private:
    Quad quad;
};