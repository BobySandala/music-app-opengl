#include "ui_element.h"

void UIElement::onHover() {}
void UIElement::onLeftClick() 
{
    if (callback) callback();
}
void UIElement::onRightClick() {}
void UIElement::onUnhover() {}