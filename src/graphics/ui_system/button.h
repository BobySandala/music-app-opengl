#include "ui_element.h"

class Button : public UIElement
{
public:
    Button(float x, float y, float w, float h,
              float r, float g, float b) :
              UIElement {x, y, w, h, r, g, b}{};

    void onClick(float mx, float my);
    void setPos(float x, float y);
};