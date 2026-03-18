#include "audio/audio_engine.h"
#include "core/window.h"
#include "core/input.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "event/event_bus.h"
#include "event/events.h"
#include "graphics/ui_system/ui_manager.h"
#include "graphics/ui_system/button.h"

void busSubsribe(Window& window, AudioEngine& audioEngine, EventBus& bus, UIManager& ui)
{
    // Subscribe to play event
    bus.subscribe<PlayEvent>([&audioEngine](const PlayEvent&)
    { 
        std::cout << "Play event received\n"; 
        AudioCommand cmd {AudiocommandType::Play, ""};
        audioEngine.enqueue(cmd);
    });

    // Subscribe to stop event
    bus.subscribe<StopEvent>([&audioEngine](const StopEvent&)
    { 
        std::cout << "Play event received\n"; 
        AudioCommand cmd {AudiocommandType::Stop, ""};
        audioEngine.enqueue(cmd);
    });

    // subscribe to pause event
    bus.subscribe<PauseEvent>([&audioEngine](const PauseEvent&)
    { 
        std::cout << "Play event received\n"; 
        AudioCommand cmd {AudiocommandType::Pause, ""};
        audioEngine.enqueue(cmd);
    });

    // subscribe to resume event
    bus.subscribe<ResumeEvent>([&audioEngine](const ResumeEvent)
    {
        std::cout << "Resume event received\n";
        AudioCommand cmd {AudiocommandType::Resume, ""};
        audioEngine.enqueue(cmd);
    });

    // subscribe to toggle play event
    bus.subscribe<TogglePlayEvent>([&audioEngine](const TogglePlayEvent)
    {
        std::cout << "Toggle Play Event Received\n";
        AudioCommand cmd {AudiocommandType::TogglePlay, ""};
        audioEngine.enqueue(cmd);
    });

    // Subscribe to exit event
    bus.subscribe<ExitEvent>([&window, &audioEngine](const ExitEvent&)
    { 
        std::cout << "Play event received\n"; 
        AudioCommand cmd {AudiocommandType::Exit, ""};
        audioEngine.enqueue(cmd);
        window.shutdown();
    });

    // Subscribe to mouse click event
    bus.subscribe<MouseClickEvent>([&ui](const MouseClickEvent& e){
        //std::cout << "Mouse click: button=" << e.button << " x=" << e.x << " y=" << e.y << "\n";
        ui.mouseClick(e.button);
    });
}

int main()
{
    Window window(800, 600, "Music Player");

    if(!window.init())
        return -1;

    AudioEngine audioEngine;
    EventBus bus;
    Renderer renderer;
    renderer.init();

    UIManager ui(renderer);

    float btnX = 0.0f;

    ui.add_element<Button>(-0.8f,  0.8f, -0.6f,  0.6f, 0.8f, 0.3f, 0.9f); // top-left
    ui.add_element<Button>( 0.6f,  0.8f,  0.8f,  0.6f, 0.2f, 0.7f, 1.0f); // top-right
    ui.add_element<Button>(-0.8f, -0.6f, -0.6f, -0.8f, 0.9f, 0.2f, 0.4f); // bottom-left
    ui.add_element<Button>( 0.6f, -0.6f,  0.8f, -0.8f, 0.4f, 1.0f, 0.6f); // bottom-right

    // Input system
    Input input(bus);
    input.init(window.getNativeWindow());

    busSubsribe(window, audioEngine, bus, ui);
    while(!window.shouldClose())
    {
        input.update();
        glClear(GL_COLOR_BUFFER_BIT);

        double mx, my;
        input.getRelMousePos(&mx, &my, 800, 600);
        //std::cout << mx << ' ' << my << std::endl;

        //std::cout << button.isHover((float)mx, (float)my) << std::endl;

        //if (button.UIElement::isHover(mx, my)) button.UIElement::setColor(0.0f, 1.0f, 0.0f);
        //else button.UIElement::setColor(1.0f, 0.0f, 0.0f);
        ui.setMousePos(mx, my);
        ui.update();

        //renderer.drawQuad(button.getQuad());

        window.update();

        // Process queued events
        bus.process();;
    }
    window.shutdown();
}