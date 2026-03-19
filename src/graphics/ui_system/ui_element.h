#pragma once
#include "graphics/renderer.h"
#include "event/events.h"
#include <functional>

class UIElement
{
public:
    using Callback = std::function<void()>;

    UIElement(float x1, float y1, float x2, float y2,
              float r, float g, float b)
    {
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        quad = {x1, y1, x2, y2, r, g, b};
        std:: cout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << std::endl;
    }
    const Quad& getQuad() const
    {
        return quad;
    }

    void setPosition(float x, float y)
    {
        quad.x1 = x;
        quad.y1 = y;
    }
    void setColor (float r, float g, float b)
    {
        quad.r = r; quad.g = g; quad.b = b;
    }
    bool isHover(float mx, float my) const
    {

        //std:: cout << mx << ' ' << my << '|' << quad.x1 << ' ' << quad.y1 << std::endl;
        return  mx >= quad.x1 &&
                mx <= quad.x2 &&
                my >= quad.y1 &&
                my <= quad.y2;
    }

    virtual void onHover();
    void onLeftClick();
    virtual void onRightClick();
    virtual void onUnhover();

    void setCallback(Callback cb)
    {
        callback = cb;
    }

protected:
    Quad quad;
    Callback callback;
};