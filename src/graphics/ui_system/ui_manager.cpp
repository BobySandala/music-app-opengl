#include "graphics/ui_system/ui_manager.h"
#include "graphics/renderer.h"

UIManager::UIManager() {}
UIManager::~UIManager() {}

void UIManager::add_element(std::unique_ptr<UIElement> element)
{
    elements.push_back(std::move(element));
}

void UIManager::setMousePos(float x, float y)
{
    mx = x;
    my = y;
}

void UIManager::update(bool mousePressed)
{
    // Iterate over all elements
    for (auto& e : elements)
    {
        if (e->isHover(mx, my)) { e->onHover(); hoveredElement = e.get(); }
        else { e->onUnhover(); }

        if (renderer) renderer->drawQuad(e->getQuad());
    }
}

void UIManager::mouseClick(int mouseBtn)
{
    if(mouseBtn == 0)
        hoveredElement->onLeftClick();
    if(mouseBtn == 1)
        hoveredElement->onRightClick();
}

void UIManager::setEventBus(EventBus& e)
{
    bus = &e;

    size_t id = bus->subscribe<MouseClickEvent>(
        [this](const MouseClickEvent& e)
        {
            mouseClick(e.button);
        }
    );
}
